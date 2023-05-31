CC = g++
CFLAGS = -c -Wall
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)