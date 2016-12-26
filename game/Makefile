PROGS = copter tviewer tprinter

CXX    ?= g++
CFLAGS	= -Wall -pipe -pedantic -DDATESTAMP="\"`date +%Y-%m-%d`\""
ifeq ($(STATIC),1)
LDFLAGS += -static
endif
LIBS = -lreadline -lncurses
BIN_DIR = bin
OBJ_DIR = obj

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
	@echo "For building statically-linked binaries, make with STATIC=1"

make-dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
.PHONY: make-dirs

clean:
	$(RM) -v *~ \#* core
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)
	$(RM) -v config.log config.status

$(OBJS): $(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/copter: $(OBJS_COPTER)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BIN_DIR)/tviewer: $(OBJS_TVIEWER)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
$(BIN_DIR)/tprinter: $(OBJS_TPRINTER)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
