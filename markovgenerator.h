#include<string>

class MarkovGenerator {
	public:
		MarkovGenerator(void) : probabilities(), last_note(), random_ceiling() {}
		MarkovGenerator(std::string midi);
		MarkovGenerator(std::string midi[]);
		~MarkovGenerator(void);
		char next(char note);
		void loadMidi(std::string midi_file);
		void loadMidi(std::string midi[], size_t size);
		void printMatrix();
	protected:
		unsigned int valueToInt(char c[], int size);
		int probabilities[12][12];
		int last_note;
		int random_ceiling;
		void updateCeiling();
		char stringToMidi(std::string note);
};
