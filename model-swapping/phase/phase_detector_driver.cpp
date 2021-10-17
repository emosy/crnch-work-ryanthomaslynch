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
// Version 0.8

#include "phase_detector.h"

using namespace std;

static phase_detector detector;


static phase_id_type old_phase = -1;
static uint64_t interval = 0;
static ofstream dram_phase_trace("dram_phase_trace.csv");


void read_file(char const log_file[], int is_binary /*= 1*/) {
    
    if (is_binary == 1) {
        // cout << "binary test!!!" << endl << endl;

        ifstream in_stream;
        in_stream.open(log_file);
        binary_output_struct_t current;
        // uint64_t address_ip = 0;
        
        while (in_stream.good()) {
            // in_stream.ignore(3 * sizeof(uint64_t));
            // in_stream.read((char*)&address_ip, sizeof(address_ip));

            // if (DEBUG) {
            //     cout << address_ip << endl;
            // } else {
            //     detector.detect(address_ip);
            // }

            //time for ignore on amg 50: 13.889s
            //time for struct on amg 50: 10.726s
            //time for struct with inlined fxns on amg 50: 
            
            in_stream.read((char*)&current, sizeof(binary_output_struct_t));
            detector.detect(current.instruction_pointer);

        }

    } else {
        // FILE* in_file = fopen(log_file, "r");
        // int is_write, matched;

        // int *is_write = (int *) malloc(sizeof(int));
        char *is_write = (char *) malloc(sizeof(char));
        uint64_t *addr_ip = (uint64_t *) malloc(sizeof(uint64_t)), *addr2 = (uint64_t *) malloc(sizeof(uint64_t)), *data_size = (uint64_t *) malloc(sizeof(uint64_t));

    
        int start = 1;

        string line_string;
        // ifstream in_stream(log_file);
        ifstream in_stream;
        in_stream.open(log_file);

        while (getline(in_stream, line_string)) {
            // if 
            // int matched = fscanf(in_file, "%d %lu %lu", is_write, addr_ip, addr2);
            //use for old file format of unsigned ints
            // char address[14]
            int matched = sscanf(line_string.c_str(), "%lx,%c,%lu,%lx", addr_ip, is_write, data_size, addr2);
            //use for file format of hex addr, r/w, data size, data addr
            if (matched < 3) {
                // 4 if new format, 3 if old format
                if (start == 1) {
                    start = 0;
                    continue;
                } else {
                    cout << line_string << endl;
                    // matched = fscanf(in_file, "%s", )
                    printf("uh oh! only matched %d \n", matched);
                }
            } else {
                // cout << addr_ip << endl;
                // if (DEBUG) {
                //     printf("%lu \n", *addr_ip);
                // }
                detector.detect(*addr_ip);
            }
        
            
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
    
}


int main(int argc, char const *argv[])
{
    if (argc == 1 or (argc == 2 and (string(argv[1]) == "-h" or string(argv[1]) == "--help"))) {
        cout << "Usage: ./phase [OPTION] [OUTPUT FILE] [INPUT FILES]...\n" <<
        "Options are -t which changes input mode from binary to text (from libmemtrace)\n" <<
        "and -h or --help which displays this usage message." << endl;
        return 0;
    }

    //args format:
    // ./phase [0] -t (text, optional) [1] phase_trace_output_name [1 or 2] log_file_1 log_file_2 ...


    // phase_detector detector;
    detector.init_phase_detector(); //probably not needed
    detector.register_listeners(dram_phase_trace_listener);
    if (argc > 1) {
        int is_binary = 1;
        int arg_index = 2;
        if ( string(argv[1]) == "-t") { //alternate: strcmp(argv[1], "hello") == 0
            is_binary = 0;
            arg_index = 3;
        }
        string phase_trace_output_name(argv[arg_index - 1]);
        while (arg_index < argc) {
            read_file(argv[arg_index], is_binary);
            arg_index++;
        }
        // string log_file(argv[2]);
        detector.cleanup_phase_detector(phase_trace_output_name);
    } else {

        //for debugging purposes :PPP

        // string log_file = "stream.ssv";
        // read_file("stream.ssv");
        //test listeners
        // detector.register_listeners(test_listener);
        read_file("xsbench-g10-p500-1.log");
        read_file("xsbench-g10-p500-2.log");
        read_file("xsbench-g10-p500-3.log");
        // detector.cleanup_phase_detector(); //probably not needed
        // detector.small_or_medium = 1;
        // read_file("meabo.medium.txt");
        // detector.cleanup_phase_detector("phase_trace_xsbench.txt");
        detector.print_log_file("phase_trace_xsbench.csv");
    }
    // cout << log_file << endl;
    dram_phase_trace.close();
    return 0;
}


void test_listener(phase_id_type current_phase) {
    cout << current_phase << endl;
}


void dram_phase_trace_listener(phase_id_type new_phase) {
    if (new_phase != old_phase) {
        if (dram_phase_trace.good()) {
            if (old_phase != -1) {
                // cout << interval * interval_len << "," << old_phase << endl;
                dram_phase_trace << interval * interval_len << "," << old_phase << '\n';
            }
        } else {
            cerr << "dram phase trace file not good anymore! on phase " << new_phase << endl;
        }
        old_phase = new_phase;
    }
    interval += 1;
}