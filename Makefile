CPP      = g++
OBJ1     = lsh.o
OBJ2	 = cube.o
BIN1     = lsh
BIN2	 = cube
CFLAGS   = -Wall -g

all: $(BIN1) $(BIN2)

$(BIN1): $(OBJ1)
	$(CPP) $(OBJ1) -o $(BIN1) $(CFLAGS)

$(BIN2): $(OBJ2)
	$(CPP) $(OBJ2) -o $(BIN2) $(CFLAGS)

lsh.o:	lsh.cpp
	$(CPP) -c lsh.cpp $(CFLAGS)

cube.o: cube.cpp
	$(CPP) -c cube.cpp $(CFLAGS)

PHONY: clean

clean: 
	rm -f $(OBJ1) $(BIN1) $(OBJ2) $(BIN2)
