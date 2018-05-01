CC = g++

INCLUDES = -I depend/glm-0.9.9-a2 -I depend/glad -I ./render -I ../Source 

CMPORDER = -march=native -O3


_CA = CAWorld.cpp Cell.cpp Model.cpp CAFunctions.cpp
_CADIR = ../Source
CA = $(patsubst %,$(_CADIR)/%,$(_CA))
CAOBJ = $(CA:.cpp=.o)


RENDDEPEN = -lglfw -lGL -ldl -lpthread

_RENDER = CARender.cpp
_RENDDIR = render
RENDER = $(patsubst %,$(_RENDDIR)/%,$(_RENDER))
RENOBJ = $(RENDER:.cpp=.o)

_GLADDEN = glad.c 
GLADDEN = $(patsubst %,$(_RENDDIR)/%,$(_GLADDEN))

_SRC = watercave.cpp fractal.cpp gameoflife.cpp ising.cpp lava.cpp maze.cpp splashes.cpp watercave.cpp
_MAINDIR = visualapp
MAINSRC = $(patsubst %,$(_MAINDIR)/%,$(_SRC))
MAINOBJ = $(MAINSRC:.cpp=.o)

EXEDIR = exe
_EXE = $(_SRC:.cpp=)
EXE = $(patsubst %,$(EXEDIR)/%,$(_EXE))

.PHONY: depend clean debug release

debug release: $(CAOBJ) $(RENOBJ) $(EXE)
	@echo $@
	@echo ======= compiled version: $@ ========


$(EXEDIR)/%: $(_MAINDIR)/%.o
	mkdir -p $(EXEDIR)
	$(CC) $(CMPORDER) $(INCLUDES) -std=c++14 -o  $@ $< $(GLADDEN) $(CAOBJ) $(RENOBJ) $(RENDDEPEN)

$(_MAINDIR)/%.o: $(_MAINDIR)/%.cpp
	$(CC) $(INCLUDES) $(CMPORDER)  -std=c++14   -c -o $@ $<

$(_CADIR)/%.o: $(_CADIR)/%.cpp
	$(CC) $(INCLUDES) $(CMPORDER)  -std=c++14   -c -o $@ $<

$(_RENDDIR)/%.o: $(_RENDDIR)/%.cpp
	$(CC) $(CMPORDER)  $(INCLUDES) -std=c++14 -c -o $@ $<  $(RENDDEPEN)

clean:
	$(RM) *.o *~ $(EXE)

cleanobj:
	$(RM)  $(RENOBJ) $(CAOBJ) 

