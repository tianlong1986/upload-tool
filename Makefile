all:upload

SOURCE=confile.h \
	confile.c \
	get_hwinfo.h \
	get_hwinfo.c \
	function_common.h \
	function_common.c \
	get_longrun.h \
	get_longrun.c \
	get_network.h \
	get_network.c \
	main.c 

CFLAGS=-g -I/usr/include/mysql -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4  -m32 -fasynchronous-unwind-tables -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv -fPIC  -fPIC -g -static-libgcc -fno-omit-frame-pointer -fno-strict-aliasing -DMY_PTHREAD_FASTMUTEX=1 -pthread -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12
#LIBS=`pkg-config --libs gtk+-2.0` -L ./libs/*
#CFLAGS=`mysql_config --cflags` `pkg-config --cflags gtk+-2.0`
LIBS=-pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -L ./libs/*

upload: $(SOURCE)
	gcc -o upload $(SOURCE) $(CFLAGS) $(LIBS)

clean:
	rm -f *.o upload
