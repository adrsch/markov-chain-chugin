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
	std::cout << "enter start note: ";
	int note;
	std::cin >> note;
	test.setLast(static_cast<char>(note));
	for (int i = 0; i < 5; i++)
		std::cout << "generated note " << static_cast<int>(test.next()) << std::endl;
	return 0;
}
