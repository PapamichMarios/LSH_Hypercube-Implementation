CPP      = g++
OBJ1     = fi.o lsh.o hyperplane.o g.o
OBJ2	 = cube.o 
BIN1     = lsh
BIN2	 = cube
CFLAGS   = -Wall -g -std=c++11

all: $(BIN1) $(BIN2)

$(BIN1): $(OBJ1)
	$(CPP) $(OBJ1) -o $(BIN1) $(CFLAGS)

$(BIN2): $(OBJ2)
	$(CPP) $(OBJ2) -o $(BIN2) $(CFLAGS)

lsh.o:	lsh.cpp
	$(CPP) -c lsh.cpp $(CFLAGS)

cube.o: cube.cpp
	$(CPP) -c cube.cpp $(CFLAGS)

hyperplane.o: hyperplane.cpp
	$(CPP) -c hyperplane.cpp $(CFLAGS)

fi.o: fi.cpp
	$(CPP) -c fi.cpp $(CFLAGS)

gi.o: gi.cpp
	$(CPP) -c g.cpp $(CFLAGS)

PHONY: clean

clean: 
	rm -f $(OBJ1) $(BIN1) $(OBJ2) $(BIN2)
