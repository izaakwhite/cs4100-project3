#!/bin/bash
# Izaak White and Joshua Moner
make

./tree_builder < plisTest.txt > parser_output.txt

cat parser_output.txt

make clean