"slm" means "Simple Language Model", 
It is a small C++17 program that builds a k-gram language model from training text and generates pseudo-random text based on that model.
-------------------------------
Run
-------------------------------
After building, the executable is "slm". 
The unix command looks like:
./slm <k> <filename> <length>
-------------------------------
Features
-------------------------------
Build k-gram frequency/transition model (class KGramModel).
Generate text using probabilistic selection or maximum-probability selection (class TextGenerator).
Uses C++ standard <random> for reproducible generation.
-------------------------------
Project layout
-------------------------------
main.cpp                — program entry, argument parsing, run generator
classKGramModel.h/.cpp — k-gram model declaration and implementation
classTextGenerator.h/.cpp — text generation (inherits KGramModel)
make_slm.txt (or Makefile) — build rules
README.txt              — this file
-------------------------------
