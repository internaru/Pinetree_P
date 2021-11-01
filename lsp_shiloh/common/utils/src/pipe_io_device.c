/*
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// Attach a file or pipe to an io_device for use as input as a connection mgr "PIPE"
#include <stdlib.h>
#include "io_device.h"
#include "agConnectMgr.h"
#include <poll.h>
#include "pipe_io_device.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "dbg_api.h"
#include <string.h>
#include "agConnectMgr.h"
#include "debug.h"

#include "logger.h"
#define DBG_PRFX "PioD: "

#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT( 14 )

#ifdef __linux__
#include <limits.h>
#define STACK_SIZE PTHREAD_STACK_MIN 
ALIGN8 unsigned char Stack[STACK_SIZE]; ///< thread stack.
static pthread_t accept_list_thd_id;
#endif

typedef struct io_device_impl_s io_generic_fd_t;
typedef struct writebuf_list_s writebuf_list_t;
typedef struct writebuf_s writebuf_t;

struct writebuf_s 
{
    void *data;
    uint32_t length;
    int fd;
    writebuf_t *next;
};

struct writebuf_list_s
{
    writebuf_t *head;
    writebuf_t *tail;
};

struct io_device_impl_s 
{
    int fd;
    void *pipe;
    writebuf_list_t writebuf_list;
};

/// write commands to these pipes to break caller thread context and
/// read from the other end on the pipe_io_device thread to act on the command.
static int pipe_register_fd;     ///< register a new pipe
static int ioctl_write_fd;       ///< e_close
static int backchannel_write_fd; ///< add output buffer to backchannel

int active_nfds = 0; 
int registered_fds = 0;

static int USB2O_AS_GPRINTER_DEVICE = 0;

#define DEV_G_PRINTER_USB2O "/dev/g_printer_usb2o"
#define DEV_G_SCAN_USB2O "/dev/g_scan_usb2o"

static int g_printer_fd = 0; // work around for gprinter usb cable disconnect is open/close device on idle.
static int g_printerUSB2o_fd = 0; // work around for gprinter_usb2o usb cable disconnect is open/close device on idle.


#define MAX_NDFS 50
struct pollfd fds[MAX_NDFS];
io_generic_fd_t *io_generic_fd_array[50];

/// adds backchannel data to a list of output messages
/// 
static void add_write_string( struct writebuf_s *writebuf )
{
    int i, j;
    
    for (i=0; i < MAX_NDFS; i++)
    {
        if (io_generic_fd_array[i]->fd == writebuf->fd)
        {
            if (io_generic_fd_array[i]->writebuf_list.tail == NULL)
                io_generic_fd_array[i]->writebuf_list.tail = io_generic_fd_array[i]->writebuf_list.head = writebuf;
            else 
            {
                io_generic_fd_array[i]->writebuf_list.tail->next = writebuf;
                io_generic_fd_array[i]->writebuf_list.tail = writebuf;
            }

                
                for (j=0; j < MAX_NDFS; j++) 
                { 
                    if (fds[j].fd == writebuf->fd) 
                    {
                        fds[j].events |= (POLLOUT | POLLWRNORM);  
                        break;
                    }
                }
            break;
        }
    }
}

/// if we have data for output and the receiver is reading we then search for the data to send.
///
static void perhaps_write( int fd )
{
    int i, j;
    int length, retval;
    writebuf_t *ptr;
    unsigned char *rover;
  
    for (i=0; i < registered_fds; i++)  
    { 
        if (io_generic_fd_array[i]->fd == fd && io_generic_fd_array[i]->writebuf_list.head)   
        {
            ptr = io_generic_fd_array[i]->writebuf_list.head;
            length = ptr->length;
            rover = ptr->data;
            while (length > 0)
            {
                retval = write(fd, rover, length);  
                if (retval < 0)
		{
		    dbg_printf("usb write failure retval %d errno %d \n", retval, errno);
		    break;
		}
                rover += retval;
                length -= retval;
            }
            
            io_generic_fd_array[i]->writebuf_list.head = ptr->next;
            memFree(ptr->data);
            memFree(ptr);
            if (io_generic_fd_array[i]->writebuf_list.head == NULL) 
            {
                io_generic_fd_array[i]->writebuf_list.tail = NULL;  // we took the last item off the list
                for (j=0; j < active_nfds; j++) 
                { 
                    if (fds[j].fd == fd) 
                    {
                        fds[j].events &= ~(POLLOUT | POLLWRNORM);
                        break;
                    }
                }
                break;      
            }
        }
    }
}

/// parser or connection manager uses this write function.
///
int32_t io_device_generic_fh_pipe_write(io_device_impl_t *fh, 
                                        void *DataBuffer, 
                                        uint32_t Length, 
                                        uint32_t TimeOut ///< centi seconds
                                        ) 
{
    int res;
    writebuf_t writebuf;

    writebuf.data = DataBuffer;
    writebuf.length = Length;
    writebuf.fd = fh->fd;
    writebuf.next = 0;

    
    res = write( backchannel_write_fd, &writebuf, sizeof(struct writebuf_s));

    if ( res == sizeof(struct writebuf_s))
        return Length;
    return -1;
}

/// parser or connection manager uses this read function.
///
int32_t io_device_generic_fh_pipe_read(io_device_impl_t *fh, 
                                       void *DataBuffer,
                                       uint32_t Length, 
                                       uint32_t TimeOut ///< centi seconds
                                       ) 
{
    int res;
    struct pollfd fds;
    fds.fd = fh->fd;
    fds.events = (POLLIN | POLLRDNORM);

    res = poll( &fds, 1, TimeOut * 10 ); 
  
    switch (res)
    {
    case 0: // timeout

        return CONIO_TIMEOUT;
    case 1:
        res = read( fds.fd, DataBuffer, Length );
        break;
    default:
        res = -1;
    }
    return res;
}

/// e_close is the only ioctl supported.
/// connection manager closes a pipe so we will now block on data.
/// send internal message to our poll thread.
int32_t io_device_generic_fh_pipe_ioctl(io_device_impl_t *fh, 
                                        ioctl_cmd_t Cmd, 
                                        void *Details)       ///< IOctal interface to driver
{
    return write(ioctl_write_fd, &fh->fd, sizeof(int));
}


/// internal add a new file/pipe to accept data on.
static void add_to_accept_list( int fd )
{
    int i;

    io_generic_fd_t *io_generic_fd = (io_generic_fd_t*) MEM_MALLOC(sizeof(io_generic_fd_t));
  
    read(fd, io_generic_fd, sizeof(io_generic_fd_t));

    for (i = 0 ; i < active_nfds; i++) 
    {
        if (io_generic_fd->fd == fds[i].fd) 
        {
            fds[i].events = (POLLIN | POLLRDNORM);
            break;
        }
    }
    if ( i == active_nfds ) 
    {
        fds[active_nfds].fd = io_generic_fd->fd;
        fds[active_nfds].events = (POLLIN | POLLRDNORM);
        ++active_nfds;
    }

    for (i = 0 ; i < registered_fds; i++) 
    {
        if (io_generic_fd->fd == io_generic_fd_array[i]->fd)
            break;
    }
    if ( i == registered_fds ) 
    {
        io_generic_fd_array[registered_fds++] = io_generic_fd;
    }
    else 
    {
	memFree( io_generic_fd ); // already registered free memory.
    }
}

/// pipe e_close means pipe will no longer read we now block on read 
/// and send a PipeConnect if new data comes in.
///
static void pio_accept_read( int fd )
{
    int j;
    io_generic_fd_t io_generic_fd;
    int err;

    err = read(fd, &io_generic_fd, sizeof(io_generic_fd_t));
    UNUSED_VAR( err ); 
    for (j = 0 ; j < active_nfds; j++) 
    { 
        if (io_generic_fd.fd == fds[j].fd) 
        {
            fds[j].events = (POLLIN | POLLRDNORM);
        }
    }
}
 

/// This thread sits between the files/pipes on the input side, /dev/g_printer the usb device for instance
/// and the reading pipe in the connection manager or the parser.  It uses poll to wait on messages in the 
/// first 3 file descriptors for instructions on what to do and the remaining file descriptors are the 
/// read write pipes to route usb/file/pipe input to and from  the "Pipe".
void *accept_list_thread(void *unused)
{
    int i,j;
    int ret;
    int nfds;
    int pipefd[2];
    
    if (pipe2(pipefd, O_CLOEXEC) == -1) // command pipe for register messages.
    {
        ASSERT(0 == "pipecreate failure");
    }
    fds[0].fd = pipefd[0]; // read
    fds[0].events = POLLIN;
    pipe_register_fd = pipefd[1]; // write

    if (pipe2(pipefd, O_CLOEXEC) == -1)  // command pipe for e_close messages.
    {
        ASSERT(0 == "pipecreate failure");
    }
    fds[1].fd = pipefd[0];
    ioctl_write_fd = pipefd[1];
    fds[1].events = POLLIN;

    if (pipe2(pipefd, O_CLOEXEC) == -1)  // command pipe for backchannel write messages.
    {
        ASSERT(0 == "pipecreate failure");
    }
    fds[2].fd = pipefd[0];
    backchannel_write_fd = pipefd[1];
    fds[2].events = POLLIN;
    active_nfds = 3; 

    // LSPTODO: move this to a usb device that sits ontop of the g_printer.
    // or use a "pjl" parser to sit inbetween interpreting backchannel data.

    SysWaitForInit();

	char *m_tli = NULL;
	m_tli = getenv("TLI");
    if(m_tli != NULL){
		if( strncmp(m_tli, "A616DNS", 7) == 0 ) {
			USB2O_AS_GPRINTER_DEVICE = 1;
			dbg_printf("USB2O_AS_GPRINTER_DEVICE: %d\n", USB2O_AS_GPRINTER_DEVICE);
		}
	}

#if defined (HAVE_USB_PRINTER_GADGET)
	g_printer_fd = pipe_io_device_register( "/dev/g_printer", 0, e_Print );
	if(USB2O_AS_GPRINTER_DEVICE){
		g_printerUSB2o_fd = pipe_io_device_register( DEV_G_PRINTER_USB2O, 0, e_Print );
	}
#endif
#if defined (HAVE_USB_SCAN_GADGET)
	pipe_io_device_register( "/dev/g_scan", 0, e_Scan );
#if 0	///TODO: Future use for Scan from USB2o
	if(USB2O_AS_GPRINTER_DEVICE){
		pipe_io_device_register(DEV_G_SCAN_USB2O, 0, e_Scan );
	}
#endif
#endif



    while(1) 
    {
        nfds = active_nfds;
        ret = poll( fds, nfds, /* -1 */ 30000 );  // block waiting for commands or data.
	if ( 0 == ret )
        {
            int new_fd;
            IOReg *pipe = 0;
            io_generic_fd_t *io;
            /// workaround g_printer bug where it expects open/close rather
            /// than open forever, this way a cable insert is detected via polling.

            for (i = 0; i < registered_fds; i++)
            {
                if (g_printer_fd == io_generic_fd_array[i]->fd)
                {
                    pipe = (IOReg *)io_generic_fd_array[i]->pipe;
                    break;
                }
            }
            if (pipe && pipe->PipeState == e_Idle)
            {
                // idle, we hope, close and reopen the device to allow it to change
                // file descriptors on cable plug/unplug.
		// DBG_PRINTF_SDH_DEBUG("closing gprinter %d\n", g_printer_fd);

                close(g_printer_fd);
                new_fd = open("/dev/g_printer", O_RDWR);  // close open may give back a different fd

                if (g_printer_fd != new_fd)
                {
                    // now we update all our references to the file descriptor, YUCK.
                    // dbg_printf("aliasing g_printer fd %d %d\n", g_printer_fd, new_fd);
                    for (i = 0; i < nfds; i++)
                    {
                        if (fds[i].fd == g_printer_fd)
                        {
                            fds[i].fd = new_fd; // poll fd update, safe since its my poll.
                            break;
                        }
                    }
                    for (i = 0; i < registered_fds; i++)
                    {
                        if (g_printer_fd == io_generic_fd_array[i]->fd)
                        {
                            io_generic_fd_array[i]->fd = new_fd;  // registered fd update.
                            break;
                        }
                    }

                    if (pipe)
                    {
                        io = (io_generic_fd_t *)pipe->io.data_read_handle; // violate the pipe's encapsulation
                        if (io) // warning will robinson
                        {
                            io->fd = new_fd; // Pipe's fd update not so safe assumes e_Idle really is idle.
                        }
                    }
                }
                g_printer_fd = new_fd;  // global is just for this loop to keep track of g_printer, sigh.
            } /// else not idle poll timeout again.

				//USB2o check /dev/g_printer_usb2o plug/unplug state	
			if(USB2O_AS_GPRINTER_DEVICE){
				for (i = 0 ; i < registered_fds; i++)  {
					if (g_printerUSB2o_fd == io_generic_fd_array[i]->fd) {
						pipe = (IOReg*) io_generic_fd_array[i]->pipe;
						break;
					}
				}
				if (pipe && pipe->PipeState == e_Idle) {  
					// idle, we hope, close and reopen the device to allow it to change 
					// file descriptors on cable plug/unplug.
					// DBG_PRINTF_SDH_DEBUG("closing g_printerUSB2o_fd %d\n", g_printerUSB2o_fd);

					close(g_printerUSB2o_fd);
					new_fd = open(DEV_G_PRINTER_USB2O, O_RDWR );  // close open may give back a different fd

					if ( g_printerUSB2o_fd != new_fd ) {  
						// now we update all our references to the file descriptor, YUCK.  
						// DBG_PRINTF_SDH_DEBUG("aliasing g_printerUSB2o  fd %d %d\n", g_printerUSB2o_fd, new_fd);
						for (i = 0; i < nfds; i++) {
							if (fds[i].fd == g_printerUSB2o_fd) {
								fds[i].fd = new_fd; // poll fd update, safe since its my poll.          
								break;
							}
						}
						for (i = 0 ; i < registered_fds; i++)  {
							if (g_printerUSB2o_fd == io_generic_fd_array[i]->fd) {
								io_generic_fd_array[i]->fd = new_fd;  // registered fd update.
								break;
							}
						}

						if (pipe) { 
							io = (io_generic_fd_t*)pipe->io.data_read_handle; // violate the pipe's encapsulation 
							if (io) { // warning will robinson
								io->fd = new_fd; // Pipe's fd update not so safe assumes e_Idle really is idle.
							}
						}
						g_printerUSB2o_fd = new_fd;  // global is just for this loop to keep track of g_printer_usb2o, sigh.	    
					}
				} /// else not idle poll timeout again.
			}
		}  /// end of g_printer and g_printer_usb2o plug / unplug workaround.
        for (i=0; i < nfds && ret > 0; i++) 
        {
            if (!fds[i].revents) 
                continue;
            ret--;
            if ( i == 0 ) 
            { // REGISTER
                add_to_accept_list( fds[0].fd );
                break;
            }
            else if ( i == 1 ) 
            { // e_close 
                pio_accept_read( fds[1].fd );
            }
            else if ( i == 2 ) 
            { // add backchannel data to a write queue.
                struct writebuf_s *writebuf;
        
                writebuf = (writebuf_t*) MEM_MALLOC(sizeof(struct writebuf_s));
                read( fds[2].fd, writebuf, sizeof(struct writebuf_s));
                add_write_string( writebuf );
            }
            
            else if ( fds[i].revents & (POLLOUT | POLLWRNORM) ) 
            {   // write backchannel data iff channel is ready and we have data. 
                perhaps_write( fds[i].fd );
            }
            else if ( fds[i].revents & (POLLIN | POLLRDNORM) ) 
            {
                fds[i].events &= ~(POLLIN | POLLRDNORM); // switch read to piperead function
                for (j = 0 ; j < MAX_NDFS; j++) 
                {
                    DBG_ASSERT( io_generic_fd_array[j] ); // null or not found error.
                    if (fds[i].fd == io_generic_fd_array[j]->fd) 
                    {
                        DBG_PRINTF_SDH_ERR("ksjung--pipe_io_device call PipeConnect\n");
                        PipeConnect(io_generic_fd_array[j]->pipe);  // Pipe will now read then will e_close.
                        break;  // 
                    }
                }
            }
        }
    }
    return 0;
}

/// sysinit function
void pipe_io_device_init(void)
{
    posix_create_thread(&accept_list_thd_id, accept_list_thread, 0, "io_accept", 
                        Stack, STACK_SIZE, POSIX_THR_PRI_NORMAL);

}




int pipe_io_device_register( const char * readfile, const char *statusfile, Channel_type_t channel_type)
{
    PIPE_INFO pipe_info;
    void * pipe;
    int fd;
    int fds=0;
    
    io_generic_fd_t *io_generic_fd = (io_generic_fd_t *)MEM_MALLOC(sizeof(io_generic_fd_t));
    
    memset( &pipe_info, 0, sizeof(PIPE_INFO));
    
    pipe_info.io.data_read = io_device_generic_fh_pipe_read;
    pipe_info.io.data_write = io_device_generic_fh_pipe_write;
    pipe_info.io.cmd_write = 0; // io_device_generic_fh_pipe_cmd_write;
    pipe_info.io.cmd_read = 0; // interrupt endpoint is write only.
    pipe_info.io.ioctl = io_device_generic_fh_pipe_ioctl;
    pipe_info.channel_type = channel_type;
    pipe_info.DocType = e_PIPEIOPAGE;	//LALIT: Set new Document type to get in Print Mgr for Host Print counter
    
    fd = open( readfile, O_RDWR );
    if (fd < 0 ) {
        dbg_printf("open fail %s fd %d errno %d \n", readfile, fd, errno);
	memFree( io_generic_fd );
        return fd;
    }
    XASSERT( fd >= 0, errno );
    
    
    pipe_info.io.data_read_handle = io_generic_fd;
    pipe_info.io.data_write_handle = io_generic_fd;
    
#if 0
    if (statusfile) 
    {
        fds = open( statusfile, O_RDWR );
        ASSERT( fd >= 0 );
        pipe_info.io.data_write_handle = io_generic_fd;
    }
    pipe_info.io.data_write_handle = 0;
#endif

    if( (pipe = PipeRegister(&pipe_info)))
    {
        ASSERT( pipe );
    }
    io_generic_fd->fd = fd;
    io_generic_fd->pipe = pipe;
    io_generic_fd->writebuf_list.head = 0;
    io_generic_fd->writebuf_list.tail = 0;
    write( pipe_register_fd, io_generic_fd, sizeof(io_generic_fd_t));
    
    if (statusfile && fds) 
    {
        io_generic_fd->fd = fds;
        write( pipe_register_fd, io_generic_fd, sizeof(io_generic_fd_t));
    }
    return fd;
}

void pipe_io_device_test()
{

}
