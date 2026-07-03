CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
CFLAGS := -Wall -Wextra -Wshadow -Werror -Wpedantic -std=c11 -Iinclude -O2
TARGET := $(BINDIR)/corbit
SRCEXT := c

SOURCES := $(wildcard $(SRCDIR)/*.$(SRCEXT)) $(wildcard $(SRCDIR)/**/*.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
INC := -I include

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -lncurses -lm

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean
