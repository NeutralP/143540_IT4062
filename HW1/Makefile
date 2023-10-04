PROGRAM = week1

all: $(PROGRAM)

$(PROGRAM): $(PROGRAM).o
	gcc -o $(PROGRAM) $(PROGRAM).o

$(PROGRAM).o: $(PROGRAM).c
	gcc -c $(PROGRAM).c

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) $(PROGRAM).o
