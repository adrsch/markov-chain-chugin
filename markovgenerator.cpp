#include<iostream>
#include<fstream>
#include<string>

#include"markovgenerator.h"

MarkovGenerator::MarkovGenerator() {
	
}

unsigned int MarkovGenerator::wordToInt(char c[]) {
	return (c[0] << 8 | c[1]);
}

void MarkovGenerator::loadMidi(std::string midi_file) {
	std::ifstream midi (midi_file,std::ifstream::binary); 
	midi.seekg(9,midi.beg);
	char buffer[8];
	midi.read(buffer, 1);
	std::cout << "format type: " << static_cast<int>(buffer[0]) << std::endl;
	midi.seekg(10, midi.beg);
	midi.read(buffer, 2);
	int tracks = wordToInt(buffer);
	std::cout << "number of tracks: " << tracks << std::endl;
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
