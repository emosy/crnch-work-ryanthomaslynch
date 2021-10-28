# ONLY use this makefile if you do NOT have a cmake 3.0+ version

CC=gcc
CXX=g++

FMT_LIB_DIR=../DRAMsim3/ext/fmt/include
INI_LIB_DIR=../DRAMsim3/ext/headers
JSON_LIB_DIR=../DRAMsim3/ext/headers
ARGS_LIB_DIR=../DRAMsim3/ext/headers

INC=-Isrc/ -I$(FMT_LIB_DIR) -I$(INI_LIB_DIR) -I$(ARGS_LIB_DIR) -I$(JSON_LIB_DIR)
CXXFLAGS=-Wall -O3 -fPIC -std=c++11 $(INC) -DFMT_HEADER_ONLY=1

LIB_NAME=libdramsim3.so
EXE_NAME=phase_test.out

SRCS = ../DRAMsim3/src/bankstate.cc ../DRAMsim3/src/channel_state.cc ../DRAMsim3/src/command_queue.cc \
		../DRAMsim3/src/common.cc ../DRAMsim3/src/configuration.cc ../DRAMsim3/src/controller.cc \
		../DRAMsim3/src/dram_system.cc ../DRAMsim3/src/hmc.cc ../DRAMsim3/src/memory_system.cc \
		../DRAMsim3/src/refresh.cc ../DRAMsim3/src/simple_stats.cc ../DRAMsim3/src/timing.cc \
		../phase/phase_detector.cpp

EXE_SRCS = ../DRAMsim3/src/cpu.cc phase_test_driver.cpp

OBJECTS = $(addsuffix .o, $(basename $(SRCS)))
EXE_OBJS = $(addsuffix .o, $(basename $(EXE_SRCS)))
EXE_OBJS := $(EXE_OBJS) $(OBJECTS)


all: $(LIB_NAME) $(EXE_NAME)

$(EXE_NAME): $(EXE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(LIB_NAME): $(OBJECTS)
	$(CXX) -g -shared -Wl,-soname,$@ -o $@ $^

%.o : %.cc
	$(CXX)  $(CXXFLAGS) -o $@ -c $<

%.o : %.c
	$(CC) -fPIC -O2 -o $@ -c $<

clean:
	-rm -f $(EXE_OBJS) $(LIB_NAME) $(EXE_NAME)