MarkovGenerator test;
test.loadMidi("vexation.mid");
3 => test.order;
test.printMatrix();
test.last(12*4);
<<<test.seed()>>>;
SinOsc s => ADSR e => dac;
e.set(10::ms, 500::ms, 0, 500::ms);
//while (true) {
	int note;
	test.next(4+12*4) => note;
	<<<note>>>;
	<<<test.order()>>>;
	Math.mtof(note) => s.freq;
	e.keyOn();
	200::ms => now;
	e.keyOff();
//}

