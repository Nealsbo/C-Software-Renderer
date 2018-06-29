#C-Software-Renderer Makefile

CC=gcc
CFLAGS=-c -std=c99 -w -lm -lSDL2 -lSDL2main
LDFLAGS=-lm -lSDL2

BUILD=build

SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:%=$(BUILD)/%.o)

EXEC=$(BUILD)/Renderer

all: $(EXEC)
	./$(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD)/%.o: %
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -c -MMD -MP -o $@

-include $(OBJECTS:.o=.d)

clean:
	rm -rf $(BUILD)

.PHONY: all clean

