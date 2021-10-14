// # Copyright (c) 2021, Georgia Institute of Technology
// #
// # SPDX-License-Identifier: Apache-2.0
// #
// # Licensed under the Apache License, Version 2.0 (the "License");
// # you may not use this file except in compliance with the License.
// # You may obtain a copy of the License at
// #
// #     http://www.apache.org/licenses/LICENSE-2.0
// #
// # Unless required by applicable law or agreed to in writing, software
// # distributed under the License is distributed on an "AS IS" BASIS,
// # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// # See the License for the specific language governing permissions and
// # limitations under the License.


// Ryan Thomas Lynch
// Georgia Institute of Technology
// ryan.lynch@gatech.edu
// CRNCH Lab
// based on pseudocode from https://doi.org/10.1145/3457388.3458670
// and Python code from Patrick Lavin available at:
// https://github.com/plavin/ModelSwapping/blob/master/PhaseDetector.py


// Phase detection code
// Version 0.4

#include "phase_detector.h"

using namespace std;

//phase detector class defined in phase_detector.h


double phase_detector::difference_measure_of_signatures(bitvec sig1, bitvec sig2) {
    auto xor_signatures = sig1 ^ sig2;
    auto or_signatures = sig1 | sig2;
    return static_cast<double>(xor_signatures.count()) / or_signatures.count(); // this should work with any compiler
    // return ((double) xor_signatures.__builtin_count()) / or_signatures.__builtin_count(); // this might only work with GCC
}

uint64_t phase_detector::hash_address(bitvec sig) {
    auto signature_minus_bottom_drop_bits = sig >> drop_bits;
    //drop the bottom {drop_bits} bits of the signature
    //hash it then return the top bits of the hash (the number of bits determined by the length of the signature)
    //use this to then index into a bitvec that represents the current signature to set a specific bit to 1
    return hash_bitvec(signature_minus_bottom_drop_bits) >> (64 - log2_signature_len);
}

void phase_detector::detect(uint64_t instruction_pointer) {
    auto ip = instruction_pointer;
    current_signature[hash_address(ip)] = 1;
    

    if (instruction_count % interval_len == 0) {
        // we are on a boundary! determine phase and notify listeners

        //first, check if the phase is stable since the difference measure is acceptably low
        if (difference_measure_of_signatures(current_signature, last_signature) < threshold) {
            stable_count += 1;
            if (stable_count >= stable_min && phase == -1) {
                //add the current signature to the phase table and make the phase # to its index
                phase_table.push_back(current_signature);
                phase = phase_table.size() - 1; // or indexof curr_sig?
                // cout << phase << endl;
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
        
        //add the current phase ID to the phase trace - from line 209 in python
        phase_trace.push_back(phase);

        //notify listeners of the current phase ID 
        //from line 212 in python
        for (auto f : listeners) {
            f(phase);
        }


        //TODO: add addr info to phase, dwarf map?

    }
    instruction_count += 1; // should this be before or after the if?
}

void phase_detector::init_phase_detector() {
    current_signature.reset();
    last_signature.reset();
    // hash_bitvec
    instruction_count = 0;
    stable_count = 0;
    phase = -1;
    phase_table.clear();
    phase_trace.clear();
    listeners.clear();
}

void phase_detector::cleanup_phase_detector(string log_file_name) {

    
    if (DEBUG) {
        // if (small_or_medium == 0) {
        ofstream log(log_file_name);
        for (auto p : phase_trace) {
            // cout << p << endl;
            if (log.is_open()) {
                log << p << endl;
            }
        }
        log.close();
        // } else {
        //     ofstream log("phase_trace_medium.txt");
        //     for (auto p : phase_trace) {
        //         // cout << p << endl;
        //         if (log.is_open()) {
        //             log << p << endl;
        //         }
        //     }
        //     log.close();
        // }
    }

    init_phase_detector();


}

void phase_detector::register_listeners(listener_function f) {
    listeners.push_back(f);
}