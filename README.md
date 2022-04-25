# Copyright (c) 2021, Georgia Institute of Technology
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


Ryan Thomas Lynch
Georgia Institute of Technology
ryan.lynch@gatech.edu
CRNCH Lab

Phase Detector for Online Model Swapping
How to build:

Run the "make" command in the directory model-swapping/phase with the provided Makefile. Run "make debug" to have debug symbols.

This will create an executable called "phase" in the directory. Run ./phase with no arguments or with the argument "-h" or "--help" to get usage information.
Current usage information as of v0.9:
Usage: ./phase [OPTION] [INPUT FILES]...
Options are -o [OUTPUT FILE NAME] which outputs the phase trace to the file named
and -h or --help which displays this usage message.

The phase program driver code expects inputs from the output logs of libmemtrace from DynamoRio run on whatever program you want to test.
The default format is binary format (there was support for text format, but that's a side issue).



TODO:
write instructions on how to build, make traces with dynamorio, and run phase
delete all the random unnecessary files
document output formats
save example plots - do example plots for bzip, gcc, etc like from simpoint
SPEC2017 simpoints are known



Good luck!

Pax,
Ryan

Go Jackets
