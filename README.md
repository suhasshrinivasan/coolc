coolc
=====

A compiler for Cool. Cool (Classroom Object Oriented Language) is an object oriented programming language developed at Stanford for their Compilers course. Here's my implementation of the compiler in C. 
Here's the manual for Cool. http://theory.stanford.edu/~aiken/software/cool/cool-manual.pdf

NOTE: WIP. DO NOT RELY ON CODE YET.

#####Compilation instructions
- Make sure you have `gcc` and `git` installed.
	- if you don't, type `sudo apt-get install gcc git`
- clone the git repository using the command `git clone https://github.com/suhasshrinivasan/coolc.git`
- the above step will create a folder named `coolc`. `cd` into that folder `cd coolc`
- now compile the lexer `gcc lexer.c lexerDriver.c`
- there you go. Now you can test a sample code with the lexer
	- if `hello.cl` is the name of the cool program, run with `./a.out hello.cl`
