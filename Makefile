CXX ?= g++
MODE ?= release

BUILDDIR ?= build

ifeq ($(MODE),debug)
CXXFLAGS ?= -std=c++17 -g  -Wall -Wextra -Iinclude
else
CXXFLAGS ?= -std=c++17  -Wall -Wextra -Iinclude -g -O3 -DNDEBUG
endif

ifeq ($(OS),Windows_NT)
RM = del /Q /F
MKDIR = if not exist "$(BUILDDIR)" mkdir "$(BUILDDIR)"
EXE_EXT = .exe
else
RM = rm
MKDIR = mkdir -p "$(BUILDDIR)"
EXE_EXT =
endif

.PHONY: all clean help
all:   find_min_chord_cycle$(EXE_EXT) find_min_chord_cycle_with_ham_filter$(EXE_EXT)

# Source lists (in src/)
SRCS_MATRIX = ComputeLongestCycleMatrix.cpp ReadGraphMatrix.cpp
SRCS_BITSET = ComputeLongestCycleBitset.cpp ReadGraphBitset.cpp

# Derived object lists in build/
OBJS_MATRIX = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS_MATRIX))
OBJS_BITSET = $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS_BITSET))


# Ensure build directory exists then compile object files into build/
$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR):
	@$(MKDIR)


# Find minimum chord cycle
find_min_chord_cycle$(EXE_EXT): find_min_chord_cycle.cpp $(BUILDDIR)/ComputeLongestCycleWithFewestChords.o $(BUILDDIR)/CountChordsBitset.o $(BUILDDIR)/ReadGraphBitset.o
	$(CXX) $(CXXFLAGS) -o $@ $^

find_min_chord_cycle_with_ham_filter$(EXE_EXT): find_min_chord_cycle_with_ham_filter.cpp $(BUILDDIR)/ComputeLongestCycleWithFewestChords_with_ham_filter.o $(BUILDDIR)/CountChordsBitset.o $(BUILDDIR)/ReadGraphBitset.o
	$(CXX) $(CXXFLAGS) -o $@ $^


# Clean
clean:
	$(RM) $(BUILDDIR)/*.o 

help:
	@echo "Usage: make [target] [MODE=release|debug]"
	@echo
	@echo "Targets:"
	@echo "  all (default)       - build main and mainB"
	@echo "  find_min_chord_cycle - build longest cycle with fewest chords finder"
	@echo "  find_min_chord_cycle_with_ham_filter - build longest cycle with fewest chords finder with hamiltonian graphs filter"
	@echo "  clean               - remove artifacts"
	@echo
	@echo "Examples:"
	@echo "  make               # build optimized release binaries"
	@echo "  make MODE=debug    # build with debug symbols"
