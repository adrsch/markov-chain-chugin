
Markov Chain Chugin for Chuck


1. Installation

To use, type make <platform name>
Where platform name is either linux, win32, or osx. Make sure ChucK is installed first! See the entry below on installing ChucK if this is needed. Windows/OSX users, see the addendum at the bottom of the section.

Afterwards, type make install

Addendum:
The ChuGin template generator, Chuginate, was used. It is found in the official ChucK repo for ChuGins (https://github.com/spencersalazar/chugins/tree/master/chuginate). The makefile was modified somewhat from the original Chuginate version, however I am only able to test the linux version, so I am not sure if the win32 and osx versions work.

2. Usage

Try it out in action!
Type "chuck test" for a silent test, or "chuck test-audio" to hear some sine waves.

To use in Chuck:
(note that the following is identical to test.ck)

MarkovGenerator mgen;
mgen.loadMidi("filename.mid");
mgen.printMatrix(); //print the matrix used for generation - each number is a note played in the loaded midis
mgen.last(0); //this sets the prior note for generation, in midi numbering (0-12 is C0 to C1). by default, C0 is used.
<<<mgen.next()>>> //this function gets the next note from the generator, in midi numbering.
mgen.seed() => mgen.seed; //this is the seed used for generation. if you like what you get, save the seed!

3. Installing ChucK

IMPORTANT: CHUCK MUST BE INSTALLED! To install ChucK, download http://chuck.cs.princeton.edu/release/files/chuck-1.4.0.0.tgz

Build instructions:
http://chuck.cs.princeton.edu/doc/build/
Requirements:
libsndfile http://www.mega-nerd.com/libsndfile/ (although you can likely get it from a package manager)
ALSA, JACK, or OSS
gcc, lex, yacc, make

In essence, likely what will be needed, with aptitude, is:
apt-get install libsndfile1-dev
apt-get install libasound2-dev (possibly)
Uncompress the tar and go into the folder
cd ./src
make linux-alsa
make install

5. Legal Stuff

This chugin uses the midifile library, with the below license:

Copyright (c) 1999-2018, Craig Stuart Sapp
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   and the following disclaimer in the documentation and/or other materials 
   provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
