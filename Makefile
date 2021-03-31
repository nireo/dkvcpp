all:
	g++ main.cpp config.cpp db.cpp server.cpp -I./ -lleveldb -lpthread -o dkvcpp