all:
	g++ -std=c++17 main.cpp config.cpp db.cpp server.cpp -I./ -lleveldb -lpthread -o dkvcpp