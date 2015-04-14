#makefile
CC=g++
CTYPE=cpp
CFLAGS=-c -Wall
INFLAGS=-Iinclude
LDFLAGS=-L.

OBJDIR:=objdir
SOURCES=main.$(CTYPE) Graph.$(CTYPE) GraphGenerator.$(CTYPE) Model.$(CTYPE) \
Testing.$(CTYPE) \
Strategy.$(CTYPE) bfs.$(CTYPE) dfs.$(CTYPE) ids.$(CTYPE) greedy.$(CTYPE) \
astar.$(CTYPE) hillClimb.$(CTYPE)
OBJECTS=$(addprefix $(OBJDIR)\, $(SOURCES:.$(CTYPE)=.o) )

EXECUTABLE=noin

#-g  allow for debug mode 
#-E  macro expanded

all: $(SOURCES) $(EXECUTABLE)

$(OBJDIR)\\%.o: %.$(CTYPE)
	$(CC) $(CFLAGS) $(INFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)


# Setup objects directory
$(OBJECTS): | $(OBJDIR)
$(OBJDIR):
	mkdir $(OBJDIR)

# clean methods
clean:
	rmdir /s /q $(OBJDIR) 2> nul

