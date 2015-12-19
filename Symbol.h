#ifndef KODA_SYMBOL_H
#define KODA_SYMBOL_H

#include <string>

/**
 * Class representing an abstract symbol from some alphabet.
 */
class Symbol {
public:
    virtual bool operator==(const Symbol&) = 0;
    virtual std::string toString() = 0;

    // must be constant for symbols with the same meaning - it is used during hashcode calculation
    virtual std::string stringRepresentation() const = 0;

    virtual ~Symbol() = 0;
};

class EqualFunction {
public:
    bool operator()(Symbol* s1, Symbol* s2) const {
        return (*s1) == (*s2);
    }
};

class HashFunction {
public:
    std::size_t operator()(Symbol const* s) const {
        return std::hash<std::string>()(s->stringRepresentation());
    }
};

#endif //KODA_SYMBOL_H
