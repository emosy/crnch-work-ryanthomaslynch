
CXX = g++
SRC = phase_detector.cpp phase_detector_driver.cpp
INCLUDE = phase_detector.h
CXXFLAGS := -Wall -Wextra -std=c++11 -O3 -pthread # -Werror
DEBUGFLAGS := -g -std=c++11 -O0 -Wall
#RELEASE := -O3 # Feel free to enable to help you test

build:
	$(CXX) $(RELEASE) $(CXXFLAGS) $(SRC) -o phase

debug:
	$(CXX) $(DEBUGFLAGS) $(SRC) -o phase

# clean:
# 	rm -f procsim *.o

# submit:
# 	tar -cvzf project3-submit.tar.gz procsim.cpp procsim_driver.cpp procsim.h Makefile

.PHONY: build debug clean submit