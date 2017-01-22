PRODUCT= ninepin
CFILES= $(PRODUCT).c cbmdos.c joystick.c d64fs.c localfs.c
OTHER_CFLAGS=
CFLAGS= -g -Wall -I$(HOME)/Unix/$(OSTYPE)/include -dD
OTHER_LIBS=-lwiringPi -lrt -lm

BIN_DIR=.

.SUFFIXES: .m .h,v .c,v .m,v .js .jh

RC_HOSTTYPE= `arch`-`uname`
LIBS=	-L$(HOME)/Unix/$(OSTYPE)/lib -lpthread -ldl

OFILES=$(CFILES:.c=.o) $(MFILES:.m=.o) $(FFILES:.f=.o) $(CLASSES:.m=.o) $(CPPFILES:.cpp=.o)
PRODUCT_DEPENDS= $(OFILES) $(OTHER_OFILES) $(HFILES) $(LIBPATHS)
MAKEDEP=Makefile.depend
MAKEDEPENDS=Makefile.dependencies

ALL_CFLAGS= $(CFLAGS) $(OTHER_CFLAGS)

all::
	@$(MAKE) $(PRODUCT) $(OTHER_TARGETS)

.c.o:
	$(CC) $(ALL_CFLAGS) -c $*.c -o $*.o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(ALL_CFLAGS) -c $*.cpp -o $*.o
.f.o:
	$(FC) $(FFLAGS) $(OTHER_FFLAGS) -c $*.f
.m.o:
	$(CC) $(OCFLAGS) $(ALL_CFLAGS) -c $*.m -o $*.o
.y.o:
	$(YACC) $*.y
	$(CC) $(ALL_CFLAGS) -c y.tab.c
	rm -f y.tab.c
	mv y.tab.o $*.o

.c,v.c:
	co -q $*.c
.m,v.m:
	co -q $*.m
.h,v.h:
	co -q $*.h

$(OFILES) $(PRODUCT_OFILES): $(HFILES)

clean::
	$(RM) $(RMFLAGS) -r $(PRODUCT) $(MAKEDEPENDS) $(MAKEDEP) *~ *_obj *_bin $(OTHER_GARBAGE) *.o

debug::
	@$(MAKE) $(PRODUCT).debug "CFLAGS = -DDEBUG $(CFLAGS)" "DIRNAME = debug_"

shar::
	shar Makefile $(PRODUCT_CFILES) $(CFILES) $(HFILES) $(OTHER_FILES) > part01

$(MAKEDEPENDS):: $(CLASSES) $(MFILES) $(CFILES)
	$(RM) $(RMFLAGS) $(MAKEDEPENDS) 
	$(CC) -MM $(CFLAGS) $(CLASSES) $(MFILES) $(CFILES) >  $(MAKEDEPENDS)

depend:: $(MAKEDEPENDS)

$(PRODUCT): $(PRODUCT_DEPENDS)
	$(CC) $(CFLAGS) $(OTHER_CFLAGS) -o $@ $(OFILES) $(PRODUCT_OFILES) $(OTHER_OFILES) $(LIBS) $(OTHER_LIBS)

$(PRODUCT).debug: $(PRODUCT_DEPENDS)
	$(CC) $(CFLAGS) $(OTHER_CFLAGS) -o $@ $(OFILES) $(PRODUCT_OFILES) $(OTHER_OFILES) $(LIBS) $(OTHER_LIBS)

-include Makefile.dependencies
