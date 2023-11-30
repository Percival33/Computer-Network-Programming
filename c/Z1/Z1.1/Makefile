CC = gcc
CFLAGS = -std=c11 -I../..
LDFLAGS =
OUTPUT_DIR = ./output
COMMON_DIR = ../..
COMMON_SRC = $(COMMON_DIR)/common.c
COMMON_OBJ = $(COMMON_DIR)/common.o

.PHONY: all clean

all: $(OUTPUT_DIR)/server $(OUTPUT_DIR)/client

$(COMMON_OBJ): $(COMMON_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT_DIR)/server: src/server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUTPUT_DIR)/client: src/client.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OUTPUT_DIR)/server $(OUTPUT_DIR)/client $(COMMON_OBJ)
