#!/bin/bash
# Izaak White and Joshua Moner
echo "Building tree_builder..."
make

echo "Running tree_builder parser on plisTest.txt..."
./tree_builder < plisTest.txt > parser_output.txt

echo "Parsing complete. Results saved to parser_output.txt"
cat parser_output.txt

make clean