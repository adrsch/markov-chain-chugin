#include<iostream>
#include<fstream>
#include<string>
#include"./midifile/MidiFile.h"

#include"markovgenerator.h"

//Update the ceiling for RNG for the last note played
void MarkovGenerator::updateCeiling() {
	random_ceiling = 0;
	for (int i = 0; i < 12; i++) {
		std::cout << probabilities[last_note][i] << " ";
		if (probabilities[last_note][i] > random_ceiling) {
			random_ceiling = probabilities[last_note][i];
		}
		std::cout << std::endl;
	}
	std::cout << random_ceiling << std::endl;
}

void MarkovGenerator::printMatrix() {
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			std::cout << probabilities[i][j] << " ";
		}
		std::cout << std::endl;
	}
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
	int last = 0;
	int next = 0;
	for (int i = 0; i < midifile[track].size(); i++) {
		if (midifile[track][i].isNoteOn()) {
			next = midifile[track][i][1] % 12;
			probabilities[last][next] += 1;
			last = next;
		}
	}
	printMatrix();
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
