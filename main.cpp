#include <iostream>
#include <vector>
#include "Symbol.h"
#include "Character.h"
#include "Encoder.h"

int main(int argc, char** argv) {
    char buf[] = {'a', 'e', 'f', 'g', 'a', 'b', 'c', 'x', 'w', 'b', 'a'};

    std::vector<Symbol*> entry_data;
    for (int i = 0; i < 11; i++) {
        Character* c = new Character(buf[i]);
        entry_data.push_back(c);
    }
    EqualFunction f;
    std::cout << (f(entry_data[10], entry_data[4])) << std::endl;

    Encoder encoder;
    encoder.encode(entry_data);
}