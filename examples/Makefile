CC = kos32-gcc
LD = kos32-ld

SDK_DIR = $(abspath ../../..)

CFLAGS = -c -fno-ident -O2 -fomit-frame-pointer -fno-ident -U__WIN32__ -U_Win32 -U_WIN32 -U__MINGW32__ -UWIN32 
LDFLAGS = -static -S -nostdlib -T $(SDK_DIR)/sources/newlib/app.lds --image-base 0

INCLUDES = -I $(SDK_DIR)/sources/newlib/libc/include -I ../include -I ../INCLUDE_MENUET/include
LIBPATH = -L $(SDK_DIR)/lib -L /home/autobuild/tools/win32/mingw32/lib -L ../lib

# All .c files
SRC = $(notdir $(wildcard *.c))

OBJECTS = $(patsubst %.c, %.o, $(SRC))

default: $(patsubst %.c,%.o,$(SRC))
	kos32-ld $(LDFLAGS) $(LIBPATH) --subsystem native -o firework $(OBJECTS) -lmcurses -lsys -lgcc -lc.dll 
	objcopy firework -O binary

%.o : %.c Makefile $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

clean:
	rm *.o
