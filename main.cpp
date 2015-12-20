#include <iostream>
#include <vector>
#include "Symbol.h"
#include "Character.h"
#include "Encoder.h"

int main(int argc, char** argv) {
    // char buf[] = {'a', 'e', 'f', 'g', 'a', 'b', 'c', 'x', 'w', 'b', 'a'};
    std::string str = "ARYTMETYKA";

    std::vector<Symbol*> entry_data;
    for (unsigned i = 0; i < str.length(); i++) {
        Character* c = new Character(str.at(i));
        entry_data.push_back(c);
    }

    Encoder encoder;
    encoder.encode(entry_data);
}