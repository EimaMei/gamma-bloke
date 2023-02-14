CC = gcc
OUTPUT = build
NAME = gbtime
EXE = $(OUTPUT)/$(NAME).exe

all: $(OUTPUT) $(EXE) run

SRC = source/main.c
FLAGS = -O2 -std=c89
LIBS =
INCLUDE = -I"include"

%.hpp: $(EXE)

$(EXE): $(SRC)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC) $(LIBS) -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm $(build)/**

$(OUTPUT):
	mkdir $(OUTPUT)