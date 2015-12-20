#include <unordered_map>
#include <map>
#include <iostream>
#include <iomanip>
#include "Encoder.h"

#define MSB(i) (i >> 7)
#define MS2B(i) (i >> 6)
#define LSB(i) (i & 0x01)

using namespace std;

vector<char> Encoder::encode(vector<Symbol*> data) {

    // calculate counts and probabilities of symbols
    calculateProbabilities(data);

    // create "discrete distribution function"

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

        while(1) {
            if (MSB(down) == MSB(up)) {

                ++curr_index_in_byte;
                byte_of_output <<= 1;
                byte_of_output |= MSB(down);
                if (curr_index_in_byte == 7) {
                    output.push_back(byte_of_output);
                    byte_of_output = 0;
                    curr_index_in_byte = -1;
                }

                while (ln > 0) {

                    ++curr_index_in_byte;
                    byte_of_output <<= 1;
                    byte_of_output |= MSB(~down);
                    if (curr_index_in_byte == 7) {
                        output.push_back(byte_of_output);
                        byte_of_output = 0;
                        curr_index_in_byte = -1;
                    }

                    --ln;
                }
                down <<= 1;
                up <<= 1;
                up |= 1;
            } else if (MS2B(down) == 0x01 && MS2B(up) == 0x10) {
                down <<= 1;
                down &= 0x3F;
                up <<= 1;
                up |= 0x40;
                up |= 1;
                ++ln;
            } else break;
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
    return output;
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
