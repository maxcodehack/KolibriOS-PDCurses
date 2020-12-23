CC = kos32-gcc
LD = kos32-ld

SDK_DIR = $(abspath ../../..)

CFLAGS = -c -fno-ident -O2 -fomit-frame-pointer -fno-ident -U__WIN32__ -U_Win32 -U_WIN32 -U__MINGW32__ -UWIN32 
LDFLAGS = -static -S -nostdlib -T $(SDK_DIR)/sources/newlib/app.lds --image-base 0

INCLUDES = -I $(SDK_DIR)/sources/newlib/libc/include
LIBPATH = -L $(SDK_DIR)/lib -L /home/autobuild/tools/win32/mingw32/lib

PORTABLE_OBJS = portable/addch.o portable/addchstr.o portable/addstr.o portable/attr.o \
	portable/beep.o portable/bkgd.o portable/border.o portable/clear.o \
	portable/color.o portable/delch.o portable/deleteln.o portable/getch.o \
	portable/getstr.o portable/getyx.o portable/inch.o portable/inchstr.o \
	portable/initscr.o portable/inopts.o portable/insch.o portable/insstr.o \
	portable/instr.o portable/kernel.o portable/move.o portable/outopts.o \
	portable/overlay.o portable/pad.o portable/printw.o portable/refresh.o \
	portable/scanw.o portable/scroll.o portable/termattr.o portable/terminfo.o \
	portable/touch.o portable/util.o portable/window.o

MENUETOS_OBJS = menuetos/textcon_if.o menuetos/pdcdebug.o menuetos/pdcdisp.o \
	menuetos/pdcgetsc.o menuetos/pdckbd.o menuetos/pdcprint.o menuetos/pdcscrn.o \
	menuetos/pdcsetsc.o menuetos/pdcutil.o menuetos/pdcwin.o

OBJECTS = $(PORTABLE_OBJS) $(MENUETOS_OBJS)

CFLAGS += -I../include -D__MENUET_CURSES__ -UDOS -I../INCLUDE_MENUET/include -DNO_MEMORY_H

default: $(OBJECTS) Makefile
	ar rcs libmcurses.a $(OBJECTS)
	mkdir ../lib
	mv libmcurses.a ../lib

%.o : %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	rm */*.o
