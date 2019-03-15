MarkovGenerator mgen;
<<<"the matrix:">>>;
mgen.loadMidi("test.mid"); //load the midi named test.mid into the generator's matrix
mgen.printMatrix(); //print the first order matrix used for generation - each number is a note played in the loaded midis.
mgen.loadMidi("test.mid"); //load in a second midi, to get a more full table
<<<"after loading another file:">>>;
mgen.printMatrix(); //loading the same midi in twice isn't too interesting, though!
mgen.last(12*5); //set the prior note for generation, in midi numbering (0-12 is C0 to C1). by default, C0 is used. the octave generated notes fall in is also set.
mgen.tonic(2); //set the base note of the octave notes fall in. by default it, is 0.
mgen.seed() => mgen.seed; //this is the seed used for generation. if you like what you hear, save the seed!
mgen.order() => mgen.order; //the order generation is based on. 1-3 are allowed, and 0, the default, randomly picks between these.

while (true) {
	int note;
	mgen.next() => note; //generate the next note, in midi numbering. a new last note to start generation from can optionally be passed, ie test.next(12*4)
	<<<"This time, we got:">>>;
	<<<note>>>;
 it can be played
	500::ms => now;
}
