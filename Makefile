# Incs & Libs
INCS := -I/usr/X11R6/include -I/usr/include/freetype2 -Isrc/
LIBS := -L/usr/X11R6/lib -lX11 -lfontconfig -lXft -lXrender

# Configuration
VERSION := 0.0.1
PROJ    := owm
CC      := clang

DEBUG   := -fsanitize=address -g -O0
RELEASE := -O3 -Os

DEF    := -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700L
CFLAGS := $(DEF) $(INCS) -std=c99 -pedantic -Wall -Wextra -Wno-deprecated-declarations -Werror 

# Build-mode
BUILD  := build
MODE   ?= debug
ifeq ($(MODE),release)
    CFLAGS += $(RELEASE)
    BUILD   := build/release
else
    CFLAGS += $(DEBUG)
    BUILD   := build/debug
endif

# paths
PREFIX    := /usr/local
MANPREFIX := ${PREFIX}/share/man

# Source files
C_SOURCES := $(wildcard src/*.c) $(wildcard src/*/*.c)
H_HEADERS := $(wildcard src/*.h) $(wildcard src/*/*.h)
SRCFILES  := $(C_SOURCES) $(H_HEADERS)
# Object files
OBJECTS := $(patsubst src/%.c, $(BUILD)/%.o, $(C_SOURCES))

all: format $(PROJ)

# Link all objects
$(PROJ): $(OBJECTS)
	@echo "Linking $(PROJ) ($(MODE) mode)..."
	@$(CC) $(OBJECTS) -o $(PROJ) $(CFLAGS) $(LIBS)

# Build all source files
$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean build
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf build $(PROJ)

# Format c files
format:
	@echo "Formatting source files..."
	@clang-format -i $(SRCFILES)

# Install
install: clean $(PROJ)
	@echo "Installing $(PROJ)..."
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f $(PROJ) ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/$(PROJ)
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < res/$(PROJ).1 > ${DESTDIR}${MANPREFIX}/man1/$(PROJ).1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/$(PROJ).1

# Uninstall
uninstall:
	@rm -f ${DESTDIR}${MANPREFIX}/man1/$(PROJ).1
	@rm -f ${DESTDIR}${PREFIX}/bin/$(PROJ)

.PHONY: all clean format install uninstall
