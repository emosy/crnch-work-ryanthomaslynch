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


// Phase detection header file
// Version 0.6


#include <cstdio>
#include <cinttypes>
#include <iostream>

#include <memory>
#include <deque>
// #include <map>
#include <vector>
#include <bits/stdc++.h>
//TODO: replace bits/stdc++ with something that actually works for other compilers?
// #include <bit>
#include <functional>
// #include <pair>

using namespace std;


// double threshold = 0.5;
// int interval_len = 10000;
// const int signature_len = 1024;
// const int log2_signature_len = 10; // should be log 2 of the above value
// int drop_bits = 3;

#define threshold 0.5
#define interval_len 10000
#define signature_len 1024
#define log2_signature_len 10
#define drop_bits 3
#define DEBUG 1


using bitvec = bitset<signature_len>;

typedef int phase_id_type;

typedef void (*listener_function)(phase_id_type); //listener functor type that takes in a phase ID and returns nothing


// double difference_measure_of_signatures(bitvec sig1, bitvec sig2);
// uint64_t hash_address(bitvec sig);
// void detect(uint64_t instruction_pointer);
// void init_phase_detector();
void read_file(char const log_file[], int is_binary = 1);
// void cleanup_phase_detector();

void test_listener(phase_id_type current_phase);


//used for reading from memtrace binary output files
struct Binary_output_struct_type {

    bool read_or_write;
    uint64_t virtual_address;
    uint64_t size_of_access;
    uint64_t instruction_pointer;

};

typedef struct Binary_output_struct_type binary_output_struct_t;



class phase_detector {
    private:
        bitvec current_signature;
        bitvec last_signature;

        hash<bitvec> hash_bitvec;

        uint64_t instruction_count = 0;
        uint64_t stable_count = 0;
        uint64_t stable_min = 5;
        phase_id_type phase = -1;

        vector<bitvec> phase_table;
        
        //phase trace?? should it be deque/stack or vector/arraylist?

        // vector<phase_id_type> phase_trace;
        deque<phase_id_type> phase_trace;

        vector<listener_function> listeners;
    public:
        double difference_measure_of_signatures(bitvec sig1, bitvec sig2);
        uint64_t hash_address(bitvec sig);
        void detect(uint64_t instruction_pointer);
        void init_phase_detector();
        void cleanup_phase_detector(string log_file_name);
        void register_listeners(listener_function f);
        void print_log_file(string log_file_name);
        int print_log_file_true = 1;
};