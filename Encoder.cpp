#include <unordered_map>
#include <iostream>
#include "Encoder.h"

using namespace std;

vector<char> Encoder::encode(vector<Symbol*> data) {

    // determine the set of symbols in input data

    // calculate probabilities of symbols
    calculateProbabilities(data);

    // recursively (?) execute the encoder

    return vector<char>();
}

void Encoder::calculateProbabilities(vector<Symbol*> data) {

    unordered_map<Symbol*, int, HashFunction, EqualFunction> symbols_counts;

    for (Symbol* s : data) {
        symbols_counts[s]++;
    }

    for (auto pair : symbols_counts) {
        cout << static_cast<Symbol*>(pair.first)->toString() << " " << pair.second << endl;
    }
}
