include ../Makefile.mk

CFLAGS += -Wall 

CFLAGS += -Wunused-argument \
-Wunused-function \
-Wunused-label \
-Wunused-parameter \
-Wunused-value \
-Wunused-variable \
-Wunused \
-Wextra

CFLAGS += -Wunreachable-code

#CFLAGS += -Wsign-conversion
zzzCFLAGS += \
-Wabi \
-Waddress-of-temporary \
-Waddress \
-Waggregate-return \
-Wall \
-Wambiguous-member-template \
-Warc-abi \
-Warc-non-pod-memaccess \
-Warc-retain-cycles \
-Warc-unsafe-retained-assign \
-Warc \
-Watomic-properties \
-Wattributes \
-Wavailability \
-Wbad-function-cast \
-Wbind-to-temporary-copy \
-Wbitwise-op-parentheses \
-Wbool-conversions \
-Wbuiltin-macro-redefined \
-Wc++-compat \
-Wc++0x-compat \
-Wc++0x-extensions \
-Wcast-align \
-Wcast-qual \
-Wchar-align \
-Wchar-subscripts \
-Wcomment \
-Wcomments \
-Wconditional-uninitialized \
-Wconversion \
-Wctor-dtor-privacy \
-Wcustom-atomic-properties \
-Wdeclaration-after-statement \
-Wdelegating-ctor-cycles \
-Wdelete-non-virtual-dtor \
-Wdeprecated-declarations \
-Wdeprecated-implementations \
-Wdeprecated-writable-strings \
-Wdeprecated \
-Wdisabled-optimization \
-Wdiscard-qual \
-Wdiv-by-zero \
-Wduplicate-method-arg \
-Weffc++ \
-Wempty-body \
-Wendif-labels \
-Wexit-time-destructors \
-Wextra-tokens \
-Wextra \
-Wformat-extra-args \
-Wformat-nonliteral \
-Wformat-zero-length \
-Wformat \
-Wformat=2 \
-Wfour-char-constants \
-Wglobal-constructors \
-Wgnu-designator \
-Wgnu \
-Wheader-hygiene \
-Widiomatic-parentheses \
-Wignored-qualifiers \
-Wimplicit-atomic-properties \
-Wimplicit-function-declaration \
-Wimplicit-int \
-Wimplicit \
-Wimport \
-Wincompatible-pointer-types \
-Winit-self \
-Winitializer-overrides \
-Winline \
-Wint-to-pointer-cast \
-Winvalid-offsetof \
-Winvalid-pch \
-Wlarge-by-value-copy \
-Wliteral-range \
-Wlocal-type-template-args \
-Wlogical-op-parentheses \
-Wlong-long \
-Wmain \
-Wmicrosoft \
-Wmismatched-tags \
-Wmissing-braces \
-Wmissing-declarations \
-Wmissing-field-initializers \
-Wmissing-format-attribute \
-Wmissing-include-dirs \
-Wmissing-noreturn \
-Wmost \
-Wmultichar \
-Wnested-externs \
-Wnewline-eof \
-Wnon-gcc \
-Wnon-virtual-dtor \
-Wnonnull \
-Wnonportable-cfstrings \
-Wnull-dereference \
-Wobjc-nonunified-exceptions \
-Wold-style-cast \
-Wold-style-definition \
-Wout-of-line-declaration \
-Woverflow \
-Woverlength-strings \
-Woverloaded-virtual \
-Wpacked \
-Wparentheses \
-Wpointer-arith \
-Wpointer-to-int-cast \
-Wprotocol \
-Wreadonly-setter-attrs \
-Wredundant-decls \
-Wreorder \
-Wreturn-type \
-Wself-assign \
-Wsemicolon-before-method-body \
-Wsequence-point \
-Wshadow \
-Wshorten-64-to-32 \
-Wsign-compare \
-Wsign-promo \
-Wsizeof-array-argument \
-Wstack-protector \
-Wstrict-aliasing \
-Wstrict-overflow \
-Wstrict-prototypes \
-Wstrict-selector-match \
-Wsuper-class-method-mismatch \
-Wswitch-default \
-Wswitch-enum \
-Wswitch \
-Wsynth \
-Wtautological-compare \
-Wtrigraphs \
-Wtype-limits \
-Wundeclared-selector \
-Wuninitialized \
-Wunknown-pragmas \
-Wunnamed-type-template-args \
-Wunneeded-internal-declaration \
-Wunneeded-member-function \
-Wunused-argument \
-Wunused-exception-parameter \
-Wunused-function \
-Wunused-label \
-Wunused-member-function \
-Wunused-parameter \
-Wunused-value \
-Wunused-variable \
-Wunused \
-Wused-but-marked-unused \
-Wvariadic-macros \
-Wvector-conversions \
-Wvla \
-Wvolatile-register-var \
-Wwrite-strings
 

 

#LDFLAGS += -Wl,--cref -Wl,-Map=dps8.map

# Configuration switches

# Enable M[] as shared memory segment
#LDFLAGS += -lrt

LOCALLIBS = ../simhv40-beta/simh.a ../decNumber/decNumber.a 

# simh requires -ldl
# Cygwin seems to need...
ifeq ($(OS),Windows_NT)
    LIBS = $(LOCALLIBS) -lgcc -lm -ldl -lrt
else
    LIBS = $(LOCALLIBS)  -lm -ldl
endif

# efence is Electric Fence, a malloc debugger.
#LIBS +- -lefence

#LIBS = -lrt -lpthread 

C_SRCS  = ./dps8_addrmods.c 
C_SRCS += ./dps8_append.c
C_SRCS += ./dps8_bar.c
C_SRCS += ./dps8_console.c
C_SRCS += ./dps8_clk.c
C_SRCS += ./dps8_cpu.c
C_SRCS += ./dps8_disk.c
C_SRCS += ./dps8_eis.c
C_SRCS += ./dps8_fnp.c
C_SRCS += ./dps8_ins.c
C_SRCS += ./dps8_iom.c
C_SRCS += ./dps8_loader.c
C_SRCS += ./dps8_lp.c
C_SRCS += ./dps8_math.c
C_SRCS += ./dps8_mt.c
C_SRCS += ./dps8_opcodetable.c
C_SRCS += ./dps8_scu.c
C_SRCS += ./dps8_stddev.c
C_SRCS += ./dps8_sys.c
C_SRCS += ./dps8_utils.c
C_SRCS += ./dps8_decimal.c
C_SRCS += ./dps8_faults.c
C_SRCS += ./dps8_iefp.c
C_SRCS += ./dps8_fxe.c
#C_SRCS += ./fnpp.c
C_SRCS += ./shm.c
C_SRCS += ./fnp_ipc.c
C_SRCS += ./dps8_crdrdr.c

OBJS  := $(patsubst %.c,%.o,$(C_SRCS))

H_SRCS  = dps8_addrmods.h
H_SRCS += dps8_append.h
H_SRCS += dps8_bar.h
H_SRCS += dps8_clk.h
H_SRCS += dps8_console.h
H_SRCS += dps8_cpu.h
H_SRCS += dps8_decimal.h
H_SRCS += dps8_disk.h
H_SRCS += dps8_eis.h
H_SRCS += dps8_faults.h
H_SRCS += dps8_fnp.h
H_SRCS += dps8_fxe.h
H_SRCS += dps8.h
H_SRCS += dps8_hw_consts.h
H_SRCS += dps8_iefp.h
H_SRCS += dps8_ins.h
H_SRCS += dps8_iom.h
H_SRCS += dps8_loader.h
H_SRCS += dps8_math.h
H_SRCS += dps8_mt.h
H_SRCS += dps8_mp.h
H_SRCS += dps8_opcodetable.h
H_SRCS += dps8_scu.h
H_SRCS += dps8_simh.h
H_SRCS += dps8_sys.h
H_SRCS += dps8_utils.h
H_SRCS += utarray.h
H_SRCS += uthash.h
H_SRCS += utlist.h
#H_SRCS += fnpp.h
H_SRCS += dps8_crdrdr.h

H_SRCS += area_info.incl.pl1.h
H_SRCS += bind_map.incl.pl1.h
H_SRCS += definition_dcls.incl.pl1.h
H_SRCS += iocbx.incl.pl1.h
H_SRCS += linkdcl.incl.pl1.h
H_SRCS += object_map.incl.pl1.h
H_SRCS += pl1_symbol_block.incl.pl1.h
H_SRCS += source_map.incl.pl1.h
H_SRCS += stack_header.incl.pl1.h
H_SRCS += std_symbol_header.incl.pl1.h
H_SRCS += symbol_block.incl.pl1.h
H_SRCS += system_link_names.incl.pl1.h
H_SRCS += shm.h



all: locallibs_then_dps8 

android : 
	ndk-build APP_BUILD_SCRIPT=./Android.mk APP_ABI=arm64-v8a APP_PLATFORM=android-21

# # Multipass stuff
# CFLAGS += -DMULTIPASS
LDFLAGS += -lrt -L../lib/ -lzyre -lczmq -lzmq -Wl,-rpath=/usr/local/lib/

blinkenLights : blinkenLights.c shm.o
	$(CC) $(CFLAGS) $(LDFLAGS) blinkenLights.c -o blinkenLights shm.o `pkg-config --cflags --libs gtk+-3.0`

#all : blinkenLights

blinkenLightsCurses : blinkenLightsCurses.c shm.o
	$(CC) $(CFLAGS) $(LDFLAGS) blinkenLightsCurses.c -o blinkenLightsCurses shm.o -lncurses

#all : blinkenLightsCurses

dps8 : tags $(OBJS) $(LOCALLIBS) 
	$(LD)  -o "dps8" $(OBJS) $(LDFLAGS) $(LIBS)
	
slte.inc : system_book_12_3.ascii
	./buildSLTE.sh

locallibs_then_dps8 :
	(cd ../simhv40-beta; $(MAKE))
	(cd ../decNumber; $(MAKE))
	#$(MAKE) zmq
	$(MAKE) dps8

# Other Targets

.PHONY: zeromq czmq zyre 

zmq: zeromq czmq zyre

zeromq : ../lib/libzmq.a

../lib/libzmq.a :
	(cd ../zeromq-4.1.0 && ./configure --prefix=`pwd`/../ && $(MAKE) && $(MAKE) check && $(MAKE) install)
	
czmq : ../lib/libczmq.so

../lib/libczmq.so :
	(cd ../czmq-master-ff0d028b16 && cmake . -DZEROMQ_LIBRARIES=../lib/libzmq.so -DZEROMQ_INCLUDE_DIRS=../include/ -DCMAKE_INSTALL_PREFIX=.. && $(MAKE) && $(MAKE) install)

zyre : ../lib/libzyre.so

../lib/libzyre.so :
	(cd ../zyre-master-9d7e62d2c9 && cmake . -DCZMQ_LIBRARIES=../lib/libczmq.so -DCZMQ_INCLUDE_DIRS=../include -DZEROMQ_LIBRARIES=../lib/libzmq.so -DZEROMQ_INCLUDE_DIRS=../include/ -DCMAKE_INSTALL_PREFIX=.. && $(MAKE) && $(MAKE) install)

# If you do not have ctags in your build environment, change 'ctags' to 
# '#ctags' below.

tags : $(C_SRCS) $(H_SRCS)
	-ctags $(C_SRCS) $(H_SRCS) ../simhv40-beta/*.[ch] ../decNumber/dec*.[ch]

.PHONY : tapes

tapes: 88534.tap 88631.tap 88632.tap 88633.tap 88634.tap 88635.tap 88636.tap 99020.tap 93085.tap 98570.tap 99019.tap 98580_1er.tap 98576_1er.tap 98577.tap 20184.tap 20185.tap 20186.tap 20187.tap 20188.tap t4d_b.2.tap

%.tap:
	wget bitsavers.trailing-edge.com/bits/Honeywell/multics/tape/$@.gz
	gunzip $@.gz

.PHONY : clean cleaner 

clean:
	-$(RM) $(OBJS) $(C_DEPS) $(EXECUTABLES) dps8 tags $(C_SRCS:.c=.d) $(wildcard $(C_SRCS:.c=.d.[0-9]*))

cleaner: clean
	(cd ../simhv40-beta; $(MAKE) clean)
	(cd ../decNumber; $(MAKE) clean)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(C_SRCS:.c=.d)
