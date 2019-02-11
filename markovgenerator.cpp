#include<iostream>
#include<fstream>
#include<string>

#include"markovgenerator.h"

MarkovGenerator::MarkovGenerator() {
	
}

void MarkovGenerator::loadMidi(std::string midi_file) {
	std::ifstream midi (midi_file,std::ifstream::binary); 
	midi.seekg(9,midi.beg);
	char buffer[8];
	midi.read(buffer, 2);
	std::cout << "format type: " << static_cast<int>(buffer[0]) << std::endl;
}

void MarkovGenerator::loadMidi(std::string midi[], size_t size) {
}

char MarkovGenerator::next(char note) {
	return 0;
}

char MarkovGenerator::stringToMidi(std::string note) {
	return 0;
}

MarkovGenerator::~MarkovGenerator(){}
