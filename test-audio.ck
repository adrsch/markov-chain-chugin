MarkovGenerator test;
test.loadMidi("test.mid");
1 => test.order;
test.printMatrix();
test.last(12*5);
<<<test.seed()>>>;
SinOsc s => ADSR e => dac;
e.set(15::ms, 500::ms, 0, 500::ms);
test.printMatrix();
while (true) {
	int note;
	test.next() => note;
	<<<note>>>;
	Math.mtof(note) => s.freq;
	e.keyOn();
	200::ms => now;
	e.keyOff();
}

