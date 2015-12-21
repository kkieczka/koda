#include <iostream>
#include <vector>
#include "Symbol.h"
#include "Character.h"
#include "Encoder.h"

#define TESTCASES 100000

int main(int argc, char** argv) {
    // char buf[] = {'a', 'e', 'f', 'g', 'a', 'b', 'c', 'x', 'w', 'b', 'a'};
//    std::string str = "7V205{P}u|jTG tX'Hu7:5+X@7I*MGUe{gu1cD,V>v+bs|;zB/3\\D;3aO\\k}!A`|&S+jw4>3+Jv G2wj>(";
//
//    std::vector<Symbol*> entry_data;
//    for (unsigned i = 0; i < str.length(); i++) {
//        Character* c = new Character(str.at(i));
//        entry_data.push_back(c);
//    }
//
//    Encoder e;
//    vector<char> encoded = e.encode(entry_data);
//    vector<Symbol*> res = e.decode(encoded, (int)str.length());
//    cout << "out: ";
//    for (int i = 0; i < str.length(); i++) {
//        cout << res[i]->toString();
//    }
//    cout << endl;

    srand(0);
    for (int test = 0; test < TESTCASES; test++) {
        int size = 78;//(rand() + 1) % 90;
        cout << "[" << test << "] len = " << size << endl;
        vector<Symbol*> testVec;
        cout << "i: ";
        for (int i = 0; i < size; i++) {
            char x = (char)((rand() % 95) + 0x20);
            Character * c = new Character(x);
            cout << c->toString();
            testVec.push_back(c);
        }
        cout << endl;
        Encoder e;
        vector<char> encoded = e.encode(testVec);
        vector<Symbol*> res = e.decode(encoded, size);
        cout << "o: ";
        for (int i = 0; i < size; i++) {
            cout << res[i]->toString();
        }
        cout << endl;

        bool equal = true;
        for (int i = 0; i < size; i++) {
            if (*(testVec[i]) != *(res[i])) {
                equal = false;
                break;
            }
        }
        if (equal) {
            cout << "equal" << endl;
        } else {
            cout << "NOT EQUAL!!!" << endl;
            break;
        }
    }

}