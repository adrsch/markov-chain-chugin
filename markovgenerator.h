#include<string>
#include<random>

class MarkovGenerator {
	public:
		MarkovGenerator(char note = 0) : probabilities(), last_note(note%12), octave(note/12), seed(std::random_device()()) {}
		MarkovGenerator(std::string midi, char note = 0);
		MarkovGenerator(std::string midi[]);
		~MarkovGenerator(void);
		char next(char note);
		char next();
		void loadMidi(std::string midi_file);
		void loadMidi(std::string midi[], size_t size);
		void printMatrix();
		int getSeed();
		void setSeed(int new_seed);
		void setLast(char note);
	protected:
		int seed;
		std::mt19937 rng; //random number generator
		unsigned int valueToInt(char c[], int size);
		int octave;
	private:
		int probabilities[12][12];
		char last_note;
		int getCeiling(char note);
		char stringToMidi(std::string note);
};
