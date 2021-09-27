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
// Version 0.1


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
#define DEBUG 0


using bitvec = bitset<signature_len>;

typedef int phase_t;


double difference_measure_of_signatures(bitvec sig1, bitvec sig2);
uint64_t hash_address(bitvec sig);
void phase_detector(uint64_t instruction_pointer);
void init_phase_detector();
void read_file(char const log_file[]);