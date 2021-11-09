# ONLY use this makefile if you do NOT have a cmake 3.0+ version

CC=gcc
CXX=g++

FMT_LIB_DIR=../DRAMsim3/ext/fmt/include
INI_LIB_DIR=../DRAMsim3/ext/headers
JSON_LIB_DIR=../DRAMsim3/ext/headers
ARGS_LIB_DIR=../DRAMsim3/ext/headers
PHASE_LIB_DIR=../phase

INC=-I../DRAMsim3/src/ -I$(FMT_LIB_DIR) -I$(INI_LIB_DIR) -I$(ARGS_LIB_DIR) -I$(JSON_LIB_DIR) -I$(PHASE_LIB_DIR)
CXXFLAGS=-Wall $(OLEVEL) -fPIC -std=c++11 $(INC) -DFMT_HEADER_ONLY=1

# CXXFLAGS_DEBUG = -Wall -ggdb -g -O0 -fPIC -std=c++11 $(INC) -DFMT_HEADER_ONLY=1

LIB_NAME=libdramsim3.so
EXE_NAME=phase_test.out

SRCS = ../DRAMsim3/src/bankstate.cc ../DRAMsim3/src/channel_state.cc ../DRAMsim3/src/command_queue.cc \
		../DRAMsim3/src/common.cc ../DRAMsim3/src/configuration.cc ../DRAMsim3/src/controller.cc \
		../DRAMsim3/src/dram_system.cc ../DRAMsim3/src/hmc.cc ../DRAMsim3/src/memory_system.cc \
		../DRAMsim3/src/refresh.cc ../DRAMsim3/src/simple_stats.cc ../DRAMsim3/src/timing.cc \
		../phase/phase_detector.cpp ../DRAMsim3/src/cpu.cc

EXE_SRCS = phase_test_driver.cpp

OBJECTS = $(addsuffix .o, $(basename $(SRCS)))
EXE_OBJS = $(addsuffix .o, $(basename $(EXE_SRCS)))
EXE_OBJS := $(EXE_OBJS) $(OBJECTS)


all: $(LIB_NAME) $(EXE_NAME)
OLEVEL = -O3

debug-O0: all
debug-O0: OLEVEL = -ggdb -g -O0

debug-O1: all
debug-O1: OLEVEL = -ggdb -g -O1

debug-02: all
debug-02: OLEVEL = -ggdb -g -O2

debug-03: all
debug-03: OLEVEL = -ggdb -g -O3

$(EXE_NAME): $(EXE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LIB_NAME): $(OBJECTS)
	$(CXX) $(OLEVEL) -shared -Wl,-v,-soname,$@ -o $@ $^

%.o : %.cc
	$(CXX)  $(CXXFLAGS) $(OLEVEL) -o $@ -c $<

%.o : %.cpp
	$(CXX)  $(CXXFLAGS) $(OLEVEL) -o $@ -c $<

%.o : %.c
	$(CC) $(OLEVEL) -fPIC o $@ -c $<

clean:
	-rm -f $(EXE_OBJS) $(LIB_NAME) $(EXE_NAME)
	
# debug: $(LIB_NAME_DEBUG) $(EXE_NAME_DEBUG)

# $(EXE_NAME_DEBUG): $(EXE_OBJS)
# 	$(CXX) $(CXXFLAGS_DEBUG) -o $@ $^
	
# $(LIB_NAME_DEBUG): $(OBJECTS)
# 	$(CXX) -g -shared -Wl,-soname,$@ -o $@ $^