#include<iostream>
#include<fstream>
#include<string>

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
	std::ifstream midi (midi_file,std::ifstream::binary); 
	midi.seekg(9,midi.beg);
	char buffer[8];
	midi.read(buffer, 1);
	std::cout << "format type: " << static_cast<int>(buffer[0]) << std::endl;
	midi.seekg(10, midi.beg);
	midi.read(buffer, 2);
	int tracks = valueToInt(buffer, 2);
	std::cout << "number of tracks: " << tracks << std::endl;
	midi.seekg(4, midi.beg); //seek to chunk size
	midi.read(buffer, 4); //read the chunk size
	int chunk_size = valueToInt(buffer, 4);
	midi.seekg(8 + chunk_size, midi.beg); //seek to next chunk
	for (int i = 0; i < tracks; i++) {
		midi.seekg(4, midi.cur);
		midi.read(buffer, 4);
		chunk_size = valueToInt(buffer, 4);
		std::cout << "chunk size: " << chunk_size << std::endl;
		midi.seekg(4, midi.cur); //seek to start of track event data
		
		//this while loop processes one event, and stops if the chunk size is reached
		int pos = 0;
		int lastNote;
		while (pos < chunk_size) {
			bool in_delta_time = true; //as this is variable-length, it is necessary to loop over the data to figure out when it ends and the rest of the event data begins
			for (pos; in_delta_time; pos++) {
				midi.read(buffer, 1);
				midi.seekg(1, midi.cur);
				in_delta_time &= buffer[0];
			}
			in_delta_time = false;
			midi.read(buffer,3);
			std::cout << std::hex << static_cast<int>(buffer[1] & 0xF) <<std::endl;
			//if it is a note-on
			if (buffer[0] & 0xF == 0x9) {
				lastNote = buffer[1];
				std::cout << "note on " << lastNote << std::endl;
				midi.seekg(3, midi.cur);
				pos += 3;
			}
			else if (buffer[0] == 0xFF) {
				std::cout << "meta event (unimplemented, may ruin parsing)" << std::endl;
				midi.seekg(3, midi.cur);
				pos += 3;
			}


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
