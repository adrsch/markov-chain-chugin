
Markov Chain Chugin for Chuck

To use, type make <platform name>
Where platform name is either linux, win32, or osx. Make sure ChucK is installed first! See the entry below on installing ChucK if this is needed.

Afterwards, type make install

To use in Chuck:
MarkovGenerator mgen;
mgen.loadMidi("filename.mid");
mgen.printMatrix(); //print the matrix used for generation - each number is a note played in the loaded midis
mgen.last(0); //this sets the prior note for generation, in midi numbering (0-12 is C0 to C1). by default, C0 is used.
<<<mgen.next()>>> //this function gets the next note from the generator, in midi numbering.
mgen.seed() => mgen.seed; //this is the seed used for generation. if you like what you get, save the seed!

Installing ChucK

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

You might also want to do:
apt-get install libpulse-dev
make linux-pulse
Instead of using the linux-alsa version, because, at least on my test machine, alsa has trouble actually playing any audio. That being said, this chugin itself doesn't have anything audio-related in it, although the provided test-audio.ck file will play audio if it is supported on your machine.

An Addendum For Visual Studio Users

You may notice VS project files are pretty much always included with ChuGins. This is because Chuginate, which generates a blueprint for making ChuGins, generates them automatically. However, as I don't use VS and would have no way to verify if they actually worked, so I removed them. If you want them, generate a fresh blueprint from Chuginate for MarkovGenerator, then paste this code over it and you'll have a version with VS project files.

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
