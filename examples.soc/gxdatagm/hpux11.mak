all:
	make -f server.h11
	make -f client.h11

clean:
	make -f server.h11 clean
	make -f client.h11 clean
