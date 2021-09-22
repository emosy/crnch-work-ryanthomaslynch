// Ryan Thomas Lynch
// Georgia Institute of Technology
// ryan.lynch@gatech.edu
// CRNCH Lab
// based on pseudocode from https://doi.org/10.1145/3457388.3458670 and Python code from Patrick Lavin

#include <cstdio>
#include <cinttypes>
#include <iostream>

#include <memory>
// #include <deque>
// #include <map>
#include <vector>
#include <bits/stdc++.h>
// #include <bit>
#include <functional>
// #include <pair>

using namespace std;


double threshold = 0.5;
int interval_len = 10000;
const int signature_len = 1024;
const int log2_signature_len = 10; // should be log 2 of the above value
int drop_bits = 3;

using bitvec = bitset<signature_len>;

bitvec current_signature;
bitvec last_signature;

hash<bitvec> hash_bitvec;

uint64_t instruction_count = 0;
uint64_t stable_count = 0;
uint64_t stable_min = 3;
typedef int phase_t;
phase_t phase = -1;

vector<bitvec> phase_table;


//phase trace?? should it be deque/stack or vector/arraylist?

vector<phase_t> phase_trace;


double difference_measure_of_signatures(bitvec sig1, bitvec sig2) {
    auto xor_signatures = sig1 ^ sig2;
    auto or_signatures = sig1 | sig2;
    return static_cast<double>(xor_signatures.count()) / or_signatures.count(); // this should work with any compiler
    // return ((double) xor_signatures.__builtin_count()) / or_signatures.__builtin_count(); // this might only work with GCC
}

uint64_t hash_address(bitvec sig) {
    auto signature_minus_bottom_drop_bits = sig >> drop_bits;
    return hash_bitvec(signature_minus_bottom_drop_bits) >> (64 - log2_signature_len);
    //i don't entirely understand why im doing the right shift by only 54 bits... like why 64 - 10?
}

void phase_detector(uint64_t instruction_pointer) {
    auto ip = instruction_pointer;
    current_signature[hash_address(ip)] = 1;
    instruction_count += 1; // should this be before or after the if?

    if (instruction_count % interval_len == 0) {
        // we are on a boundary! determine phase and notify listeners

        //first, check if the phase is stable since the difference measure is acceptably low
        if (difference_measure_of_signatures(current_signature, last_signature) < threshold) {
            stable_count += 1;
            if (stable_count >= stable_min && phase == -1) {
                //add the current signature to the phase table and make the phase # to its index
                phase_table.push_back(current_signature);
                phase = phase_table.size() - 1; // or indexof curr_sig?
                //line 194 in the python
            }
        } else { //line 196 in python
            //if difference too high then it's not stable and we might now know the phase
            stable_count = 0;
            phase = -1;

            //see if we've entered a phase we have seen before
            if (!phase_table.empty()) { //line 201 python
                // vector<double> difference_scores_from_phase_table;
                double best_diff = threshold; 
                for (auto phase_table_iterator = phase_table.begin(); phase_table_iterator != phase_table.end(); phase_table_iterator++) {
                    auto s = *phase_table_iterator;
                    auto diff = difference_measure_of_signatures(current_signature, s);
                    // difference_scores_from_phase_table.push_back(diff);
                    if (diff < threshold && diff < best_diff) {
                        phase = std::distance(phase_table.begin(), phase_table_iterator);
                        best_diff = diff;
                        // phase = index of s in phase_table?
                        //oh yeah cuz phase is index in phase table duh
                        //set current phase to the phase of the one with the lowest difference from current
                    }
                }
                
                // double best = 0;
                // auto best = difference_scores_from_phase_table. best = index of max of diff scores? shouldn't it be min?
                // if (similar[best] < threshold)
            }
        }
        //whether or not the phase is stable, we need to update last phase and whatnot
        last_signature = current_signature;
        current_signature.reset();
        //TODO: append curr phase to phase trace?
        //need to figure out if this should be random access (vector) or sequential (stack/deque)
        phase_trace.push_back(phase);


        //TODO: callback listeners??? from line 212 in python

        //TODO: add addr info to phase, dwarf map?

    }
}