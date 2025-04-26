AVAILABLE_TARGETS = qcx212_0h00
TOOLCHAIN = GCC

TOP := ../../

PROJECT_NAME =      Driver_Example
BINNAME           = HTNB32L-XXX-Driver_Example

BUILD_DRIVER_EXAMPLE = y
BUILD_DRIVER_LIB = y
BUILD_STATIC_LIBRARY = n

BUILD_PS                    = n
BUILD_FW                    = n
#No mater BUILD_USE_PREBUILD_LIB, below is configurable.
DRIVER_DMA_ENABLE           = y
DRIVER_CACHE_ENABLE         = y
DRIVER_PMU_ENABLE           = n
DRIVER_FLASHRT_ENABLE       = y
DRIVER_FLASH_ENABLE         = y

DRIVER_GPR_ENABLE           = y
DRIVER_IC_ENABLE            = y

DRIVER_CIPHER_ENABLE        = y
DRIVER_CIPHER_FLH_ENABLE    = y
DRIVER_EFUSE_ENABLE         = y
DRIVER_CIPHER_TLS_ENABLE    = y

DRIVER_RNG_ENABLE           = n
DRIVER_MCUMODE_ENABLE       = n
DRIVER_ECMAIN_ENABLE        = n
RF_ADI_USED_ENABLE          = n
BUILD_EC_MW                 = n
BUILD_OS                    = n

BUILD_AT                    = n
BUILD_AT_DEBUG              = n
#Choose if below driver need to be compiled or not
DRIVER_USART_ENABLE         = y
DRIVER_SPI_ENABLE           = y
DRIVER_I2C_ENABLE           = y

DRIVER_GPIO_ENABLE          = y
DRIVER_TIMER_ENABLE         = y


DRIVER_UNILOG_ENABLE        = n
DRIVER_WDG_ENABLE           = y
DRIVER_ADC_ENABLE           = y

#Choose if below thirdparty sofware need to be compiled
THIRDPARTY_CISONENET_ENABLE = n
THIRDPARTY_CTWING1_5_ENABLE = n
THIRDPARTY_WAKAAMA_ENABLE   = n
THIRDPARTY_MQTT_ENABLE      = n
THIRDPARTY_LIBCOAP_NO_DTLS_ENABLE   = n
THIRDPARTY_LIBCOAP_DTLS_ENABLE   = n
THIRDPARTY_ERCOAP_ENABLE    = n


THIRDPARTY_HTTPC_ENABLE     = n
THIRDPARTY_LITTEFS_ENABLE   = y
THIRDPARTY_IPERF_ENABLE     = n
THIRDPARTY_CJSON_ENABLE     = n
THIRDPARTY_LWIP_ENABLE      = n
THIRDPARTY_PING_ENABLE      = n
THIRDPARTY_ROHC_ENABLE      = n
THIRDPARTY_MBEDTLS_ENABLE   = n
THIRDPARTY_TINYDTLS_ENABLE  = n
THIRDPARTY_RTT_ENABLE       = n
THIRDPARTY_LIBSNTP_ENABLE   = n
THIRDPARTY_ABUP_FOTA_ENABLE = n
THIRDPARTY_CTCC_DM_ENABLE   = n
THIRDPARTY_CUCC_DM_ENABLE   = n
THIRDPARTY_CMCC_DM_ENABLE   = n
THIRDPARTY_EXAMPLE_ENABLE   = n
#Choose if below ecomm middleware  need to be compiled
MIDDLEWARE_DEBUG_TRACE_ENABLE =n


MIDDLEWARE_CMS_ENABLE       = n
MIDDLEWARE_SIMBIP_ENABLE    = n
MIDDLEWARE_NVRAM_ENABLE     = n
MIDDLEWARE_FEATURE_PRODMODE_ENABLE =n

HT_USART_API_ENABLE := y
HT_SPI_API_ENABLE := y
HT_I2C_API_ENABLE := y

HT_DEFAULT_LINKER_FILE = n



CFLAGS_INC += -I Inc

LINK_FILE_PATH     = ./
PRECINIT_FILE_PATH = Startup/Src
SYSTEM_FILE_PATH   = Startup/Src
STARTUP_FILE_PATH  = Startup/Src

obj-y             += Src/HT_UART_Api.o \
                     Src/HT_GPIO_Demo.o \
                     Src/HT_AON_Demo.o \
                     Src/HT_SPI_Demo.o \
                     Src/HT_I2C_Demo.o \
                     Src/HT_ADC_Demo.o \
                     Src/HT_WDT_Demo.o \
                     Src/HT_USART_Demo.o \
                     Src/stub_os.o \
                     Src/main.o

include $(TOP)/SDK/PLAT/tools/scripts/Makefile.rules
