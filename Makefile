CC = gcc
OUTPUT = build
NAME = gbtime
EXE = $(OUTPUT)/$(NAME).exe

BACKEND-SRC = $(basename $(wildcard source/emulator/*.c))
BACKEND-OBJ = $(addprefix $(OUTPUT)/,$(addsuffix .o,$(notdir $(BACKEND-SRC))))
BACKEND = $(OUTPUT)/libGamma.a

SRC = source/main.c
FLAGS = -O2 -std=c89
LIBS = $(BACKEND)
INCLUDE = -I"include"


all: $(OUTPUT) $(BACKEND) $(EXE) run

run: $(EXE)
	./$(EXE)

clean:
	rm $(OUTPUT)/**

$(EXE): $(SRC)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC) $(LIBS) -o $@

$(BACKEND): $(BACKEND-OBJ)
	$(AR) -rcs $@ $^
	@strip --strip-unneeded $@

$(OUTPUT)/%.o: source/emulator/%.c
	$(CC) $(FLAGS) $(INCLUDE) $^ -c -o $@

$(OUTPUT):
	mkdir $(OUTPUT)