all:
	g++ main.cpp server.cpp -I./ -lleveldb -lpthread -o dkvcpp