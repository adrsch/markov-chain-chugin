#include<iostream>
#include<fstream>
#include<string>
#include<random>
#include"./midifile/MidiFile.h"

#include"markovgenerator.h"

//Get the ceiling for RNG for a note
int MarkovGenerator::getCeiling(char note) {
	int random_ceiling = 0;
	for (int i = 0; i < 12; i++) {
		random_ceiling += probabilities[note%12][i];
	}
	return random_ceiling;
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
	last_note = note % 12;
	return next();
}

//iffy thing: if you end up in a situation where there's no notes in the table to jump to from the last, it'll always send you to C. 
char MarkovGenerator::next() {
	std::uniform_int_distribution<std::mt19937::result_type> gen(0, getCeiling(last_note)); //generator for a number from 0 to the sum of the entries for a note in the table
	int rng_result = gen(rng);
	//next, to find what we got and return it
	char i = 0;
	for (int total = 0; total < rng_result; i++) {
		total += probabilities[last_note][i];
	}
	last_note = i;
	return last_note + (12 * octave);
}

void MarkovGenerator::setLast(char note) {
	last_note = note;
}

//TODO: convert note names ie C C# to 0-12, perhaps also C3 to 3rd octave C
char MarkovGenerator::stringToMidi(std::string note) {
	return 0;
}

int MarkovGenerator::getSeed() {
	return seed;
}

void MarkovGenerator::setSeed(int new_seed) {
	seed = new_seed;
	rng.seed(seed);
}

MarkovGenerator::~MarkovGenerator() { }
