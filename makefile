CC = g++

INCLUDES = -I depend/glm-0.9.9-a2 -I depend/glad -I ./render

CMPORDER = -march=native -g


_CA = CAWorld.cpp Cell.cpp Model.cpp run.cpp
CAOBJ = $(_CA:.cpp=.o)



RENDDEPEN = -lglfw -lGL -ldl

_RENDER = CARender.cpp
_RENDDIR = render
RENDER = $(patsubst %,$(_RENDDIR)/%,$(_RENDER))
RENOBJ = $(RENDER:.cpp=.o)

_GLADDEN = glad.c 
GLADDEN = $(patsubst %,$(_RENDDIR)/%,$(_GLADDEN))

MAIN = cave_visualized

.PHONY: depend clean

all:    $(MAIN)
	@echo drone_sim has been compiled

$(MAIN): $(CAOBJ) $(RENOBJ)
	$(CC) $(CMPORDER) $(INCLUDES) -std=c++14 -o $(MAIN) $(GLADDEN) $(CAOBJ) $(RENOBJ) $(RENDDEPEN)

./%.o: ./%.cpp
	$(CC) $(INCLUDES) $(CMPORDER)  -std=c++14   -c -o $@ $<

$(_RENDDIR)/%.o: $(_RENDDIR)/%.cpp
	$(CC) $(CMPORDER)  $(INCLUDES) -std=c++14 -c -o $@ $<  $(RENDDEPEN)

clean:
	$(RM) *.o *~ $(MAIN)

cleanobj:
	$(RM)  $(RENOBJ) $(CAOBJ)



