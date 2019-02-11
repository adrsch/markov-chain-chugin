MAIN = test.cpp
CC = g++
CFLAGS = -std=c++14

%.o: %.cpp
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<

lmarkovgenerator: markovgenerator.o
	$(CC) -shared $(CFLAGS) -o libmarkovgenerator.so $<

markovgenerator: lmarkovgenerator
	$(CC) $(CFLAGS) -o t $(MAIN) -L. -Wl,-rpath,. -$<
