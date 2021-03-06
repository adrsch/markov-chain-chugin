//-----------------------------------------------------------------------------
// Markov Chain Note Generation ChuGin
// See readme.txt along with test.ck and test-audio.ck to learn how to use it!
//-----------------------------------------------------------------------------
// this should align with the correct versions of these ChucK files
#include "chuck_dl.h"
#include "chuck_def.h"

// this is needed for this chugin - see readme.txt for licensing stuff
#include "./midifile/MidiFile.h"

// general includes
#include <stdio.h>
#include <limits.h>

#include <random>
#include <string>
#include <fstream>
#include <iostream>

// declaration of chugin constructor
CK_DLL_CTOR(MarkovGenerator_ctor);
// declaration of chugin desctructor
CK_DLL_DTOR(MarkovGenerator_dtor);

// member functions
CK_DLL_MFUN(MarkovGenerator_loadMidi);
CK_DLL_MFUN(MarkovGenerator_next);
CK_DLL_MFUN(MarkovGenerator_next_arg);
CK_DLL_MFUN(MarkovGenerator_setLast);
CK_DLL_MFUN(MarkovGenerator_printMatrix);
CK_DLL_MFUN(MarkovGenerator_setSeed);
CK_DLL_MFUN(MarkovGenerator_getSeed);
CK_DLL_MFUN(MarkovGenerator_setOrder);
CK_DLL_MFUN(MarkovGenerator_getOrder);
CK_DLL_MFUN(MarkovGenerator_setTonic);
CK_DLL_MFUN(MarkovGenerator_getTonic);

// this is a special offset reserved for Chugin internal data
t_CKINT MarkovGenerator_data_offset = 0;


// class definition for internal Chugin data
// (note: this isn't strictly necessary, but serves as example
// of one recommended approach)
//
class MarkovGenerator
{
public:

	MarkovGenerator(t_CKINT note = 0) : empty(true), tonic(0), order(0), seq(), probabilities1(),probabilities2(), probabilities3(), last_note(note%12), octave(note/12), seed(std::random_device{}()) {}

	// this function adds the data from a midi file to the table for generation
	void loadMidi(std::string midi_file = "test.mid") 
	{
		smf::MidiFile midifile;
		midifile.read(midi_file);
		
		if (!midifile.status()) { throw std::invalid_argument("Bad filename."); }
		midifile.joinTracks();
		int track = 0;
		if (midifile[track].size() == 0) { throw std::invalid_argument("No notes are present in the file. Could not load anything from it."); }
		empty = false;
		int last[4] = {};
		int i = 0;
		int left_to_start = 4;
		for (i; left_to_start > 0 && i < midifile[track].size(); i++) {
			if (midifile[track][i].isNoteOn()) {
				switch (left_to_start) {
					case 4:
						last[3] = midifile[track][i][1] % 12;
						break;
					case 3:
						last[2] = midifile[track][i][1] % 12;
						probabilities1[last[3]][last[2]] += 1;
						break;
					case 2:
						last[1] = midifile[track][i][1] % 12;
						probabilities2[last[3]][last[2]][last[1]] += 1;
						break;
					case 1:
						last[0] = midifile[track][i][1] % 12;
						probabilities3[last[3]][last[2]][last[1]][last[0]] += 1;
						break;
				}
				left_to_start--;
			}
		}

		for (i; i < midifile[track].size(); i++) {
			if (midifile[track][i].isNoteOn()) {
				last[0] = midifile[track][i][1] % 12;
				probabilities1[last[1]][last[0]] += 1;
				probabilities2[last[2]][last[1]][last[0]] += 1;
				probabilities3[last[3]][last[2]][last[1]][last[0]] += 1;
				last[3] = last[2];
				last[2] = last[1];
				last[1] = last[0];
			}
		}
	}

	// generates the next note using the table and the current order
	t_CKINT next()
	{
		if (empty) { throw std::logic_error("Please load a midi file in using loadMidi(\"filename.mid\" before trying to generate notes!"); }

		if (!seq.empty()) {	
			last_note = seq.back();
			seq.pop_back();
		}
		else {	
			int cur_order = order;
			if (!order) {
				std::uniform_int_distribution<std::mt19937::result_type> gen(1, 3);
				cur_order = gen(rng);
			}
			int rng_ceiling = getCeiling(last_note);
			if (rng_ceiling == 0) { throw std::logic_error("Dead end reached!"); }
			std::uniform_int_distribution<std::mt19937::result_type> gen(1, rng_ceiling);
			int rng_result = gen(rng);
			switch (cur_order) {
				case 1:	
				{
					int i = -1;
					int total = 0;
					while (total < rng_result) { 
						i++;
						total += probabilities1[last_note][i]; 
					}
					last_note = i;
					break;
				}
				case 2:
				{
					int i = -1;
					int total = 0;
					while (total < rng_result) { 
						i++;
						total += probabilities2[last_note][i/12][i%12]; 
					}
					seq.push_back(i%12);
					last_note = i/12;
					break;
				}
				case 3:
				{
					int i = -1;
					int total = 0;
					while (total < rng_result) { 
						i++;
						total += probabilities3[last_note][i/144][(i%144)/12][i%12]; 
					}
					seq.push_back((i%144)/12);
					seq.push_back(i%12);
					last_note = i/144;
					break;
				}
			}
		}
		if (last_note < tonic)
			return last_note%12 + (13 * octave);
		else
			return last_note%12 + (12 * octave);

	}

	// return the current order
	t_CKINT getOrder() { return order; }

	// set a new order
	t_CKINT setOrder(t_CKINT new_order)
	{
		if (new_order < 0 || new_order > 3)
			throw std::invalid_argument("Order not valid! Orders 1-3 are supported. Use 0 to pick one randomly each time new notes are generated.");
		order = new_order;
		return new_order;
	}

	// generate the next note, but set a new note to start at - sequences that have been generated in the past if the order used does so will not be used
	t_CKINT next(t_CKINT cur_note)
	{
		setLast(cur_note);
		return next();
	}

	// get the seed used for generation
	t_CKINT getSeed() { return seed; }
	
	// set a new seed for generation
	t_CKINT setSeed(t_CKINT new_seed)
	{
		seed = new_seed;
		rng.seed(seed);
		return new_seed;
	}

	// set the note to use as the note to calculate the next note from
	t_CKINT setLast(t_CKINT note) 
	{
		if (note < 0)
			throw std::invalid_argument("A negative note was used. Please use a positive note value.");
		octave = note / 12;
		last_note = note % 12;
		seq.clear();
		return note;
	}

	// send the matrix entires to std::cout
	void printMatrix() 
	{
		int entry;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 12; j++) {
				entry = probabilities1[i][j];
				if (entry < 9)
					std::cout << entry << "   ";
				else if (entry < 99)
					std::cout << entry << "  ";
				else if (entry < 999)
					std::cout << entry << " ";
				else
					std::cout << entry;
			}
			std::cout << std::endl;
		}
	}

	// get the tonic used to calculate which octave the next note should be in
	t_CKINT getTonic() { return tonic; }

	// set a new tonic
	t_CKINT setTonic(int new_tonic)
	{
		tonic = new_tonic % 12;
		return tonic;
	}
	
private:
	
	// this identicates whether the tables have anything in them
	bool empty;

	// this determines which octave a generated note should be in: the octave the generator produces is from tonic to tonic, ie C0-C0 if the tonic is C
	t_CKINT tonic;

	// this is the seed used for the generator
	t_CKINT seed;
	
	// the random number generator itself
	std::mt19937 rng;

	// the octave that generated notes should be in
	t_CKINT octave;

	// probability tables for Markov chains - 3 of them, as up to order 3 is supported
	int probabilities1[12][12];
	int probabilities2[12][12][12];
	int probabilities3[12][12][12][12];

	// a sequence of notes that are predetermined to come next, as higher order generation produces multiple notes at once but next() only returns the next single note
	std::vector<int> seq;

	// this is the order used for generation. 0 will randomly pick.
	int order;

	// the note that is used as the note to start from for table lookup
	t_CKINT last_note;

	// this gets the ceiling for RNG to produce the next note
	int getCeiling(int note) 
	{
		int random_ceiling = 0;
			for (int i = 0; i < 12; i++) 
				random_ceiling += probabilities1[note%12][i];
		return random_ceiling;
	}
	
};


// query function: chuck calls this when loading the Chugin
CK_DLL_QUERY( MarkovGenerator )
{
	// hmm, don't change this...
	QUERY->setname(QUERY, "MarkovGenerator");
	
	// begin the class definition
	// can change the second argument to extend a different ChucK class
	QUERY->begin_class(QUERY, "MarkovGenerator", "UGen");

	// register the constructor (probably no need to change)
	QUERY->add_ctor(QUERY, MarkovGenerator_ctor);
	// register the destructor (probably no need to change)
	QUERY->add_dtor(QUERY, MarkovGenerator_dtor);
	
	
	// NOTE: if this is to be a UGen with more than 1 channel, 
	// e.g., a multichannel UGen -- will need to use add_ugen_funcf()
	// and declare a tickf function using CK_DLL_TICKF

	QUERY->add_mfun(QUERY, MarkovGenerator_loadMidi, "void", "loadMidi");
	QUERY->add_arg(QUERY, "string", "filename");

	QUERY->add_mfun(QUERY, MarkovGenerator_printMatrix, "void", "printMatrix");

	QUERY->add_mfun(QUERY, MarkovGenerator_next, "int", "next");

	QUERY->add_mfun(QUERY, MarkovGenerator_next_arg, "int", "next");
	QUERY->add_arg(QUERY, "int", "note");

	QUERY->add_mfun(QUERY, MarkovGenerator_setLast, "int", "last");
	QUERY->add_arg(QUERY, "int", "note");

	QUERY->add_mfun(QUERY, MarkovGenerator_setSeed, "int", "seed");
	QUERY->add_arg(QUERY, "int", "seed");

	QUERY->add_mfun(QUERY, MarkovGenerator_getSeed, "int", "seed");

	QUERY->add_mfun(QUERY, MarkovGenerator_setOrder, "int", "order");
	QUERY->add_arg(QUERY, "int", "order");

	QUERY->add_mfun(QUERY, MarkovGenerator_getOrder, "int", "order");

	QUERY->add_mfun(QUERY, MarkovGenerator_setTonic, "int", "tonic");
	QUERY->add_arg(QUERY, "int", "tonic");

	QUERY->add_mfun(QUERY, MarkovGenerator_getTonic, "int", "tonic");

	// this reserves a variable in the ChucK internal class to store 
	// referene to the c++ class we defined above
	MarkovGenerator_data_offset = QUERY->add_mvar(QUERY, "int", "@m_data", false);

	// end the class definition
	// IMPORTANT: this MUST be called!
	QUERY->end_class(QUERY);

	// wasn't that a breeze?
	return TRUE;
}


// implementation for the constructor
CK_DLL_CTOR(MarkovGenerator_ctor)
{
	// get the offset where we'll store our internal c++ class pointer
	OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset) = 0;

	MarkovGenerator * m_obj;	
	// instantiate our internal c++ class representation
	try { m_obj = new MarkovGenerator(); }
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Could not allocate memory! This is not good! No MarkovGenerator was created." << std::endl;
		return;
	}
		
	// set the seed
	m_obj -> setSeed(m_obj -> getSeed());
	
	// store the pointer in the ChucK object member
	OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset) = (t_CKINT) m_obj;
}


// implementation for the destructor
CK_DLL_DTOR(MarkovGenerator_dtor)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// check it
	if( m_obj )
	{
		// clean up
		delete m_obj;
		OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset) = 0;
		m_obj = NULL;
	}
}

CK_DLL_MFUN(MarkovGenerator_loadMidi)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	m_obj->loadMidi(GET_NEXT_STRING_SAFE(ARGS));
}

CK_DLL_MFUN(MarkovGenerator_printMatrix)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	m_obj->printMatrix();
}

CK_DLL_MFUN(MarkovGenerator_setLast)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	try { RETURN->v_int = m_obj->setLast(GET_NEXT_INT(ARGS)); }
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		RETURN->v_int = GET_NEXT_INT(ARGS);
	}
}

CK_DLL_MFUN(MarkovGenerator_setOrder)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	try { RETURN->v_int = m_obj->setOrder(GET_NEXT_INT(ARGS)); }
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		RETURN->v_int = m_obj->getOrder();
	}
}

CK_DLL_MFUN(MarkovGenerator_setSeed)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->setSeed(GET_NEXT_INT(ARGS));
}

CK_DLL_MFUN(MarkovGenerator_next)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->next();
}

CK_DLL_MFUN(MarkovGenerator_next_arg)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->next(GET_NEXT_INT(ARGS));
}

CK_DLL_MFUN(MarkovGenerator_setTonic)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->setTonic(GET_NEXT_INT(ARGS));
}


CK_DLL_MFUN(MarkovGenerator_getTonic)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->getTonic();
}


CK_DLL_MFUN(MarkovGenerator_getOrder)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->getOrder();
}


CK_DLL_MFUN(MarkovGenerator_getSeed)
{
	// get our c++ class pointer
	MarkovGenerator * m_obj = (MarkovGenerator *) OBJ_MEMBER_INT(SELF, MarkovGenerator_data_offset);
	// set the return value
	RETURN->v_int = m_obj->getSeed();
}
