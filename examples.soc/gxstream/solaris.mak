all:
	make -f server.sol all
	make -f client.sol all

clean:
	make -f server.sol clean
	make -f client.sol clean
