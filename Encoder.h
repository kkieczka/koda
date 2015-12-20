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

    Encoder() : probabilities_(nullptr), counts_(nullptr) {}

    ~Encoder();

private:
    unordered_map<Symbol*, double>* probabilities_;
    map<Symbol*, int, LessThanFunction>* counts_;

    void calculateProbabilities(vector<Symbol*> data);
};


#endif //KODA_ENCODER_H
