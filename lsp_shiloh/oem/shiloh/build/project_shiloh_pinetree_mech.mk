### LSP pinetree using granite C0 or later.

# build sanity check
HAVE_PROJECT_PRODUCT_MK = 1 

# only enable when building sdk
#BUILD_SDK_FILESYSTEM = 1
#BUILD_SDK_RTOS = 1
#BUILD_SDK_NETWORK = 1

# Set to 0 to disable compile errors when there is a compiler warning
# and set to 1 to enable compiler errors when there is a compiler warning
ERROR_ON_WARNING = 1

# features of this product -- these macros are required
PROCESSOR_ARM = 1
CPU_LITTLE_ENDIAN = 1

#the spcific type of the processor; typically used to determine
# compiler flags.
# legal values:
#   ARM_PJ4 - granite
export CPU_TYPE = ARM_PJ4

#Specifies which floating-point ABI to use. Permissible values are: `soft', `softfp' and `hard'.
#If unspecified the default is 'soft'.
export FLOAT_ABI = hard

# Optional features
HAVE_DCACHE = 1

# name of oem directory
OEM_DIR = shiloh

#name of product directory; located under OEM_DIR
PRODUCT_DIR = pinetree

# which asic and revision
ASIC = granite
ASIC_REV = c0

# which board signals ie what product are you?

# PROJECT specifies the makefile
# BOARD specifies the mapping from asic to board to mech ie the signal/wiring.
BOARD = pinetree_mech

# allow building both form factor and mech from same board directory since differences
# are minor.  This define is oem specific and should only be used in local oem
# code, not unity common code
#HAVE_FORM_FACTOR_BOARD = 1
ifdef HAVE_FORM_FACTOR_BOARD
MACROS += BOARD_TYPE_FORM_FACTOR
else
MACROS += BOARD_TYPE_MECH
endif

# ini file to be passed to imagebuilder.pl when creating a bootable spi image
#BOOTSPI_INI_FILE = 

# PRINT_MECH specifies the printer mechanism
PRINT_MECH = pinetree
# MECH family and type for mechs that share common implementation
#PRINT_MECH_FAMILY =

# this can't be turned off at the moment
HAVE_PRINT_SUPPORT = 1
HAVE_PRINT_CORE = 1
HAVE_PRINT_HW_PIPE = 1
HAVE_PIP = 1
HAVE_AUTOMATIC_DUPLEXER_SUPPORT = 1

ifdef HAVE_PRINT_HW_PIPE
HAVE_VIDEO_APP = 1
HAVE_VIDEO_DRIVER = 1

HAVE_JBIG= 1
HAVE_ODMA= 1
#HAVE_MIP= 1
#HAVE_TEXT_PAD= 1
endif

# this is currently stubbed out at the fax_svr level
#HAVE_FAX = silabs
ifeq ($(HAVE_FAX),silabs)
  HAVE_JPEG_SW = 1
  HAVE_FREETYPE = 1
  HAVE_GRAY2BW = 1
  HAVE_RGB2LABI = 1
  HAVE_PPM = 1
  HAVE_XYSCALE = 1
  HAVE_DATE_TIME = 1
endif

# generators
HAVE_GENERATORS = 1
#HAVE_GENERATOR_SKELETON = 1
HAVE_GENERATOR_PIP_ADAPTOR = 1
HAVE_GENERATOR_COMPRESS = 1
HAVE_IMAGELOG = 1

#ScanBox
HAVE_SCANBOX = 1

#HAVE_TCL = 1
HAVE_CMD = 1
CMD_INTERP_TYPE=simple
#CMD_INTERP_TYPE=tcl
#HAVE_UFS = 1
#HAVE_DATAFS = 1
#HAVE_GLOSS = 1

# common print engine mech stuff
HAVE_UIO_LIB = 1
HAVE_DEC_SENSOR = 1
HAVE_DEC_ADC = 1
HAVE_DEC_FUSER = 1
HAVE_FUSER_ZC_DRIVER = 1
HAVE_DEC_LASER = 1
HAVE_PWM = 1
#can't have both common and oem device either or.
#HAVE_DCMOTOR_OL = 1
HAVE_DCMOTOR_OL_OEM = 1

ifndef PRINT_MECH
HAVE_ENGINE_STUB = 1
endif


# required core modules
#HAVE_UART = dwapb
HAVE_INIT = 1

# non optional malloc 
HAVE_MEMMGR = 1
HAVE_MLIMITER_CONFIG = 1
HAVE_PLATFORM = 1
HAVE_SDHFAX = 1
#HAVE_RTOS = threadx
#HAVE_POSIX = 1
HAVE_SYSTEM = 1
HAVE_UTILS = 1
#HAVE_CPU = 88sv581x-v7
HAVE_GPIO = mrvl_gpio_linux
HAVE_HWCONFIG = linux_application
#HAVE_INTERRUPT = arm_pl190v32_4sic
HAVE_INCLUDE = 1
HAVE_DEBUG = 1
HAVE_STRFMT = 1
HAVE_LOGGER = 1
HAVE_STATUS = 1
HAVE_LOCATION = 1
HAVE_DATA_ACCESS = 1
HAVE_STRING_MGR = 2
#HAVE_CLOUD=1
HAVE_EFUSE = 1
HAVE_AXI_BUS = 1

#ksjung
HAVE_JSONRPC = 1

HAVE_SCAN_SUPPORT=1
HAVE_COPY_SUPPORT=1

ifdef HAVE_SCAN_SUPPORT
    # Define the desired scan mech. Note that the defined mech will also get a
    # MACROS definition for use in the code, see project.mk (it will look like
    # MACROS += SCAN_MECH_$(SCAN_MECH))
#	SCAN_MECH = TBD
endif

ifdef HAVE_COPY_SUPPORT
  HAVE_GENERATOR_NUP = 1
  HAVE_ID_COPY = 1
  ifeq ($(TYPE), debug)
#  HAVE_COPY_TO_HOST = 1
  endif
endif

#Define for System Service Layer(SSL)
HAVE_SYSTEM_SERVICE_LAYER = 1

#Tiny Debugger
HAVE_TINY_DEBUGGER = 1

HAVE_CARD_READER = 1

HAVE_PAPERCUT =1

# PDF module: Scan Image to PDF Service 
HAVE_HPDF_LIB = 1

# JPEG module: Scan Image to JPEG Service (libjpeg-turbo)
#HAVE_JPEG_TURBO_LIB = 1

# required modules for printing and basic parsing
HAVE_ACL = 1
HAVE_PAPER = 1
HAVE_USB_GADGET = 1
HAVE_DELAY = 1
#HAVE_TIMER = apbtimer
#HAVE_TIMER = linux_timer


# ??? not in all serenity builds
HAVE_NVRAM = 1
HAVE_NVRAM_FILE = 1
HAVE_HIPS_PLL = 1

#ifdef HAVE_NVRAM_FILE
#ifndef HAVE_NVRAM
HAVE_NVRAM = 1
#endif
#endif

#tcl needs rtc to compile ??? Is this true?
#HAVE_TCL = 1

HAVE_AIRPRINT = 1
ifdef HAVE_AIRPRINT
HAVE_URF = 1
endif

ifdef HAVE_URF
   ifdef HAVE_AIRPRINT
     HAVE_LASER_HW_STRIP_COLOR_PIPE = 1
     HAVE_PARSER_UNI10=1
     HAVE_PARSER_CUPS20=1
     HAVE_PARSER_CUPS30=1
#     HAVE_PARSER_JPEG = 1
   else
     HAVE_LASER_SW_COLOR_PIPE = 1
   endif
endif

#HAVE_ZLIB = 1
#HAVE_ZLIB_APPS = 1
HAVE_PJL = 1

# thirdparty PDL's from GlobalGraphic requires license.
# either fontfusion or ufst fonts 
HAVE_GGSRIP = 1
#HAVE_GGSRIP_FREETYPE =1
HAVE_GGSRIP_FONTFUSION =1
#HAVE_GGSRIP_UFST = 1

HAVE_ZJ = 1
HAVE_XML = 1
#ifndef HAVE_SERENITY
# not supported on Serenity, Serenity has built-in phy
#HAVE_USB_PHY = marvell_phy_utmi_1_7
#endif
# HAVE_USB_HOST_PRINTER_CLASS = 1
# HAVE_USB_HOST = 1
# HAVE_USB_MASS_STORAGE = 1
ifndef HAVE_GGSRIP
#HAVE_USB_HOST_PARSER = 1 #Adding to convert from tx to posix
endif
#HAVE_STATISTICS = 1 #currently required to print, but should not be once we have end of page and job notification working
#HAVE_BOOTSPI = apbbootspi
#HAVE_I2C = dw_apb_i2c_v1_08b
#HAVE_EEPROM = 1

#ifdef HAVE_FORM_FACTOR_BOARD
# HAVE_SPI = dw_apb_ssi_v3_10a
#endif

#HAVE_CDMA = mrvl_cdma_r03_00

# HAVE_NAND_PLUS = 1
HAVE_INTERNAL_PAGE_MGR = 1

# consumables oids
HAVE_COMMON_CONSUMABLES_API = 1
HAVE_OEM_CONSUMABLES_API = $(PRODUCT_DIR)
HAVE_COMMON_CONSUMABLES_STUB = 1
HAVE_SM_JOB = 1

HAVE_OID = 1
HAVE_SCAN=1

# HAVE_POWER_MGR = 1
# If ASIC has unique power save driver, set it appropriately
#HAVE_ASIC_POWER_DRIVER = 1

# HAVE_SD = 1
HAVE_JPEG_SW = 1

ifeq ($(HAVE_KINOMA),1)

  unexport HAVE_EMAIL
  
  export KINOMA_MANIFEST_FILE = manifest_mfp_demo_kpr

  # The path in BUILD_KINOMA_FILE_SYSTEM is relative to $(OEM_F_HOME)/kinoma/
  BUILD_KINOMA_FILE_SYSTEM = scripts/create_kinomafs

  # INITIALIZE_KINOMA_FILE_SYSTEM = 1
  
  #ifneq ( $(HAVE_EMAIL), 0 )
  #  HAVE_EMAIL = 1
  #endif

  ifeq ( $(HAVE_EMAIL), 1 )
    HAVE_JPG2PDF = 1
    HAVE_EMAIL_APPS = 1
    HAVE_RTC = swiss
    HAVE_CURL = 1
    HAVE_SSL = 1
  endif # HAVE_EMAIL == 1

  # enable fonts
  #HAVE_UFST_FONT_SUPPORT = 1
  HAVE_FREETYPE = 1

  HAVE_FRAME_BUFFER = arm_pl110
  HAVE_DISPLAY = giantplus_4_3_1198a0
  HAVE_CONTROL_PANEL = diamond_touch_enh

  # Set the Kinoma thread stack size (Kilobytes)
  # Default is 64 (Kilobytes)
  export KINOMA_THREAD_STACK_SIZE = 128

  HAVE_NAND = 1
  HAVE_NAND_NV = 1

  # Set the type of NAND partition.
  NAND_PARTITIONS = SI_Kinoma
  
  # Set the NAND size.
  NAND_SIZE = 1Gb_part

  SMARTINSTALL_DISABLE = 1
  HAVE_USB_MASS_STORAGE_DEVICE = 1
  HAVE_MEDIA_MANAGER = 1

  HAVE_FILESYSTEM = blunk
  RAM_PARTITION_NAME = r
  FS_VERSION = 20100
  
endif

HAVE_OS_SHIM = 1
# which os shims do we need?
#HAVE_OS_SHIM_VFS = 1
#HAVE_OS_SHIM_DIRENT = 1
#HAVE_OS_SHIM_FTRUNCATE = 1
#HAVE_OS_SHIM_UTIME = 1


HAVE_NETWORK = 1 
ifdef HAVE_NETWORK
	HAVE_IPV6 = 1
    HAVE_USB_WLAN = 1
    ifdef HAVE_USB_HOST
	# uncomment to include WLAN
	# HAVE_USB_WLAN = 1
	ifdef HAVE_USB_WLAN
		HAVE_WIRELESS = 1
		WIRELESS_MODULE_TYPE = 878x
        endif
    endif
    # uncomment to include Ethernet
    HAVE_ETHERNET=1
    HAVE_SMTP=1
    # uncomment for Internet FAX Support
    #HAVE_IFAX=1;
    ifdef HAVE_IFAX
        HAVE_POP=1
        HAVE_IMAP=1
    endif
    # uncomment for POP3 Support
    #HAVE_POP=1
    # uncomment for IMAP Support
    #HAVE_IMAP=1    
    # uncomment to include Wifi interface and links
    HAVE_WIRELESS = 1
    ifdef HAVE_WIRELESS
        WIRELESS_MODULE_TYPE = 878x

        # wlan synonym for mlan/wifi
        HAVE_WLAN = 1
        HAVE_UAP = 1
        #uncomment for WifiSetter
        #HAVE_WIFI_SETTER = 1
    endif


    #uncomment to include NETIO
    HAVE_NETIO=1
    ifdef HAVE_NETIO
        #uncomment to include NET_RAW_IO
        HAVE_NET_RAW_IO=1
    endif
    ifdef HAVE_NET_RAW_IO
        #uncomment to include NETPRINT
        HAVE_NETPRINT=1
        #uncomment to include LPD
        HAVE_NET_LPD=1
        ifdef HAVE_SCAN_SUPPORT
            #uncomment to include NET_SCAN
            HAVE_NET_SCAN=1
        endif
    endif
    # uncomment to include SNMP
    # HAVE_SNMP=1
    # uncomment to use (3rd-party) net-snmp (service + agentx)
    HAVE_SNMP_SUBAGENT=net-snmp
    # uncomment to include Network Services (mdns, snmp, etc..)
    # HAVE_NET_SERVICES = yes
    # uncomment to include SNTP
    # HAVE_SNTP=1
    #uncomment to include ECHO
    # HAVE_NET_ECHO=1
    #uncomment to include HTTP
    HAVE_HTTP=1
    #uncomment to include HTTP EWS services
    # HAVE_EWS_HTTP=1
    #uncomment to include NBNS
    # HAVE_NBNS=1           
    #uncomment to include SLP
    # HAVE_SLP=1
    #uncomment to include MDNS
    HAVE_MDNS=1
    #uncomment to include TFTP - requires a Treck stack with licensed TFTP option
    # HAVE_TFTP=1  
    ifeq ($(TYPE), debug)
        #uncomment to include TELNET
        # HAVE_TELNET=1
    endif
    # Uncomment to include BOOTP
    # HAVE_BOOTP=1
    # Uncomment to include SSL
    HAVE_SSL = 1
    # Uncomment to include DNS
    # HAVE_DNS = 1
    # Uncomment to include LLTD - requires MS Rally license
    # HAVE_LLTD = 1
    # Uncomment to include raw sockets
    # HAVE_RAW_SOCKET = 1
    # Uncomment to include TTCP
    ifeq ($(TYPE), debug)
	# HAVE_TTCP = 1
    endif

    #HAVE_WSD=1
    #HAVE_WSD_PRINT=1
    #HAVE_WSD_SCAN=1
     # airprint stuff
    ifdef HAVE_AIRPRINT
      HAVE_IPP=1
#      HAVE_SM_JOB = 1
#      HAVE_CURL = 1
#      HAVE_SSL = 1
      # The SSL version to use.
#      SSL_VERSION = openssl-1.0.0g
#      REMOVE_HAVE_IPV6 = 1
    endif

    # Uncomment to include CIFS
    # HAVE_CIFS = 1

   ifdef HAVE_CLOUD
	HAVE_XML =1
	HAVE_SSL =1
	HAVE_XMPP =1
	HAVE_CURL =1
	HAVE_GCPP =1
    endif

    ifdef HAVE_GCPP
    HAVE_JSON=1
    endif
endif

ifeq ($(TYPE), debug)
# Optional debug features 
HAVE_MEMORY_LOG = 1		
HAVE_UNIT_TEST = 1
HAVE_DBG_PNM = 1
#HAVE_DEBUG_STACK_SCAN = 1
#HAVE_DBG_PRINTF = 1
HAVE_SINDOH_LOG_SYSTEM = 1
endif

## END BIND SECTION
## 
#########################################################################################
