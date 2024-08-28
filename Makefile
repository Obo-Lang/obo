CC = gcc

CFLAGS = -O2

SRC_DIR = src
SRC = main.c $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
	   
INCLUDES = -I "include"

OUT_DIR = out
OUT = $(OUT_DIR)/main.exe

all: $(OUT)

$(OUT): $(SRC)
	if not exist $(OUT_DIR) mkdir $(OUT_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT) $(SRC) $(LIBS)

clean:
	rm -f $(OUT)

rebuild: clean all
