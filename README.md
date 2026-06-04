# LongestCyclesWithFewChords
This is the code for my thesis "Longest cycles with few chords in 3-connected graphs". It contains an algorithm for searching the longest cycle with minimal chords in a given graph.


## Compiling
The code can simply be compiled by executing the 
```
make
```
command in this directory. To compile the tests, run the "make" command in the ./tests directory.

## Running
To run the program, simply execute 
```
./find_min_chord_cycle < graphs.g6
```
The expected output is something of the form <graph6, circumference, chords, cycle>, like 
```
E_lo 5 0 [4 0 1 5 2 ] 
EC^w 5 2 [5 0 3 4 1 ] 
```
Here, the first term is the graph6 representation of the graph, the second term is the circumference of the graph, i.e. the length of the longest cycle. The third term indicates how many chords the longest cycle with fewest chords has, and lastly that longest cycle with minimal chords is given.

Alternatively, and our recommendation, is to run 
```
./find_min_chord_cycle_with_ham_filter < graphs.g6
```
which stops calculations on hamiltonian graphs and does not output these graphs, since here the number of chords in a longest cycle will always be |E|-|V|.
