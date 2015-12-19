#ifndef KODA_ENCODER_H
#define KODA_ENCODER_H

#include <vector>
#include "Symbol.h"

using namespace std;

class Encoder {

public:
    vector<char> encode(vector<Symbol*> data);

private:
    void calculateProbabilities(vector<Symbol*> data);
};


#endif //KODA_ENCODER_H
