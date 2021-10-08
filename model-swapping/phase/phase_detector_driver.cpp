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
// Version 0.2

#include "phase_detector.h"

using namespace std;

static phase_detector detector;



void read_file(char const log_file[]) {
    FILE* in_file = fopen(log_file, "r");
    // int is_write, matched;

    // int *is_write = (int *) malloc(sizeof(int));
    char *is_write = (char *) malloc(sizeof(char));
    uint64_t *addr_ip = (uint64_t *) malloc(sizeof(uint64_t)), *addr2 = (uint64_t *) malloc(sizeof(uint64_t)), *data_size = (uint64_t *) malloc(sizeof(uint64_t));

    


    while (!feof(in_file)) {
        // if 
        // int matched = fscanf(in_file, "%d %lu %lu", is_write, addr_ip, addr2);
        //use for old file format of unsigned ints
        // char address[14]
        int matched = fscanf(in_file, "%lx,%c,%lu,%lx", addr_ip, is_write, data_size, addr2);
        //use for file format of hex addr, r/w, data size, data addr
        if (matched < 3) {
            // 4 if new format, 3 if old format
            printf("uh oh! only matched %d \n", matched);
        }
        // cout << addr_ip << endl;
        // if (DEBUG) {
        //     printf("%lu \n", *addr_ip);
        // }
        detector.detect(*addr_ip);
    }

    // detector.cleanup_phase_detector();

    free(is_write);
    free(addr_ip);
    free(addr2);
    free(data_size);

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
    // phase_detector detector;
    detector.init_phase_detector(); //probably not needed
    if (argc > 1) {
        // string log_file(argv[2]);
        read_file(argv[2]);
    } else {
        // string log_file = "stream.ssv";
        // read_file("stream.ssv");
        read_file("meabo.small.txt");
        detector.cleanup_phase_detector(); //probably not needed
        detector.small_or_medium = 1;
        read_file("meabo.medium.txt");
        detector.cleanup_phase_detector(); //probably not needed
    }
    // cout << log_file << endl;
    return 0;
}
