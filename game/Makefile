PROGS = copter tviewer tprinter

CC	= g++
CFLAGS	= -Wall -pipe -pedantic -DDATESTAMP="\"`date +%Y-%m-%d`\""
#CFLAGS	= -Wall -pipe -pedantic -DDATESTAMP="\"`date +%Y-%m-%d`\"" -mcpu=i386 -static
LDFLAGS = 
BIN_DIR = bin
OBJ_DIR = obj
SHARED_LIBS =

BINS = $(addprefix $(BIN_DIR)/, $(PROGS))
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
__OBJS1 = file_io generic rel_coords
__OBJS2 = curses copter trail $(__OBJS1)
OBJS_COPTER   = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, game     $(__OBJS2)))
OBJS_TVIEWER  = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, tviewer  $(__OBJS2)))
OBJS_TPRINTER = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, tprinter $(__OBJS1)))

all:	static-notice make-dirs $(BINS)

static-notice:
	@echo "For building statically-linked binaries, uncomment Makefile's commented-out CFLAGS."

make-dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
.PHONY: make-dirs

clean:
	rm -fv *~ \#* core
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)
	rm -fv config.log config.status

$(OBJS): $(OBJ_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/copter: $(OBJS_COPTER)
	g++ $(CFLAGS) $(LDFLAGS) -o $@ $^ $(SHARED_LIBS) -lreadline -lncurses

$(BIN_DIR)/tviewer: $(OBJS_TVIEWER)
	g++ $(CFLAGS) $(LDFLAGS) -o $@ $^ $(SHARED_LIBS) -lreadline -lncurses

$(BIN_DIR)/tprinter: $(OBJS_TPRINTER)
	g++ $(CFLAGS) $(LDFLAGS) -o $@ $^ $(SHARED_LIBS) -lreadline -lncurses
