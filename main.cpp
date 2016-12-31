#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>
#include <cstdio>

#include "phenotype.hpp"
#include "ecosystem.hpp"

/** Returns a random valid character (one of 26 uppercase ASCII letters) */
char random_char()
{
    uniform_int_distribution<int> dist(0, 25);
    return 'A' + dist(mt);
}

/** Checks if the word given is valid or not (only uppercase letters are allowed)
  * and prints a message if not. */
bool validate_word(const string& w)
{
    for (size_t i = 0; i < w.length(); i++) {
        if (w[i] < 'A' || w[i] > 'Z') {
            cout << "[!] Invalid word. Uppercase ASCII letters only." << endl;
            return false;
        }
    }

    return true;
}

int main()
{
    do {
        cout << "[+] Enter the hidden word to search for: ";
        cin >> word;
    } while (!validate_word(word));

    ecosystem e;
    e.optimize(MAX_EPOCHS);

    return 0;
}
