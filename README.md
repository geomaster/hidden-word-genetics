# Hidden word search using a genetic algorithm

A small piece of software, written as an exercise for my intelligent systems
class, which applies a genetic algorithm to the problem of guessing a hidden
word. The word is input at the beginning, and the algorithm uses only the fitness
function, defined as the number of indexes on which the letter was correctly
guessed, to drive the optimization.

## Building and running

Easy enough, run
```
$ make
```
or compile `main.cpp` in your favorite C++11 compiler or IDE. Then run the
resulting application and enter the word.

## Short description

### Mode of operation

A phenotype (individual member of the generation) is a string. The fitness
of the phenotype is computed as described above.

The size of the generation is fixed, but configurable in the code (256 by
default). In each iteration, a new generation is created from the previous one,
using the following algorithm:

1. Parents are selected using fitness-proportionate random selection
2. Crossover is done twice on the parents, to create new children
3. Steps 1 and 2 are repeated until the new generation has enough members
4. Each phenotype in the resulting generation is mutated with a probability **p**
   (set to 0.1 in code)

Crossover is performed by taking each letter either from one parent or the other,
with probability 0.5.

Mutation is done by picking a random index and replacing the letter at that index
with a random letter.

**Fitness-proportionate selection** means that a member *X_i* has the probability
*p* to be selected, where *p = fitness(X_i) / sum[fitness(X_j) for j=1..n]*.
