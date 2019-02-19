BIN = bin
BUILD = build
SRC = src

CC = gcc
CFLAGS = -Wall -o0
CUNIT = -lcunit
GCOV_COMP = -fprofile-arcs -ftest-coverage
GCOV_LINK = -lgcov

.PHONY: all clean

all: $(BIN)/main

################
# Object files #
################

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS)  $(GCOV_COMP) -c -o $@ $<


################
# Executables  #
################

$(BIN)/main: $(BUILD)/main.o $(BUILD)/servo.o $(BUILD)/mock.o
	$(CC) -o $@ $^ $(CUNIT) $(GCOV_LINK)

clean:
	rm -f $(BUILD)/*.o  $(BUILD)/*.gcno $(BIN)/*