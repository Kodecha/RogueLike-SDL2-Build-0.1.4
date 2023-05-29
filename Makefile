CC = g++
CFLAGS = -c -Wall
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SOURCES = src/main.cpp src/player.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

src/player.o: src/player.cpp
	$(CC) $(CFLAGS) $< -o $@

src/SettingsMenu.o: src/SettingsMenu.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)