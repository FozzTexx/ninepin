PRODUCT= ninepin
CFILES= $(PRODUCT).c
OTHER_CFLAGS=
CFLAGS= -g -Wall -I$(HOME)/Unix/$(OSTYPE)/include -dD
OTHER_LIBS=-lwiringPi -lrt
MAKEFILEDIR=/usr/local/Makefiles
MAKEFILE=single.make

-include Makefile.preamble

include $(MAKEFILEDIR)/$(MAKEFILE)

-include Makefile.postamble

-include Makefile.dependencies
