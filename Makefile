ifaddr.so:
	gcc -Wall `pkg-config lxpanel gtk+-2.0 --cflags` -shared -fPIC ifaddr.c -o ifaddr.so `pkg-config --libs lxpanel gtk+-2.0`
clean:
	rm -rf *.so
all: ifaddr.so

install: all
	cp ifaddr.so /usr/lib/lxpanel/plugins/
uninstall:
	rm -v /usr/lib/lxpanel/plugins/ifaddr.so