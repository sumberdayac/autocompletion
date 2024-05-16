CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lwinmm
SRCDIR = src
OBJDIR = build
BINDIR = bin
EXECUTABLE = $(BINDIR)/autocomplete.exe

# Define the source files
SRC = $(wildcard $(SRCDIR)/main.c)
# Define the object files
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# Include dependency files
-include $(OBJ:.o=.d)

all: directories $(EXECUTABLE)

directories:
	mkdir -p $(OBJDIR) $(BINDIR) 
	
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $(OBJ) $(OBJ:.o=.d)
	rm -rf build


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<


.PHONY: all clean directories