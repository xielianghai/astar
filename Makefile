CC= cc

CFLAGS= -g
LDFLAGS= -lm

TARGETS= astar

BHEAPOBJS= Astar.o

all: $(TARGETS)


astar: $(BHEAPOBJS)
	$(CC) $(CFLAGS) -o $@ $(BHEAPOBJS) $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(TARGETS) *.o
run:
	./Astar
