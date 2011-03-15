all: tcp udp

tcp: tcp-client tcp-server
udp: udp-client udp-server

tcp-client: tcp-client.cpp
	g++ tcp-client.cpp -o tcp-client

tcp-server: tcp-server.cpp
	g++ tcp-server.cpp -o tcp-server

udp-client: udp-client.cpp
	g++ udp-client.cpp -o udp-client

udp-server: udp-server.cpp
	g++ udp-server.cpp -o udp-server

clean:
	rm -f *.o *.bin tcp-client tcp-server udp-client udp-server

