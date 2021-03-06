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
// Version 0.9

#include "phase_detector.hpp"
#include <thread>
#include <functional>

using namespace std;
using namespace phase_detector_constants;

static PhaseDetector detector_all;
static PhaseDetector detector_mem;
static PhaseDetector detector_cbr;


static phase_id_type old_phase = -2;
static uint64_t interval = 0;
static ofstream dram_phase_trace("dram_phase_trace.csv");

// static void (*f)(uint64_t, phase_id_type) ext_dram_listener_function;
vector<dram_listener_function> ext_dram_listeners;


void read_file(char const log_file[], bool is_binary /*= 1*/, int max_num_intervals = 1000000) {
    

    int is_ryan_simple_format = 1;

    if (is_ryan_simple_format == 1) {


        ifstream in_stream;
        in_stream.open(log_file);
        simple_ins_ref_t current;
        // uint64_t address_ip = 0;
        
        // auto detector_detect = std::bind(&PhaseDetector::detect, &detector, std::placeholders::_1);
        // auto detector_va_detect = std::bind(&PhaseDetector::detect, &detector_va, std::placeholders::_1);

        int interval_index = 0;
        int num_intervals = 0;

        while (in_stream.good()) {
            
            //IDEA: read in an array of structs or multiple structs or something
            in_stream.read((char*)&current, sizeof(simple_ins_ref_t));

            // std::thread t1(detector_detect, current.instruction_pointer);
            // std::thread t2(detector_va_detect, current.virtual_address);

            // t1.join();
            // t2.join();
            if (current.is_cond_branch != 0) {
                detector_cbr.detect((uint64_t)current.pc);
            }
            else {
                detector_cbr.increment_instruction_count();
            }
            if (current.is_memory_ref != 0) {
                detector_mem.detect((uint64_t)current.pc);
            }
            else {
                detector_mem.increment_instruction_count();
            }
            detector_all.detect((uint64_t)current.pc);
            interval_index++;
            if (interval_index % interval_len == 0) {
                num_intervals++;
                interval_index = 0;
            }
            if (num_intervals >= max_num_intervals) {
                break;
            }
        }

    } else if (is_binary) {
        // cout << "binary test!!!" << endl << endl;

        ifstream in_stream;
        in_stream.open(log_file);
        binary_output_x86_memtrace_struct_t current;
        // uint64_t address_ip = 0;
        
        // auto detector_detect = std::bind(&PhaseDetector::detect, &detector, std::placeholders::_1);
        // auto detector_va_detect = std::bind(&PhaseDetector::detect, &detector_va, std::placeholders::_1);

        
        while (in_stream.good()) {
            
            //IDEA: read in an array of structs or multiple structs or something
            in_stream.read((char*)&current, sizeof(binary_output_x86_memtrace_struct_t));

            // std::thread t1(detector_detect, current.instruction_pointer);
            // std::thread t2(detector_va_detect, current.virtual_address);

            // t1.join();
            // t2.join();
            // detector.detect(current.instruction_pointer);
            // detector_va.detect(current.virtual_address);

        }

    // } else {
    //     // FILE* in_file = fopen(log_file, "r");
    //     // int is_write, matched;
    //     // int *is_write = (int *) malloc(sizeof(int));
    //     char *is_write = (char *) malloc(sizeof(char));
    //     uint64_t *addr_ip = (uint64_t *) malloc(sizeof(uint64_t)), *addr2 = (uint64_t *) malloc(sizeof(uint64_t)), *data_size = (uint64_t *) malloc(sizeof(uint64_t));
    //     int start = 1;
    //     string line_string;
    //     // ifstream in_stream(log_file);
    //     ifstream in_stream;
    //     in_stream.open(log_file);
    //     while (getline(in_stream, line_string)) {
    //         // if 
    //         // int matched = fscanf(in_file, "%d %lu %lu", is_write, addr_ip, addr2);
    //         //use for old file format of unsigned ints
    //         // char address[14]
    //         int matched = sscanf(line_string.c_str(), "%lx,%c,%lu,%lx", addr_ip, is_write, data_size, addr2);
    //         //use for file format of hex addr, r/w, data size, data addr
    //         if (matched < 3) {
    //             // 4 if new format, 3 if old format
    //             if (start == 1) {
    //                 start = 0;
    //                 continue;
    //             } else {
    //                 cout << line_string << endl;
    //                 // matched = fscanf(in_file, "%s", )
    //                 printf("uh oh! only matched %d \n", matched);
    //             }
    //         } else {
    //             // cout << addr_ip << endl;
    //             // if (DEBUG) {
    //             //     printf("%lu \n", *addr_ip);
    //             // }
    //             detector.detect(*addr_ip);
    //         }
    //     }
    //     // detector.cleanup_phase_detector();
    //     free(is_write);
    //     free(addr_ip);
    //     free(addr2);
    //     free(data_size);
    //     // fstream in_file(log_file, ios_base::in);
    //     // while (!in_file.eof()) {
    //     //     in_file >> is_write;
    //     //     in_file >> addr_ip;
    //     //     in_file >> addr2;
    //     //     cout << addr_ip << endl;
    //     //     // bitvec inst_pointer_bitvec(addr_ip);
    //     //     phase_detector(addr_ip);
    //     // }
    }
    
}


int main(int argc, char const *argv[])
{
    if (argc == 1 or (argc == 2 and (string(argv[1]) == "-h" or string(argv[1]) == "--help"))) {
        cout << "Usage: ./phase [max num intervals] [OPTION] [FILE WITH LIST OF INPUT FILES]...\n" <<
        "Options are -o [OUTPUT FILE NAME] which outputs the phase trace to the file named\n" <<
        "and -h or --help which displays this usage message." <<
        "The file passed in after options should be a plain text file with each line containing\n" <<
        "the name of a file to be processed." <<
        "NOTE: Options are currently disabled." << endl;
        return 0;
    }

    //args format:
    // ./phase [0] -t (text, optional) [1] phase_trace_output_name [1 or 2] log_file_1 log_file_2 ...
    
    // PhaseDetector detector;
    detector_all.init_phase_detector(); //probably not needed
    // detector.register_listeners(dram_phase_trace_listener);

    detector_mem.init_phase_detector();
    detector_cbr.init_phase_detector();
    // detector_va.register_listeners(dram_phase_trace_listener);

    // detector.register_listeners(test_listener);
    // detector_va.register_listeners(test_listener);
    if (argc > 1) {
        bool is_binary = true;
        int arg_index = 2;
        // bool output_phase_trace = true;
        string phase_trace_output_name_all = "xsb_all_test.csv";
        string phase_trace_output_name_mem = "xsb_mem_test.csv";
        string phase_trace_output_name_cbr = "xsb_cbr_test.csv";
        // if ( string(argv[1]) == "-o") {
        //     output_phase_trace = true;
        //     phase_trace_output_name = string(argv[2]);
        //     arg_index = 3;
        // }
        // if ( string(argv[1]) == "-t") { //alternate: strcmp(argv[1], "hello") == 0
        //     is_binary = 0;
        //     arg_index = 3;
        // }
        // while (arg_index < argc) {
        //     read_file(argv[arg_index], is_binary);
        //     arg_index++;
        // }

        int max_num_intervals = 1;
        std::string arg = argv[1];
        try
        {
            std::size_t pos;
            int x = std::stoi(arg, &pos);
            if (pos < arg.size())
            {
                std::cerr << "Trailing characters after number: " << arg << '\n';
            }
            max_num_intervals = x;
        }
        catch (std::invalid_argument const &ex)
        {
            std::cerr << "Invalid number: " << arg << '\n';
        }
        catch (std::out_of_range const &ex)
        {
            std::cerr << "Number out of range: " << arg << '\n';
        }

        std::string input_files = argv[arg_index];
        ifstream input_file_list_stream(input_files);
        std::string line;
        //loop through the list of log files and call read_file on each one
        while (std::getline(input_file_list_stream, line)) {
            const char* log_file = line.c_str();
            read_file(log_file, is_binary, max_num_intervals);
        }


        detector_all.cleanup_phase_detector(phase_trace_output_name_all);
        detector_mem.cleanup_phase_detector(phase_trace_output_name_mem);
        detector_cbr.cleanup_phase_detector(phase_trace_output_name_cbr);
    }
    dram_phase_trace.close();
    return 0;
}


void test_listener(phase_id_type current_phase) {
    cout << current_phase << endl;
}

//TODO: use std::bind so i can pass self as a param and use instance methods instead of stuff

void dram_phase_trace_listener(phase_id_type new_phase) {
    if (new_phase != old_phase) {
        if (dram_phase_trace.good()) {
            int64_t adjustment = 0;  // (new_phase == -1) ? 0 : detector.stable_min * -1; //if we want to back label intervals
            for (const auto &i: ext_dram_listeners) {
                i(interval + adjustment, new_phase);
            }
            // cout << interval + adjustment << "," << old_phase << endl;
            dram_phase_trace << (interval + adjustment) << "," << new_phase << '\n';
        } else {
            cerr << "dram phase trace file not good anymore! on phase " << new_phase << endl;
        }
        old_phase = new_phase;
    }
    interval += 1;
}


void register_dram_trace_listener(dram_listener_function f) {
    ext_dram_listeners.push_back(f);
}