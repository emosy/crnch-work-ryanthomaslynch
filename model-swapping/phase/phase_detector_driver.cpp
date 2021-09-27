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


// Phase detection driver code
// Version 0.1

#include "phase_detector.h"

using namespace std;



void read_file(char const log_file[]) {
    FILE* in_file = fopen(log_file, "r");
    // int is_write, matched;

    int *is_write = (int *) malloc(sizeof(int));
    uint64_t *addr_ip = (uint64_t *) malloc(sizeof(uint64_t)), *addr2 = (uint64_t *) malloc(sizeof(uint64_t));

    while (!feof(in_file)) {
        int matched = fscanf(in_file, "%d %lu %lu", is_write, addr_ip, addr2);
        if (matched < 3) {
            printf("uh oh! only matched %d \n", matched);
        }
        // cout << addr_ip << endl;
        if (DEBUG) {
            printf("%lu \n", *addr_ip);
        }
        phase_detector(*addr_ip);
    }

    free(is_write);
    free(addr_ip);
    free(addr2);

    // fstream in_file(log_file, ios_base::in);

    

    // while (!in_file.eof()) {
    //     in_file >> is_write;
    //     in_file >> addr_ip;
    //     in_file >> addr2;

    //     cout << addr_ip << endl;

    //     // bitvec inst_pointer_bitvec(addr_ip);
    //     phase_detector(addr_ip);
    // }
}


int main(int argc, char const *argv[])
{
    init_phase_detector();
    if (argc > 1) {
        // string log_file(argv[2]);
        read_file(argv[2]);
    } else {
        // string log_file = "stream.ssv";
        read_file("stream.ssv");
    }
    // cout << log_file << endl;
    return 0;
}
