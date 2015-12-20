#ifndef KODA_CHARACTER_H
#define KODA_CHARACTER_H


#include "Symbol.h"

/**
 * Class representing an ASCII character.
 */
class Character : public Symbol {
public:

    Character(char x) : val_(x) {}

    ~Character() {}

    bool operator==(const Symbol &symbol);
    std::string toString();
    std::string stringRepresentation() const;
    bool operator<(Symbol const &symbol);

private:
    char val_; // actual ASCII character
};


#endif //KODA_CHARACTER_H
