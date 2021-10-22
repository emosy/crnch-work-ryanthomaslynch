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

#include <iostream>
#include "./../DRAMsim3/ext/headers/args.hxx"
#include "./../DRAMsim3/src/cpu.h"
#include <fstream>
#include <cstdio>
#include "./../phase/phase_detector.h"
#include <sstream>
#include <string>


using namespace dramsim3;

// static void dram_phase_trace_listener(int new_phase);
static OnlineCPU *cpu;


static int old_phase = -2;
static uint64_t interval = 0;
static std::ofstream dram_phase_trace;

void dram_phase_trace_listener(int new_phase) {
    if (new_phase != old_phase) {
        if (dram_phase_trace.good()) {

            //TODO: need to get the stats for the current phase here to put in the output file all together

            int64_t adjustment = 0;  // (new_phase == -1) ? 0 : detector.stable_min * -1; //if we want to back label intervals
            // for (const auto &i: ext_dram_listeners) {
            //     i(interval + adjustment, new_phase);
            // }
            // cout << interval + adjustment << "," << old_phase << endl;
            dram_phase_trace << (interval + adjustment) << "," << new_phase << '\n';
        } else {
            std::cerr << "dram phase trace file not good anymore! on phase " << new_phase << std::endl;
        }
        old_phase = new_phase;
        cpu->ResetStats();
    }
    interval += 1;
}



int main(int argc, const char **argv) {

    dram_phase_trace.open("dram_phase_trace.csv");

    
    // using namespace std;
    args::ArgumentParser parser(
        "DRAM Simulator.",
        "Examples: \n."
        "./build/dramsim3main configs/DDR4_8Gb_x8_3200.ini -c 100 -t "
        "sample_trace.txt\n"
        "./build/dramsim3main configs/DDR4_8Gb_x8_3200.ini -s random -c 100");
    args::HelpFlag help(parser, "help", "Display the help menu", {'h', "help"});
    args::ValueFlag<uint64_t> num_cycles_arg(parser, "num_cycles",
                                             "Number of cycles to simulate",
                                             {'c', "cycles"}, 100000);
    args::ValueFlag<std::string> output_dir_arg(
        parser, "output_dir", "Output directory for stats files",
        {'o', "output-dir"}, ".");
    args::ValueFlag<std::string> input_files_arg(
    parser, "input_files", "File containing list of input files (one on each line)",
    {'i', "input-files"}, "input.txt");
    args::Positional<std::string> config_arg(
        parser, "config", "The config file name (mandatory)");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::string config_file = args::get(config_arg);
    if (config_file.empty()) {
        std::cerr << parser;
        return 1;
    }

    uint64_t cycles = args::get(num_cycles_arg);
    std::string output_dir = args::get(output_dir_arg);
    // std::string output_dir = std::string("dram_sim_test.txt");
    std::string input_files = args::get(input_files_arg);



    // if (stream_type == "stream" || stream_type == "s") {
    //     cpu = new StreamCPU(config_file, output_dir);
    // } else {
    //     cpu = new RandomCPU(config_file, output_dir);
    // }
    cpu = new OnlineCPU(config_file, output_dir);
    
    //phase detector setup    

    PhaseDetector detector;
    detector.init_phase_detector();

    ifstream input_file_list_stream(input_files);
    std::string line;
    uint64_t fake_clock = 0;


    detector.register_listeners(dram_phase_trace_listener);

    while (std::getline(input_file_list_stream, line)) {
        //line is the name of the current file to read from
        ifstream in_stream;
        in_stream.open(line);
        binary_output_struct_t current;
        while (in_stream.good()) {
            in_stream.read((char*)&current, sizeof(binary_output_struct_t));
            detector.detect(current.instruction_pointer);
            while (true) {
                bool canSend = cpu->canSendTransaction(current.virtual_address, current.is_write);
                cpu->ClockTick();
                fake_clock++;
                if (canSend) {
                    break;
                }
            }
        }
        in_stream.close();

    }


    for (uint64_t clk = 0; clk < cycles; clk++) {
        
    }
    cpu->PrintStats();

    delete cpu;

    dram_phase_trace.close();

    return 0;
}


