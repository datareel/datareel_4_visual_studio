all:
	make -f server.h10 all
	make -f client.h10 all

clean:
	make -f server.h10 clean
	make -f client.h10 clean
