#include<iostream>
#include<fstream>
#include<string>
#include"./midifile/MidiFile.h"

#include"markovgenerator.h"

MarkovGenerator::MarkovGenerator() {
	
}

//big endian, constant length format. currently, this conversion means it only supports little-endian machines - could add detection later
unsigned int MarkovGenerator::valueToInt(char c[], int size) {
	unsigned int value = 0; 
	for (int i = 0; i < size; i++) {
		value |= c[size - i - 1] << (8 * i);
	}
	return value;
}

void MarkovGenerator::loadMidi(std::string midi_file) {
	smf::MidiFile midifile;
	midifile.read(midi_file);
	if (!midifile.status()) {
		std::cout << "bad file name" << std::endl;
		return;
	}
	midifile.joinTracks();
	int track = 0;
	int lastNote = 0;
	for (int i = 0; i < midifile[track].size(); i++) {
		if (midifile[track][i].isNoteOn()) {
			lastNote = midifile[track][i][1] % 12;
			std::cout<<lastNote << "\t";
		}
	}
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
