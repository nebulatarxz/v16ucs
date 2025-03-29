CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fpermissive -mconsole -Wl,-subsystem,console

SRCDIR = .
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/UntitledCatShooter

SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/UCS.cpp $(SRCDIR)/resource.o

OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(filter %.cpp, $(SOURCES)))

ifeq ($(OS), Windows_NT)
    INCLUDES = -I/mingw64/include -I/mingw64/include/SDL2
    LIBS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_gfx -lSDL2_image -static-libgcc -static-libstdc++
else
    INCLUDES = `sdl2-config --cflags`
    LIBS = -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_gfx -lSDL2_image -static-libgcc -static-libstdc++
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
