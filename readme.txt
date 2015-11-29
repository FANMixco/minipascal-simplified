Developed by Federico Navarrete in 2015 for Compiler Construction at Politechnika Łódzka (www.p.lodz.pl/).

The Pascal grammar from this compiler in Yacc/BISON format, it was based on dr inż. Grzegorz Jabłoński grammar but it was modified for Educational Purposes.
http://neo.dmcs.p.lodz.pl/tk/asu-appendix.htm  (2015-06-13, 09:50)

The project it's inspired on:
Write Your Own Toy Compiler Using Flex, Bison and LLVM by Loren Legal in 2009
http://gnuu.org/2009/09/18/writing-your-own-toy-compiler/ (2015-05-10, 22:15)

Compiler Construction using Flex and Bison from Microsoft Research by Anthony A. Aaby, Walla Walla College:
http://research.microsoft.com/en-us/um/people/rgal/ar_language/external/compiler.pdf (2015-06-01, 08:10)

A compiler from ADA to LLVM developed by the Department of Computer Science, National Chiao Tung University in 2010.
http://people.cs.nctu.edu.tw/~chenwj/compiler/html/ (2015-04-10, 23:57)

The current project includes Different OpenSource Projects and it's rights are exclusive from their Inventors or Supporters.
This project was developed and tested on:
	- Xubuntu 14.04 x86
	- FLEX 2.5.35
	- BISON 3.0.2

How to install those libraries in your Ubuntu quickly?
Open the console and type:
	sudo su
	apt-get install flex
	apt-get install bison

Some known issues:
- Not all versions of LLVM compile the result.ill file.

Future improvements:
- MOD operation due to some limitations of LLVM language, possible alternative:
How to replace modulo operator by using bitwise AND operator?
http://robertgawron.blogspot.com/2011/02/modulo-operation-using-bitwise.html (2015-06-13 09:56)
- Operations between Reals and Integers, right now the operations are just Integers with Integers and Reals with Reals.

More info about the software mentioned above:
http://foja.dcs.fmph.uniba.sk/kompilatory/docs/compiler.pdf
http://cdimage.ubuntu.com/xubuntu/releases/12.04/release/
http://flex.sourceforge.net/manual/
http://www.eecg.toronto.edu/~jzhu/csc467/readings/csc467-bison-tut.pdf
http://neo.dmcs.p.lodz.pl/cc/LLVMIntro.pdf
http://llvm.org/docs/
http://llvm.org/releases/2.5/docs/LangRef.html

Any problem during the compilation or deployment of it, it might be due to wrong libraries or your Ubuntu Version, please check it before any test.
This project is licenced by MIT Licence:

The MIT License (MIT)

Copyright (c) 2015, Federico Navarrete

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.