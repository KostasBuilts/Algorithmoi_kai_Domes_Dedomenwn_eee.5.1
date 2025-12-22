# Makefile for Gravity Simulation with MathGL

CC = gcc
CFLAGS = -Wall -O2 -fopenmp
LDFLAGS = -lm -lmgl -fopenmp
TARGET = gravity_sim

# MathGL paths (adjust as needed)
# For Ubuntu: sudo apt-get install libmgl-dev
# For Fedora: sudo dnf install mathgl-devel
MGL_INCLUDE = -I/usr/include
MGL_LIB = -L/usr/lib

SRC = main.c physics.c simulation.c visualization.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(MGL_LIB) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(MGL_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) *.png *.csv *.txt

run: $(TARGET)
	./$(TARGET)

# Installation check
check-mathgl:
	@echo "Checking for MathGL installation..."
	@pkg-config --exists libmgl || echo "MathGL not found. Please install:"
	@echo "  Ubuntu: sudo apt-get install libmgl-dev"
	@echo "  Fedora: sudo dnf install mathgl-devel"
	@echo "  Mac:    brew install mathgl"
	@echo "  Or download from: http://mathgl.sourceforge.net/"

.PHONY: all clean run check-mathgl