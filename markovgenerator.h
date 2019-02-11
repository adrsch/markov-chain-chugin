#include<string>

class MarkovGenerator {
	public:
		MarkovGenerator(void);
		MarkovGenerator(std::string midi);
		MarkovGenerator(std::string midi[]);
		~MarkovGenerator(void);
		char next(char note);
		//char next(void);
		//void loadMidi(std::string midi);
		void loadMidi(std::string midi[], size_t size);
	protected:
		static int probabilities[12][12];
		static char last_note;
		char stringToMidi(std::string note);
};
