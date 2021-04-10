all:
	g++ -std=c++17 main.cpp config.cpp db.cpp -I./ -lleveldb -lpthread -o dkvcpp

shard:
	g++ -std=c++17 shard.cpp db.cpp -I./ -lleveldb -lpthread -o shard