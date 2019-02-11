#include"markovgenerator.h"

int main(int argc, char *argv[]) {
	MarkovGenerator test;
	test.loadMidi(argv[1]);
	return 0;
}
