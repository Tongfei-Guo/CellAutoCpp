
.PHONY: debug release clean

CC = g++

INCLUDES = -I ../Source

CFLAGS_BASIC = -std=c++14 -lpthread

release : CFLAGS = $(CFLAGS_BASIC) -O3
debug : CFLAGS = $(CFLAGS_BASIC) -g -Wall

_CA = CAWorld.cpp Cell.cpp Model.cpp CAFunctions.cpp CAConsoleViz.cpp
CADIR = ../Source
CASRC = $(patsubst %,$(CADIR)/%,$(_CA))

_CAOBJ = $(_CA:.cpp=.o)
OBJDIR = obj
CAOBJ = $(patsubst %,$(OBJDIR)/%,$(_CAOBJ))

_SRC = cave_viz.cpp forestfire_viz.cpp fractal_viz.cpp gameoflife_viz.cpp ising_viz.cpp lava_viz.cpp maze_viz.cpp splashes_viz.cpp watercave_viz.cpp
EXEDIR = exe
_EXE = $(_SRC:.cpp=)
EXE = $(patsubst %,$(EXEDIR)/%,$(_EXE))

debug release: $(EXE)
	@echo $@
	@echo ======= compiled version: $@ ========

$(EXEDIR)/%: %.cpp $(CAOBJ)
	mkdir -p $(EXEDIR)
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ $< $(CAOBJ)

$(OBJDIR)/%.o: $(CADIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJDIR)/*.o $(EXEDIR)/*
	rmdir $(EXEDIR) $(OBJDIR)
