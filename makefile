CC = g++

INCLUDES = -I depend/glm-0.9.9-a2 -I depend/glad -I ./render -I ./src

CMPORDER = -march=native -g


_CA = CAWorld.cpp Cell.cpp Model.cpp lava.cpp CAFunctions.cpp
_CADIR = CAsrc
CA = $(patsubst %,$(_CADIR)/%,$(_CA))
CAOBJ = $(CA:.cpp=.o)



RENDDEPEN = -lglfw -lGL -ldl -lpthread

_RENDER = CARender.cpp
_RENDDIR = render
RENDER = $(patsubst %,$(_RENDDIR)/%,$(_RENDER))
RENOBJ = $(RENDER:.cpp=.o)

_GLADDEN = glad.c 
GLADDEN = $(patsubst %,$(_RENDDIR)/%,$(_GLADDEN))

MAIN = ising

.PHONY: depend clean

all:    $(MAIN)
	@echo drone_sim has been compiled

$(MAIN): $(CAOBJ) $(RENOBJ)
	$(CC) $(CMPORDER) $(INCLUDES) -std=c++14 -o $(MAIN) $(GLADDEN) $(CAOBJ) $(RENOBJ) $(RENDDEPEN)

$(_CADIR)/%.o: $(_CADIR)/%.cpp
	$(CC) $(INCLUDES) $(CMPORDER)  -std=c++14   -c -o $@ $<

$(_RENDDIR)/%.o: $(_RENDDIR)/%.cpp
	$(CC) $(CMPORDER)  $(INCLUDES) -std=c++14 -c -o $@ $<  $(RENDDEPEN)

clean:
	$(RM) *.o *~ $(MAIN)

cleanobj:
	$(RM)  $(RENOBJ) $(CAOBJ)

