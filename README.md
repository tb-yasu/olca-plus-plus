Introduction

OLCA++ is a c++ implementation of online LCA (1), which is an online grammar-based compression algorithm. It takes a text as input, and builds a straight line program consisting of restricted production rules in a context free grammar . Our implementation uses a variable-length dictionary for memory-efficiency (2).

Quick Start

To compile olca++, please type the following commands:

tar -xjvf online-lca-vld-public-X.X.X.tar.bz2

cd online-lca-vld-public-X.X.X/src

make

To build a grammar from a given text, please type the following command:

./olca-compress ../dat/ex.txt grammar

where ex.txt is an input text and grammar is an output file. 
To recover an original text from a grammar, please type the following command:

./olca-decompress grammar output.txt

where output.txt included the original text. 

References

(1) S. Maruyama, H. Sakamoto, M. Takeda, An Online Algorithm for Lightweight Grammar-Based Compression,  Algorithms 5(2):214-235, 2012.

(2) Y.Takabatake, Y.Tabei, H.Sakamoto, Variable-Length Codes for Space-Efficient Grammar-Based Compression, In proceedings of the 19th International Symposium on String Processing and Information Retrieval (SPIRE), 2012.
