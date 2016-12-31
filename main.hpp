#pragma once
#include <random>

using namespace std;

random_device rd;
mt19937 mt(rd());
string word;

const size_t POPULATION_SIZE = 256;
const double MUTATION_PROBABILITY = 0.1;
const size_t PRINT_GRANULARITY = 100;
const size_t MAX_EPOCHS = 50000;

char random_char();
