#include <unordered_map>
#include <map>
#include <iostream>
#include <iomanip>
#include <bitset>
#include "Encoder.h"
#include "Character.h"

#define MSB(i) (i >> 7)
#define MS2B(i) (i >> 6)
#define LSB(i) (i & 0x01)

using namespace std;

vector<char> Encoder::encode(vector<Symbol*> data) {

    // calculate counts and probabilities of symbols
    calculateProbabilities(data);

    // create "discrete distribution function"

//    vector<pair<Symbol*, int> > testvector;
//    testvector.push_back(pair<Symbol*, int>(new Character('A'), 2));
//    testvector.push_back(pair<Symbol*, int>(new Character('R'), 1));
//    testvector.push_back(pair<Symbol*, int>(new Character('Y'), 2));
//    testvector.push_back(pair<Symbol*, int>(new Character('T'), 2));
//    testvector.push_back(pair<Symbol*, int>(new Character('M'), 1));
//    testvector.push_back(pair<Symbol*, int>(new Character('E'), 1));
//    testvector.push_back(pair<Symbol*, int>(new Character('K'), 1));
    // vector of cumulated symbol frequencies; synchronized with counts_
    vector<int> high_values;

    // map: symbol -> index of its cumulated frequency in high_values table
    unordered_map<Symbol*, int, HashFunction, EqualFunction> indexes_in_high_values;

    int current_count = 0;
    int in = 0;
    for (auto pair : *counts_) {
        current_count += pair.second;
        high_values.push_back(current_count);
        indexes_in_high_values[pair.first] = in;
        //cout << pair.first->toString() << " " << current_count << endl;
        in++;
    }


    cout << "Starting encoding..." << endl;

    vector<char> output;

    uint8_t up = 0xFF, down = 0x0; // TODO increase the width of registers used in encoding
    int range;
    int ln = 0;

    char byte_of_output = 0;
    int curr_index_in_byte = -1;
    for (Symbol* s : data) {
        range = up - down + 1;
        int index = indexes_in_high_values[s];
        uint8_t old_down = down;
        down = (uint8_t) (old_down + range * (index > 0 ? high_values[index - 1] : 0) / data.size());
        up = (uint8_t) (old_down + range * high_values[index] / data.size() - 1);

        cout << "down: " << bitset<8>(down) << "(" << dec << (int)down << "), up: " << bitset<8>(up) << "(" << dec << (int)up << ") (" << s->toString() << ")" << endl;

        while(1) {
            if (MSB(down) == MSB(up)) {

                ++curr_index_in_byte;
                byte_of_output <<= 1;
                byte_of_output |= MSB(down); cout << "out " << hex << MSB(down) << endl;
                if (curr_index_in_byte == 7) {
                    output.push_back(byte_of_output);
                    byte_of_output = 0;
                    curr_index_in_byte = -1;
                }

                while (ln > 0) {

                    ++curr_index_in_byte;
                    byte_of_output <<= 1;
                    byte_of_output |= MSB((uint8_t)(~down));  cout << "out " << hex << MSB((uint8_t)(~down)) << endl;
                    if (curr_index_in_byte == 7) {
                        output.push_back(byte_of_output);
                        byte_of_output = 0;
                        curr_index_in_byte = -1;
                    }

                    --ln;
                }
                //down <<= 1;
                //up <<= 1;
                //up |= 1;

            } else if (MS2B(down) == (uint8_t)0x1 && MS2B(up) == (uint8_t)0x2) {
                cout << "E3" << endl;
                //down <<= 1;
                //down &= 0x7F;
                //up <<= 1;
                //up |= 0x80;
                //up |= 1;
                down &= 0x3F;
                up |= 0x40;
                //cout << "-- down: " << bitset<8>(down) << "(" << (int)down << "), up: " << bitset<8>(up) << "(" << (int)up << ")" << endl;
                ++ln;
            } else break;

            down <<= 1;
            up <<= 1;
            up |= 1;
            cout << "-- down: " << bitset<8>(down) << "(" << dec << (int)down << "), up: " << bitset<8>(up) << "(" << dec << (int)up << ")" << endl;
        }
    }

    uint16_t dol_cpy = down;
    int ind;
    for (ind = 0; ind < 8; ind++) {
        if (LSB(dol_cpy) == 1) break;
        dol_cpy >>= 1;
    }
    if (ind < 8) {
        for (int i = 7; i >= ind; i--) {
            ++curr_index_in_byte;
            byte_of_output <<= 1;
            byte_of_output |= MSB(down);
            if (curr_index_in_byte == 7) {
                output.push_back(byte_of_output);
                byte_of_output = 0;
                curr_index_in_byte = -1;
            }
            down <<= 1;
        }
    }

    // if the last part of output is not a full byte (usual case),
    // push it to the left and fill the (less significant) rest with zeros.
    // In other words - last byte of output is padded with zeros on less significant bits.
    if (curr_index_in_byte != -1) {
        byte_of_output <<= (7 - curr_index_in_byte);
        output.push_back(byte_of_output);
    }

    cout << "Encoded." << endl;

    for (char c : output) {
        cout << hex << (c & 0xFF) << " ";
    }
    cout << endl;
    decode(output, (int)data.size(), indexes_in_high_values, high_values);
    return output;
}


void Encoder::decode(vector<char> input, int size, unordered_map<Symbol*, int, HashFunction, EqualFunction> indexes_in_high_values, vector<int> high_values) {

    cout << "Decoding..." << endl;
    int curr_input_byte = 0;
    int curr_input_bit = 0;

    uint8_t code = static_cast<uint8_t>(input[curr_input_byte++]);
    uint8_t up = 0xFF, down = 0x0;


    for (int t = 0; t < size; t++) {

        int range = (up - down) + 1;
        int temp = ((((code - down) + 1) * size) - 1) / range;

        //cout << "\ntemp = " << temp;

        int ind = 0;
        while (temp >= high_values[ind]) ind++;

        // reverse search for symbol in indexes_in_high_values
        Symbol *s = nullptr;
        for (auto pair : indexes_in_high_values) {
            if (pair.second == ind) {
                s = pair.first;
                break;
            }
        }

        cout << s->toString();


        uint8_t old_down = down;
        int index = indexes_in_high_values[s];
        down = (uint8_t) (old_down + range * (index > 0 ? high_values[index - 1] : 0) / size);
        up = (uint8_t) (old_down + range * high_values[index] / size - 1);

        while (1) {
            if (MSB(up) == MSB(down)) {
                // shift
            } else if (MS2B(down) == 0x1 && MS2B(up) == 0x2) {
                code = (uint8_t) (code ^ 0x40);
                down &= 0x3F;
                up |= 0x40;
                // shift
            } else break;

            down <<= 1;
            up <<= 1;
            up |= 0x1;
            code <<= 1;
            if (curr_input_bit == 8) {
                curr_input_bit = 0;
                curr_input_byte++;
            }
            code |= ((input[curr_input_byte] >> (7 - curr_input_bit)) & 0x1);
            //cout << "\ncode = " << bitset<8>(code);
            curr_input_bit++;
        }
    }
}


void Encoder::calculateProbabilities(vector<Symbol*> data) {

    if (counts_ != nullptr)
        delete counts_;

    counts_ = new map<Symbol*, int, LessThanFunction>;

    for (Symbol* s : data) {
        (*counts_)[s]++;
    }

    if (probabilities_ != nullptr)
        delete probabilities_;

    probabilities_ = new unordered_map<Symbol*, double>();

    for (auto pair : *counts_) {
        (*probabilities_)[pair.first] = (double)pair.second / data.size();
    }

    for (auto pair : *probabilities_) {
        cout << static_cast<Symbol*>(pair.first)->toString() << " " << pair.second << endl;
    }
}

Encoder::~Encoder() {
    if (counts_ != nullptr)
        delete counts_;

    if (probabilities_ != nullptr)
        delete probabilities_;
}
