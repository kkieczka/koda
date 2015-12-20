#ifndef KODA_ENCODER_H
#define KODA_ENCODER_H

#include <vector>
#include <unordered_map>
#include <map>
#include "Symbol.h"

using namespace std;

class Encoder {

public:
    vector<char> encode(vector<Symbol*> data);
    void decode(vector<char> input, int size, unordered_map<Symbol*, int, HashFunction, EqualFunction> indexes_in_high_values, vector<int> high_values);

    Encoder() : probabilities_(nullptr), counts_(nullptr) {}

    ~Encoder();

private:
    unordered_map<Symbol*, double>* probabilities_;
    map<Symbol*, int, LessThanFunction>* counts_;
    //unordered_map<Symbol*, int, HashFunction, EqualFunction>* counts_;
    void calculateProbabilities(vector<Symbol*> data);
};


#endif //KODA_ENCODER_H
