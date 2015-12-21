#include <typeinfo>
#include <iostream>
#include "Character.h"

bool Character::operator==(const Symbol & symbol) {
    try {
        const Character &second = dynamic_cast<const Character &>(symbol);
        return this->val_ == second.val_;

    } catch (const std::bad_cast& e) {
        std::cerr << "Bad cast to character in operator==" << std::endl;
        throw e;
    }
}

bool Character::operator!=(const Symbol & symbol) {
    return !(*this == symbol);
}

std::string Character::toString() {
    return std::string(1, this->val_);
}

std::string Character::stringRepresentation() const {
    return std::string(1, this->val_);
}

bool Character::operator<(Symbol const &symbol) {
    try {
        const Character &second = dynamic_cast<const Character &>(symbol);
        return this->val_ < second.val_;

    } catch (const std::bad_cast& e) {
        std::cerr << "Bad cast to character in operator==" << std::endl;
        throw e;
    }
}
