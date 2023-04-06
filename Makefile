all: read server client

read: readChunk.cpp
	g++ -o read readChunk.cpp

server: server.cpp
	g++ -o server server.cpp

client: client.cpp
	g++ -o client client.cpp

	