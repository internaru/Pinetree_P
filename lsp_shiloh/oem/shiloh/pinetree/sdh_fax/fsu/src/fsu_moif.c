/*****************************************************************
***               Copyright (c) 2008 - 2009                    ***
***               Conexant Systems Inc.                        ***
***               Imaging and PC Media                         ***
***               All Rights Reserved                          ***
***                                                            ***
***            CONFIDENTIAL -- No Dissemination or             ***
***            use without prior written permission.           ***
***                                                            ***
******************************************************************
*                                                                *
*  2008 - 2009 Modification for SSF336                        tm *
*                                                                *
******************************************************************/

#include "dc_common_includes.h"
#include "tx_api.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "fshs.h"
#include <linux/types.h>
#include "error_types.h"
#include "gpio_api.h"
#include "GPIO_regstructs.h"
#include "GPIO_regmasks.h"
#include "mrvl_gpio_linux_config.h"
#include "faxapp_notify.h"

#define SPI_IOC_MAGIC			'k'

struct spi_ioc_transfer {
	__u64		tx_buf;
	__u64		rx_buf;

	__u32		len;
	__u32		speed_hz;

	__u16		delay_usecs;
	__u8		bits_per_word;
	__u8		cs_change;
	__u32		pad;
};

#define SPI_MSGSIZE(N) \
	((((N)*(sizeof (struct spi_ioc_transfer))) < (1 << _IOC_SIZEBITS)) \
		? ((N)*(sizeof (struct spi_ioc_transfer))) : 0)

#define SPI_IOC_MESSAGE(N) _IOW(SPI_IOC_MAGIC, 0, char[SPI_MSGSIZE(N)])

#define SPI_IO_MODE_POLLING     0
#define USART_IO_MODE_INTERRUPT 1
#define SPI_MAX_DATA_SIZE       16

UINT8 tx_prefix0[ SPI_MAX_DATA_SIZE ] = {0, };
UINT8 rx_prefix0[ SPI_MAX_DATA_SIZE ] = {0, };
static int spi_dev = 0;

void  modem_spi_reset( void );
void  modem_spi_clear_reset( void );
void  modem_spi_write( UINT8 addr, UINT8 data );
UINT8 spi_read_addr(UINT8 addr, UINT8 readlen);
UINT8 modem_spi_read( UINT8 addr );
void  modem_spi_multi_write( UINT8 addr, UINT8 *data, UINT8 cnt );
void  modem_spi_multi_read( UINT8 addr, UINT8 *data, UINT8 cnt );
UINT8 modem_spi_open( void );


void modem_spi_reset( void )
{
    UINT8 prefix_length0 = 0;

	/* SSF336 reset code */
	tx_prefix0[ 0 ] = 0xeb;
	tx_prefix0[ 1 ] = 0xaf;
	tx_prefix0[ 2 ] = 0x55;
	tx_prefix0[ 3 ] = 0x4d;

	prefix_length0 = 4;

    if (write(spi_dev, tx_prefix0, prefix_length0) == -1)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_RESET);
    }

    tx_thread_sleep( 10 );
}


void modem_spi_clear_reset( void )
{
	/* SSF336 clear reset */
	modem_spi_write( 0x2e, 0x07 );

    tx_thread_sleep( 100 );
}


void modem_spi_write( UINT8 addr, UINT8 data )
{
    UINT8 prefix_length0 = 0;
    UINT8 tx_prefix0[32] = {0, };

	tx_prefix0[ 0 ] = 0x80 | ( addr << 1 );
	tx_prefix0[ 1 ] = data;
	
	prefix_length0 = 2;

    if (write(spi_dev, tx_prefix0, prefix_length0) == -1)
    {
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_WRITE);
    }
}


UINT8 spi_read_addr(UINT8 addr, UINT8 readlen)
{
    struct spi_ioc_transfer xfer[1];
    int status = 0;

	xfer[0].speed_hz = 0;
	xfer[0].delay_usecs = 0;
	xfer[0].bits_per_word = 0;
	xfer[0].cs_change = 0;
	xfer[0].pad = 0;
    
    tx_prefix0[0] = addr;
    xfer[0].tx_buf = (unsigned long)tx_prefix0;
    xfer[0].len = 2;
    
    xfer[0].rx_buf = (unsigned long)tx_prefix0;
    xfer[0].len = readlen;
    
    status = ioctl(spi_dev, SPI_IOC_MESSAGE(1), xfer);
    if (status < 0) {
        perror("SPI_IOC_MESSAGE");
        return 0;
    }
    
    return tx_prefix0[1];
}


UINT8 modem_spi_read( UINT8 addr )
{
    UINT8 return_val = 0;

    addr = addr<<1; // shift the read addr for the modem's protocol

    return_val = spi_read_addr(addr,2); // read 1 byte at addr
	
    return return_val;
}


void modem_spi_multi_write( UINT8 addr, UINT8 *data, UINT8 cnt )
{
    UINT8 prefix_length0 = 0;
    int i = 0;

	tx_prefix0[ 0 ] = addr;

	for ( i = 0; i < cnt; i++ )
	{
		tx_prefix0[ i + 1 ] = data[ i ];
	}

	prefix_length0 = cnt + 1;

    if (write(spi_dev, tx_prefix0, prefix_length0) == -1)
    {
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_MULTI_WRITE);
    }
}


void modem_spi_multi_read( UINT8 addr, UINT8 *data, UINT8 cnt )
{
	struct spi_ioc_transfer xfer[1];
    int status = 0;
    int i = 0;

	xfer[0].speed_hz = 0;
	xfer[0].delay_usecs = 0;
	xfer[0].bits_per_word = 0;
	xfer[0].cs_change = 0;
	xfer[0].pad = 0;
    
    tx_prefix0[0] = addr;
#if 1
    for( i = 0; i < cnt; i++ ) {
    	tx_prefix0[i+1] = 0xff;	/* dummy for read time */
    }
#endif
    xfer[0].tx_buf = (unsigned long)tx_prefix0;
    xfer[0].len = cnt + 1;
    
    xfer[0].rx_buf = (unsigned long)tx_prefix0;
    xfer[0].len = cnt + 1;
    
    status = ioctl(spi_dev, SPI_IOC_MESSAGE(1), xfer);
    if (status < 0) {
        perror("SPI_IOC_MESSAGE");
        return;
    }

	for ( i = 0; i < cnt; i++ )
	{
		data[ i ] = tx_prefix0[ i + 1 ];
	}
}


UINT8 modem_spi_open(void)
{
    UINT8 ret_val = 0;

	gpio_handle_t *fax_gpio_handler = NULL;

	if( gpio_open(&fax_gpio_handler,   // open, default and ignore: //out parameter
				GPIO_BANK_H,
				20,
				GPIO_DIRECTION_OUTPUT,
				GPIO_LOGIC_SETTING_ACTIVE_HIGH, //=0x0
				0) != OK)
	{
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_OPEN_1);
	}

	if( gpio_write(fax_gpio_handler, 1) )
	{
		faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_OPEN_2);
	}

	usleep(400*1000);

	spi_dev = open("/dev/spidev2.0", O_RDWR);
	if(spi_dev == -1)
	{
        printf("\n(YHCHO) modem_spi_open() - open /dev/spidev2.0 FAILED!!!\n");
        ret_val = 1;
	}
	else
	{
        modem_spi_reset();

        sleep(1);
		
		modem_spi_clear_reset();
		
        sleep(1);

        ret_val = modem_spi_read( 0x12 );
        if (ret_val != 0xAA)
		{
            printf("\n(YHCHO) modem_spi_open() - mode_spi_read FAILED!!!\n");            
            ret_val = 1;
		}
        else
        {
            ret_val = 0;
        }
	}

    return ret_val;
}

