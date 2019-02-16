MAIN = test.cpp
DEPENDENCIES = $(wildcard midifile/*.cpp)
OBJ = $(DEPENDENCIES:.cpp=.o)
CC = g++
CFLAGS = -std=c++14

%.o: %.cpp
	$(CC) -c $(CFLAGS) -fPIC -o $@ $< 

midifile/%.o: midifile/%.cpp
	$(CC) -c $(CFLAGS) -fPIC -o $@ $< 

lmarkovgenerator: markovgenerator.o $(OBJ)
	$(CC) -shared $(CFLAGS) -o libmarkovgenerator.so $^

markovgenerator: lmarkovgenerator
	$(CC) $(CFLAGS) -o t $(MAIN) -L. -Wl,-rpath,. -$< 
