CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
CFLAGS := -std=c99 -O2 -Wall -Wextra -Wshadow -Werror
TARGET := $(BINDIR)/corbit
SRCEXT := c

SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
INC := -I include

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lncurses -lm

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean
