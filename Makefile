CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRCDIR = .
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/vega16

SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/VEGA1.cpp

OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

ifeq ($(OS), Windows_NT)
    INCLUDES = -I/mingw64/include
    LIBS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++
else
    INCLUDES =
    LIBS = -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++
endif

$(shell mkdir -p $(OBJDIR) $(BINDIR))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean
