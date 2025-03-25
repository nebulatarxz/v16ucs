# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Directories
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Target executable
TARGET = $(BINDIR)/vega16

# Source files
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/VEGA1.cpp

# Object files
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Include directories and libraries
ifeq ($(OS), Windows_NT)
    INCLUDES = -I/mingw64/include
    LIBS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++
else
    INCLUDES =
    LIBS = -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++
endif

# Create directories if they don't exist
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean
