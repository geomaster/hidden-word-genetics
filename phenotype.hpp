#pragma once
#include <string>
#include "main.hpp"
#include <random>

using namespace std;

/** Represents a single organism in the population */
class phenotype
{
private:
    /** Cached fitness value, to avoid recomputing every time */
    mutable int cached_fitness;

public:
    /** Genome, i.e. the word this organism represents */
    string genome;

    /** Default constructor */
    phenotype(string _genome) : cached_fitness(-1), genome(_genome)
    {
    }

    phenotype(const phenotype& other) = default;
    phenotype(phenotype&& other) = default;
    phenotype& operator=(const phenotype& other) = default;
    phenotype& operator=(phenotype&& other) = default;

    /** Compute the fitness of the organism */
    int fitness() const
    {
        if (cached_fitness < 0) {
            /* Not cached, compute */
            int fitness = 0;
            for (size_t i = 0; i < word.length(); i++) {
                /* If the character matches the right word, +1 for fitness */
                fitness += (word[i] == genome[i] ? 1 : 0);
            }

            cached_fitness = fitness;
        }

        return cached_fitness;
    }

    /** Randomly this organism and return the result */
    phenotype mutate() const
    {
        /* Pick a random index, replace it with a random char */
        uniform_int_distribution<size_t> dist(0, word.length() - 1);
        auto index = dist(mt);
        auto s = genome;
        s[index] = random_char();

        return phenotype(s);
    }

    /** Crossover this organism with another one to create a new child organism,
      * and return the result */
    phenotype crossover(const phenotype& other) const
    {
        /* Pick the source for each character with a 0.5 probability of being
         * either parent */
        string child = "";
        uniform_int_distribution<int> dist(0, 1);
        for (size_t i = 0; i < word.length(); i++) {
            auto flip = dist(mt);
            child.push_back(flip ? genome[i] : other.genome[i]);
        }

        return phenotype(child);
    }

    /** Comparison operator, compares by fitness */
    bool operator<(const phenotype& other) const
    {
        return fitness() < other.fitness();
    }

    /** Generates a random organism and returns it */
    static phenotype random()
    {
        /* Pick word.length() random chars */
        string s = "";
        for (size_t i = 0; i < word.length(); i++) {
            s.push_back(random_char());
        }

        return phenotype(s);
    }

    /** Return the fitness of the best possible organism */
    static int ideal_fitness()
    {
        return word.length();
    }
};


