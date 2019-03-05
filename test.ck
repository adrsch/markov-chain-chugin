MarkovGenerator test;
test.loadMidi("roygbiv.mid");
test.printMatrix();
test.last(12*4);
<<<test.seed()>>>;
SinOsc s => dac;
while (true) {
	int note;
	test.next() => note;
	<<<note>>>;
	Math.mtof(note) => s.freq;
	500::ms => now;
}

