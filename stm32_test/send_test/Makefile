# name of your application
APPLICATION = Test

# If no BOARD is found in the environment, use this default:
BOARD ?= stm32f0discovery

DEVELHELP ?= 1

# USEMODULE += gnrc_netdev_default
# USEMODULE += auto_init_gnrc_netif
# USEMODULE += auto_init
USEMODULE += mrf24j40


# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
