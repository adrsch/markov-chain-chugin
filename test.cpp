#include<iostream>
#include"markovgenerator.h"

int main(int argc, char *argv[]) {
	MarkovGenerator test;
	if (argc > 1) {
		test.loadMidi(argv[1]);
	}
	else {
		char name[20];
		std::cout << "enter midi filename: ";
		std::cin >> name;
		std::cout << "opening " << name << std::endl;
		test.loadMidi(name);
	}
	return 0;
}
