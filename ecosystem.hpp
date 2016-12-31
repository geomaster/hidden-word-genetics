#pragma once
#include "phenotype.hpp"
#include "main.hpp"
#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

/**
 * Describes an ecosystem of evolving generations.
 */
class ecosystem
{
protected:
    /** List of the phenotypes that make the population **/
    vector<phenotype> population;
    /** Sum of all fitness values in the population **/
    int fitness_sum;

    /** Prints the whole population. Used for debugging purposes. */
    void print_population()
    {
        for (const auto& p : population) {
            cout << p.genome << " ";
        };
    }

    /** Generates a random population. **/
    void gen_random_population()
    {
        population.clear();
        population.reserve(POPULATION_SIZE);
        for (size_t i = 0; i < POPULATION_SIZE; i++) {
            population.push_back(phenotype::random());
        }
    }

    /** Computes the summed fitness of the whole population. */
    void compute_sum()
    {
        fitness_sum = 0;
        for (const auto& p : population) {
            fitness_sum += p.fitness();
        }
    }

    /** Selects a phenotype according to the selection algorithm */
    pair<phenotype, phenotype> select()
    {
        // Fitness proportionate selection
        uniform_int_distribution<int> dist(0, fitness_sum + population.size());
        pair<phenotype, phenotype> result("", "");

        // To pick 2 parents
        for (auto k = 0; k < 2; k++) {
            int sum = 0;
            int toss = dist(mt);
            phenotype choice("");

            /* Biased dice */
            for (size_t i = 0; i < population.size(); i++) {
                sum += 1 + population[i].fitness();
                if (toss <= sum) {
                    choice = population[i];
                    break;
                }
            }

            if (k == 0) {
                result.first = choice;
            } else {
                result.second = choice;
            }
        }

        return result;
    }

    /** Performs a single epoch, replacing the population with a new generation. */
    void epoch()
    {
        vector<phenotype> new_pop;
        new_pop.reserve(POPULATION_SIZE);
        for (size_t i = 0; i < POPULATION_SIZE / 2; i++) {
            /* Pick parents and create two children */
            for (auto k = 0; k < 2; k++) {
                pair<phenotype, phenotype> parents = select();
                new_pop.push_back(parents.first.crossover(parents.second));
            }
        }

        population = new_pop;

        /* Mutate randomly */
        uniform_real_distribution<double> dist(0.0, 1.0);
        for (size_t i = 0; i < POPULATION_SIZE; i++) {
            auto toss = dist(mt);
            if (toss < MUTATION_PROBABILITY) {
                population[i] = population[i].mutate();
            }
        }

        compute_sum();
    }

    /** Returns the best phenotype in the population */
    phenotype best()
    {
        return *max_element(population.begin(), population.end());
    }

    /** Padded print */
    void print_pad(const string& s, size_t l, ostream& ss)
    {
        ss << s;
        for (int i = 0; i < ((int)l - (int)s.length()); i++) {
            ss << " ";
        }
    }

    /** Prints the statistics header */
    void print_stat_header()
    {
        print_pad("CURRENT", word.length(), cout);
        cout << "  ";
        print_pad("FITNESS", 10, cout);
        cout << "  ";
        print_pad("EPOCH #", 10, cout);
        cout << endl;
    }

    /** Prints the current statistics of the ecosystem */
    void print_stats(const phenotype& best, size_t epoch)
    {
        ostringstream res;
        res << "\33[2K\r"; /* VT100 escape for going back to the beginning of the line */
        print_pad(best.genome, 7, res);
        res << "  ";

        ostringstream ft;
        ft << best.fitness();
        print_pad(ft.str(), 10, res);

        res << "  ";

        ostringstream ep;
        ep << epoch;
        print_pad(ep.str(), 10, res);

        cout << res.str();
        fflush(stdout);
    }

public:
    /** Performs the genetic-based optimization */
    string optimize(size_t max_epochs)
    {
        gen_random_population();
        compute_sum();
        size_t epochs = 0;
        auto printed_headers = false;

        cout << "[+] Starting search for a " << word.length() << "-character hidden word" << endl <<
                "[-]   Population size:       " << POPULATION_SIZE << endl <<
                "[-]   Mutation probability:  " << MUTATION_PROBABILITY << endl <<
                "[-]   Max epochs to run:     " << max_epochs << endl;

        phenotype bst("");
        auto global_best = best();

        /* While we haven't found the solution, or until we hit the epoch count
         * limit */
        while ((bst = best()).fitness() != phenotype::ideal_fitness() && epochs < max_epochs)
        {
            epoch();
            epochs++;

            /* Print the stats each PRINT_GRANULARITY epochs */
            if (epochs % PRINT_GRANULARITY == 0) {
                if (!printed_headers) {
                    cout << endl;
                    print_stat_header();
                    printed_headers = true;
                }
                print_stats(bst, epochs);
            }

            /* Record the globally best phenotype */
            if (bst.fitness() > global_best.fitness()) {
                global_best = bst;
            }
        }

        /* Print the final stats */
        if (!printed_headers) {
            cout << endl;
            print_stat_header();
        }
        print_stats(bst, epochs);
        cout << endl << endl;

        /* Run the debug output */
        bst = best();
        if (bst.fitness() == phenotype::ideal_fitness()) {
            cout << "[*] Found the solution in " << epochs << " epochs." << endl;
            return bst.genome;
        } else {
            cout << "[!] Solution not found after " << epochs << " epochs." << endl <<
                    "[-]   Best candidate: " << global_best.genome << endl << 
                    "[-]   Fitness:        " << global_best.fitness() << endl;

            return global_best.genome;
        }
    }
};

