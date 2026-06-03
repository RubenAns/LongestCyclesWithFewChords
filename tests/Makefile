CXX ?= g++
MODE ?= release

SRCDIR := ..
INCDIR := ../include

ifeq ($(MODE),debug)
CXXFLAGS ?= -std=c++17 -g  -I$(INCDIR) -Wall -Wextra
else
CXXFLAGS ?= -std=c++17   -I$(INCDIR) -Wall -Wextra -O3 -DNDEBUG
endif

SOURCES = $(SRCDIR)/ReadGraphMatrix.cpp $(SRCDIR)/ReadGraphBitset.cpp $(SRCDIR)/ComputeLongestCycleMatrix.cpp  $(SRCDIR)/CountChordsBitset.cpp  $(SRCDIR)/ComputeLongestCycleAndCountThem.cpp

EXES = test_hamiltonian$(EXE_EXT) test_longestpath$(EXE_EXT) test_compare$(EXE_EXT) test_count_chords$(EXE_EXT)

ifeq ($(OS),Windows_NT)
RM = del /Q /F
EXE_EXT = .exe
else
RM = rm -f
EXE_EXT =
endif

all: $(EXES)

test_hamiltonian$(EXE_EXT): test_hamiltonian.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_longestpath$(EXE_EXT): test_longestpath.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_compare$(EXE_EXT): test_compare.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_count_chords$(EXE_EXT): test_count_chords.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^
# Jorik's implementation is treated as an external blackbox executable.
# Build it separately if needed:
#   g++ -std=c++17 -O3 -o countLongestCycles_Jorik.exe countLongestCycles_Jorik.cpp
# Then run: test_compare.exe ./countLongestCycles_Jorik.exe

 
clean:
	$(RM) *.exe *.o || true

.PHONY: all clean
