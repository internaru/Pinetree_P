/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// public includes
#include <string.h> // strerror
#include <stdio.h>
#include <sys/stat.h> // chmod()
#include <unistd.h> 

// common includes
#include "error_types.h"
#include "net_api.h" // common net api INITD
#include "os_system.h" // os_system

// net includes
#include "os_network.h" // thread_errno
#include "net_conf_api.h" // file_purpose_t
#include "net_intf_api.h" // get_intf_mac_addr()

//--------------------------------------
// Local Functions
//--------------------------------------

error_type_t read_conf_file(char* filename);
error_type_t write_conf_file(file_purpose_t, char* content);

//--------------------------------------
// Global Variables
//--------------------------------------

// identify configuration files by utility, name and directory
// NOTE: tabled aligned with 'file_purpose_t' (see net_conf_api.h)
net_config_file_t net_conf_files[] = {

    { .purp     = Interface_e,
      .dirname  = "/etc/network",
      .basename = "interfaces",
      .toFS     = write_conf_file,     
      .fromFS   = read_conf_file },

    { .purp     = Bonjour_e,
      .dirname  = INITD,
      .basename = "service.mdns.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = DHCP_client_e,
      .dirname  = "/usr/local/etc",
      .basename = "dhclient.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = SNMP_e,
      .dirname  = "/usr/local/etc/snmp",
      .basename = "snmpd.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = Station_e,
      .dirname  = INITD,
      .basename = "wpa_supplicant.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = uAP_e,
      .dirname  = INITD,
      .basename = "uaputl.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = uAP_udhcp_e,
      .dirname  = INITD,
      .basename = "udhcpd.conf",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },

    { .purp     = WIFI_drivers_e,
      .dirname  = WIFI_DEV_DRIVER,
      .basename = "install_wifi_drivers.sh",
      .toFS     = write_conf_file,
      .fromFS   = read_conf_file },
};

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#define DBG_PRFX "net.conf.api: "
#define DBG_ON
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON

#include "net_debug.h"

//  echo file contents -- returns OK (otherwise ~OK)
#define max_line_bytes 256
error_type_t read_conf_file(char* file) {
    FILE* fd = fopen(file,"r");
    int  fp;
    char line_in[max_line_bytes];
    int bytes_read_sofar = 0;
    if (!fd)
    {
        DBG_VERBOSE("%s(%s) not available!\n", __func__, file);
        return ~OK;
    }
    fp = fileno(fd);
    if (fp < 0)
    {
        if (fd) fclose(fd);
        DBG_VERBOSE("%s(%s) not available!\n", __func__, file);
        return ~OK;
    }

    DBG_VERBOSE("%s(%s):\n", __func__, file);
    int bytes_read =
        read(fp, line_in, max_line_bytes);
    while (bytes_read && !feof(fd))
    {
        bytes_read_sofar += bytes_read;
        if (bytes_read > 0)
        {
            line_in[max_line_bytes-1] = '\0'; // insure terminated str
            dbg_printf("%s", line_in); // echo line (w/o DBG_VERBOSE)

            int err = 0;
            if ((err = fseek(fd, bytes_read_sofar, SEEK_SET)))
            {
                DBG_ERR("%s err %d\n", file, err);
                break; // done w/this file
            }
        }
        bytes_read = read(fp, line_in, max_line_bytes);
    }
	
	if(fflush(fd) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fd->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
    fclose(fd);
    return OK;
}

// returns filename (if successful) -- NOTE empty buffer implies 'touch filename'
char* commit_file_image(char* filename, char* buffer)
{
    if (!filename) return NULL;
    int bytes_written = 0;

    FILE* fd = fopen(filename,"w+");
    if (fd > 0 && buffer != NULL) // fopen err?
    {
        int file_size = strlen(buffer);
        bytes_written = fwrite(buffer, 1, file_size, fd); 
        if (bytes_written != file_size)
        {
            DBG_ERR("%s(%s) err '%s' (%d)\n", __func__, filename, strerror(thread_errno), thread_errno);
            filename = NULL; // implies error
        }
    }
    
    // file error?
    if (!fd)
    {
        DBG_ERR("%s(%s) err '%s' (%d)\n", __func__, filename, strerror(thread_errno), thread_errno);
    }

    if (fd){
		if(fflush(fd) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fd->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fd);
	}
    return filename;
}

// return the given string buffer (filled with fully-qualified filename)
char* lookup_fs_name(file_purpose_t type, char* given_buffer, int max)
{
    if (given_buffer)
    {
        switch (type)
        {
            case Interface_e:
            case SNMP_e:
            case Bonjour_e:
            case DHCP_client_e:
            case Station_e:
            case uAP_e:
            case uAP_udhcp_e:
            case WIFI_drivers_e:

                if ((strlen(net_conf_files[type].dirname) + strlen(net_conf_files[type].basename + 2) > max)) // overflow?; add 2 to account for NULL '/'
                {
                    DBG_ERR("%s() filename '%s/%s' overflow..\n", __func__,
                        net_conf_files[type].dirname,
                        net_conf_files[type].basename);
                }

                // transfer lookup -> given buffer
                strncpy(given_buffer, net_conf_files[type].dirname, max-2);
                given_buffer[max-2] = '\0';
                strcat(given_buffer, "/");
                strncat(given_buffer, net_conf_files[type].basename, max - (strlen(net_conf_files[type].dirname) + 2));
                given_buffer[max-1] = '\0';

                //noisy DBG_VERBOSE("%s() file '%s' (%d)\n", __func__, given_buffer, type);

            break;

            default:
                DBG_ERR("%s() type %d unknown!?\n", __func__, type);
                DBG_ASSERT(0); // fix now
                return NULL;
        };
    }
    return given_buffer;
}

// each configuration file has it's own formatting routine
error_type_t write_conf_file(file_purpose_t which_config, char* updated_image)
{
    error_type_t res = FAIL;
    char conf_file_name[MAX_FILENAME_BYTE_SIZE] = {0}; // space for qualified fs-name
    switch(which_config)
    {

        case Interface_e:
        case Bonjour_e:
        case DHCP_client_e:
        case Station_e:
        case uAP_e:
        case uAP_udhcp_e:
        case WIFI_drivers_e:
        {
            lookup_fs_name(which_config, conf_file_name, MAX_FILENAME_BYTE_SIZE);

            DBG_VERBOSE("%s() %s\n", __func__, conf_file_name);
            if (commit_file_image(conf_file_name, updated_image))
            {
                return OK;
            }
            else
            {
                DBG_ERR("%s() %s err '%s' (%d)\n", __func__,
                    conf_file_name, strerror(thread_errno), thread_errno);
                DBG_ASSERT(0); // fix now
                return FAIL;
            }
        } break;

        case SNMP_e:
        {
            DBG_VERBOSE("%s(%d) file '%s'\n", __func__, which_config, net_conf_files[which_config].basename);
        } break;

        default:
                DBG_ERR("%s() unknown config %d !?\n", __func__, which_config);
                res = OK; // ignored?
            break;
    }
return res;
}

// general file retrieval
// return a memory buffer copy of requested file
char* slurp_file(char* file)
{
    char* mem_buffer = NULL;
    FILE* fd = fopen(file,"r+");

    if (fd == NULL)
    {
        DBG_ERR("%s() file '%s' error?\n", __func__, file);
        return NULL;
    }
    DBG_VERBOSE("%s(%s):\n", __func__, file);

    // determine conf file size
    fseek(fd, 0L, SEEK_END);
    int last_byte = ftell(fd);
    fseek(fd, 0L, SEEK_SET); // rewind to beginning

    if (0 < last_byte) // non-empty file
    {
        mem_buffer = malloc(last_byte+1);
        if (mem_buffer != NULL)
        {
            int bytes_read_total = 0;
            int bytes_read = fread(mem_buffer, 1, last_byte, fd); 
            while (bytes_read && bytes_read_total < last_byte)
            {
                if (bytes_read > 0)
                {
                    bytes_read_total += bytes_read;
                }
                else if (feof(fd)) break;
            }
            if (bytes_read_total < last_byte) // note shortage
            {
                DBG_ERR("%s() read %d/%d bytes\n", __func__, bytes_read_total, last_byte);
            }
            mem_buffer[bytes_read_total] = '\0'; //null terminate buffer
        }
    }
	if(fflush(fd) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fd->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
    fclose(fd);
    DBG_VERBOSE("%s() returning %d-byte buf @ x%x\n", __func__, last_byte, mem_buffer);
    return mem_buffer;
}

// general file retrieval (slow byte x byte)
// return a memory buffer copy of requested file
#define hostname_bytesize 128
char* slurp_file_hostname(char* file)
{
    char stack_buf[hostname_bytesize]={0};
	char *loc_buf = stack_buf;
    char* new_buffer = NULL;
    FILE* fd = fopen(file,"r");
    int last_byte = 0;

    DBG_VERBOSE("%s(%s):\n", __func__, file);
    if (fd == NULL)
    {
        DBG_ERR("%s(%s) error %d\n", __func__, file, thread_errno);
        return NULL;
    }

    // count filesize byte by byte
    while (!feof(fd))
    {
        fread(loc_buf++, 1, 1, fd);
        last_byte++;
    }
    last_byte--;

    if (0 < last_byte) // non-empty file
    {
        loc_buf=stack_buf;
        while (*loc_buf++ != 0x0a && last_byte--); *(loc_buf-1)='\0'; // trim newline
        last_byte = strlen(stack_buf);
        DBG_VERBOSE("%s contains %d bytes\n", file, last_byte);
        new_buffer = (char*) malloc(hostname_bytesize);
        if (new_buffer != NULL)
        {
            DBG_ERR("%s(%s) no memory!\n", __func__, file);
            strcpy(new_buffer, stack_buf);
        }
    }
    fclose(fd);
    return new_buffer;
}

// (re)write an executable sh script
// first empty line in script signals end-of-file content
error_type_t write_sh_file(char* filename, char** sh_line)
{
    error_type_t res = OK;
    if (!filename || !sh_line) return ~OK;

    if (unlink(filename)) // make way for a fresh file image
    {
        DBG_VERBOSE("%s(%s) delete '%s' ignored..\n", __func__, filename, strerror(thread_errno));
    }

    // include 'executable' in file creation privileges
    mode_t exe_mask = S_IRWXU | S_IXGRP | S_IXOTH; // all exe privileges
    mode_t sync_mask = O_DIRECT; // minimize cache effects
    int fp = open(filename, FD_CLOEXEC | O_RDWR | (O_EXCL | O_CREAT | O_TRUNC), sync_mask | exe_mask); // touch
    if (fp < 0)
    {
        DBG_ERR("%s(%s) open %s (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
        return FAIL;
    }
    close(fp); // close executable file
    FILE* fd = fopen(filename,"w+"); // reopen/truncate file
    if (!fd)
    {
        DBG_ERR("%s(%s) open %s (err %d)\n", __func__, filename, strerror(thread_errno), thread_errno);
        return FAIL;
    }
 
    // transfer line x line (1st empty line stops write)
    int i = 0;
    char* line = sh_line[i];
    while (line && res == OK)
    {
        int line_size = (line ? strlen(line) : 0);
        if (line_size>0)
        {
            int bytes_written = fwrite(sh_line[i], 1, line_size, fd);
            if (bytes_written != line_size)
            {
                DBG_ERR("%s(%s) write err '%s' (%d)\n", __func__, filename, sh_line[i], strerror(thread_errno), thread_errno);
                res = thread_errno;
            }
        }
        fwrite("\n", 1, strlen("\n"), fd); // newline delimiter
        i++;
        // next line
        line = sh_line[i];
    }
	if(fflush(fd) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fd->_fileno) == -1){
		DBG_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
    fclose(fd);

    return res;
}


inline char* add_newline(char* next)
{
    *next++ = '\n';
    return next;
}

// copy/transfer given array of strings into given loc (delimited with '/n')
char* add_txt_records(char* loc, char** txt_records)
{
    int i = 0;
    while (txt_records[i] != NULL)
    {
        strcpy(loc, txt_records[i]);
        loc += strlen(txt_records[i]);
        loc = add_newline(loc);
        i++; // next txt
    }
    return loc;
}

char* ipv4_b2str(uint32_t addr); // convert a 32-bit addr rep -> dotted notation str
// convert a 32-bit addr rep -> dotted notation str
// local buffer needs to be copied
static char v4ip_addr_str[20]; // need 16 revisit
uint8_t nibbles[4];
char* ipv4_b2str(uint32_t addr_b32)
{
    if (addr_b32)
    {
        memcpy(nibbles, &addr_b32, 4); //nibbles = (uint8_t*)&addr_b32; // cast
        sprintf(v4ip_addr_str, "%d.%d.%d.%d", nibbles[0], nibbles[1], nibbles[2], nibbles[3]);
    }
    else sprintf(v4ip_addr_str, "0.0.0.0");

    DBG_VERBOSE("%s(x%x) -> %s\n", __func__, addr_b32, v4ip_addr_str);
    return v4ip_addr_str;
}

// convert an ipv4 addr str -> 32-bit rep
uint32_t ipv4_str2b32(char* ipv4_str)
{
    union {
        uint32_t b32;
        uint8_t  b8[4];
    } addr_u = { 0 };
    int b1, b2, b3, b4;

    if (ipv4_str)
    {
        sscanf(ipv4_str, "%d.%d.%d.%d", &b1, &b2, &b3, &b4);

        // transfer digits -> packed 32-bits
        addr_u.b8[0] = (uint8_t)b1;
        addr_u.b8[1] = (uint8_t)b2;
        addr_u.b8[2] = (uint8_t)b3;
        addr_u.b8[3] = (uint8_t)b4;
    }

    DBG_VERBOSE("%s(%s) -> (%d.%d.%d.%d) (x%x)\n", __func__, ipv4_str,
         (addr_u.b8[0]), (addr_u.b8[1]), (addr_u.b8[2]), (addr_u.b8[3]), addr_u.b32);

    return addr_u.b32;
}

// convert a 32-bit addr rep into a dotted notation str buffer (provided)
// NOTE - assumes buffer is at least 16 bytes long
//   returns buf .. with a nul-terminated c-string (if buf provided)

char* ipv4_b2str_inplace(uint32_t addr_b32, char* buf)
{
    if (buf)
    {
        strncpy(buf, ipv4_b2str(addr_b32), 16);
        buf[15] = '\0'; 
    }
    return buf;
}

// len == 1 returns NULL str (if buf provided)
char* ipv4_b2str_inplace_with_len(uint32_t addr_b32, char* buf, int len)
{
    if (buf && len > 0)
    {
        strncpy(buf, ipv4_b2str(addr_b32), len);
        buf[len-1] = '\0'; 
    }
    return buf;
}


// place a double-quoted string into location 'next' -- returns char* just beyond last "
#define double_quote 0x22
char* add_quoted_string(char* next, char* string)
{
    if (!string) return next;

   *next++ = (char)double_quote;
    strcat(next, string);
    next += strlen(string);
   *next++ = (char)double_quote;

    return next;
}

// general check for file existance
bool file_exists(const char* filename)
{
    if (!filename) return false;
    FILE* file = fopen(filename,"r");
    bool exists = file ? true : false;
    if (exists){
		fseek(file, 0L, SEEK_END);
		if (!ftell(file)) exists = false;
		fclose(file);
	}
    return exists;
}

#ifdef HAVE_WIRELESS
// retrieve current os-socket layer assigned wifi mac address -- returns NULL if unsuccessful
static uint8_t* oid_get_mac_wifi(uint8_t* mac_addr_buf, int buf_len)
{
    if (!mac_addr_buf) return NULL; // buf required (assumes full-size MAC_ADDR_BYTE_SIZE mac)

    uint8_t* mac_addr = get_intf_mac_addr(NET_IF_NAME_UAP, mac_addr_buf); // 1st-choice NET_UAP_LINK_NAME
    if (!mac_addr || null_mac(mac_addr, buf_len)) 
        mac_addr = get_intf_mac_addr(NET_IF_NAME_STA, mac_addr_buf); // 2nd-choice NET_WLAN_LINK_NAME

    if (mac_addr)
    {
        memcpy(mac_addr_buf, mac_addr, buf_len);
        DBG_VERBOSE("%s() mac [%02x:%02x:%02x:%02x:%02x:%02x]\n", __func__,
            mac_addr_buf[0], mac_addr_buf[1], mac_addr_buf[2],
            mac_addr_buf[3], mac_addr_buf[4], mac_addr_buf[5]
        );
        return mac_addr_buf;
    }
    return NULL;
}

// generate_uniq_ssid() -- embellish DEFAULT_UAP_SSID w/mac suffix
char* generate_uniq_ssid(char* uniq_name_buf)
{
    uint8_t  oid_mac[MAC_ADDR_BYTE_SIZE] = { 0 };
    if (!uniq_name_buf) return NULL; // nothing to be done

    oid_get_mac_wifi(oid_mac, sizeof(oid_mac)); 

    // create 'unique' devname as 'service_name[mac[3,5]]'
    const char* fmt = "%s [%02x%02x%02x]\0";
    sprintf(uniq_name_buf, fmt, DEFAULT_UAP_SSID, oid_mac[3], oid_mac[4], oid_mac[5]);
    DBG_VERBOSE("%s() uniq ssid -> '%s'\n", __func__, uniq_name_buf);
    return uniq_name_buf;
}
#endif // HAVE_WIRELESS

#ifdef __cplusplus
}
#endif

// eof

