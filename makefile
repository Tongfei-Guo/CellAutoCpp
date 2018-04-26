CC = g++

CFLAGS = -g -std=c++14

BasicCpp = CAWorld.cpp Cell.cpp Model.cpp CAFunctions.cpp
BasicObj = CAWorld.o Cell.o Model.o CAFunctions.o

CyclicCpp = $(BasicCpp) cyclic.cpp
CyclicObj = $(BasicObj) cyclic.o

CaveCpp = $(BasicCpp) cave.cpp
CaveObj = $(BasicObj) cave.o

ForestfireCpp = $(BasicCpp) forestfire.cpp
ForestfireObj = $(BasicObj) forestfire.o

TARGET = cyclic cave forestfire

.PHONY: depend clean cleanobj

all:  $(TARGET)
	@echo program has been compiled

cave: $(CaveObj)
	$(CC) $(CFLAGS) -o cave $(CaveObj)

cyclic: $(CyclicObj)
	$(CC) $(CFLAGS) -o cyclic $(CyclicObj)

forestfire: $(ForestfireObj)
	$(CC) $(CFLAGS) -o forestfire $(ForestfireObj)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(TARGET)

cleanobj:
	$(RM)  $(CAOBJ)
