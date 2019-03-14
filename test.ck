MarkovGenerator test;
test.loadMidi("roygbiv.mid");
test.printMatrix();
test.last(12*4);
<<<test.seed()>>>;
SinOsc s => ADSR e => dac;
e.set(10::ms, 500::ms, 0, 500::ms);
while (true) {
	int note;
	test.next() => note;
	<<<note>>>;
	<<<test.order()>>>;
	Math.mtof(note) => s.freq;
	e.keyOn();
	200::ms => now;
	e.keyOff();
}

