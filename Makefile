CC = gcc

CFLAGS = -O2

SRC_DIR = src
SRC = main.c src/types/primitive/numeric/real.c src/types/primitive/numeric/number.c src/types/composite/numeric/complex.c src/types/composite/mapping/dict.c
INCLUDES = -I "include"

OUT_DIR = out
OUT = $(OUT_DIR)/main.exe

all: $(OUT)

$(OUT): $(SRC)
	mkdir $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT) $(SRC) $(LIBS)

clean:
	rm -f $(OUT)

rebuild: clean all
