CC = gcc
OUTPUT = build
NAME = gbtime
EXE = $(OUTPUT)/$(NAME).exe

BACKEND-SRC = $(basename $(wildcard source/emulator/*.c) $(wildcard source/ESGL/*.c))
BACKEND-OBJ = $(addprefix $(OUTPUT)/,$(addsuffix .o,$(notdir $(BACKEND-SRC))))
BACKEND = $(OUTPUT)/libGamma.a

SRC = source/main.c
FLAGS = -O2 -std=c89
LIBS = $(BACKEND) -lopengl32 -lgdi32 -lShcore
INCLUDE = -I"include"


all: $(OUTPUT) $(BACKEND) $(EXE) run

run: $(EXE)
	./$(EXE)

clean:
	rm $(OUTPUT)/**

$(EXE): $(SRC) $(BACKEND)
	$(CC) $(FLAGS) $(INCLUDE) $(SRC) $(LIBS) -o $@

$(BACKEND): $(BACKEND-OBJ)
	$(AR) -rcs $@ $^
	@strip --strip-unneeded $@

$(OUTPUT)/%.o: source/emulator/%.c
	$(CC) $(FLAGS) $(INCLUDE) $(BACKEND-LIBS) $^ -c -o $@

$(OUTPUT)/%.o: source/esgl/%.c
	$(CC) $(FLAGS) $(INCLUDE) $(BACKEND-LIBS) $^ -c -o $@

$(OUTPUT):
	mkdir $(OUTPUT)