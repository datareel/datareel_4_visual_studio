all:
	make -f server.gcc all
	make -f client.gcc all

clean:
	make -f server.gcc clean
	make -f client.gcc clean

