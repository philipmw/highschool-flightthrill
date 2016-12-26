PROGS = copter tviewer tprinter

CC	= g++
CFLAGS	= -Wall -pipe -pedantic -DDATESTAMP="\"`date +%Y-%m-%d`\""
#CFLAGS	= -Wall -pipe -pedantic -DDATESTAMP="\"`date +%Y-%m-%d`\"" -mcpu=i386 -static
LDFLAGS = 
BIN_DIR = bin
OBJ_DIR = obj
SHARED_LIBS =

all:	static-notice make-dirs $(PROGS)

static-notice:
	@echo "For building statically-linked binaries, uncomment Makefile's commented-out CFLAGS."

make-dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
.PHONY: make-dirs

clean:
	rm -fv *~ \#* core
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)
	rm -fv config.log config.status

game.o: game.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

rel_coords.o: rel_coords.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

curses.o: curses.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

file_io.o: file_io.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<
	
copter.o: copter.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

trail.o: trail.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

generic.o: generic.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

tviewer.o: tviewer.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

tprinter.o: tprinter.cpp
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $<

copter: game.o curses.o file_io.o copter.o trail.o generic.o rel_coords.o
	g++ $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $(OBJ_DIR)/game.o $(OBJ_DIR)/curses.o $(OBJ_DIR)/file_io.o $(OBJ_DIR)/copter.o $(OBJ_DIR)/trail.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/rel_coords.o $(SHARED_LIBS) -lreadline -lncurses

tviewer: tviewer.o file_io.o generic.o trail.o curses.o copter.o rel_coords.o
	g++ $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $(OBJ_DIR)/tviewer.o $(OBJ_DIR)/file_io.o $(OBJ_DIR)/trail.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/curses.o $(OBJ_DIR)/copter.o $(OBJ_DIR)/rel_coords.o $(SHARED_LIBS) -lreadline -lncurses

tprinter: tprinter.o file_io.o generic.o rel_coords.o
	g++ $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $(OBJ_DIR)/tprinter.o $(OBJ_DIR)/file_io.o $(OBJ_DIR)/generic.o $(OBJ_DIR)/rel_coords.o $(SHARED_LIBS) -lreadline -lncurses
