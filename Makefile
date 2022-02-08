CC=g++
CFLAGS= -lcurl -ljsoncpp -std=c++17

bot: bot.o Swyftx.o HTTP.o
	$(CC) -o bot bot.o Swyftx.o HTTP.o $(CFLAGS)

bot.o: bot.cpp HTTP.h Exchange.h Swyftx.h 
	$(CC) -c bot.cpp $(CFLAGS)

Swyftx.o: Swyftx.cpp Swyftx.h 
	$(CC) -c Swyftx.cpp $(CFLAGS)

HTTP.o: HTTP.cpp HTTP.h
	$(CC) -c HTTP.cpp

clean:
	$(RM) *.o
