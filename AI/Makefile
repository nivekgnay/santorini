CXX = g++
CXXFLAGS = -Wall -Werror -g -std=c++11  

all: minimax alphabeta 

minimax: minimax.o GameState.o
	$(CXX) $(CXXFLAGS) -I ~/Documents/Projects/boost_1_67_0/ -o minimax minimax.o GameState.o -L /usr/local/lib -l boost_serialization

alphabeta: alphabeta.o GameState.o
	$(CXX) $(CXXFLAGS) -I ~/Documents/Projects/boost_1_67_0/ -o alphabeta alphabeta.o GameState.o -L /usr/local/lib -l boost_serialization

minimax.o: minimax.cpp GameState.h
	$(CXX) $(CXXFLAGS) -c minimax.cpp

alphabeta.o: alphabeta.cpp GameState.h
	$(CXX) $(CXXFLAGS) -c alphabeta.cpp

GameState.o: GameState.h
	$(CXX) $(CXXFLAGS) -c Gamestate.cpp

clean:
	rm minimax alphabeta *.o