PROJECT = mysync
COMPILE = gcc -std=c99 -Wall -pedantic -Werror
MAIN = print.o process.o sync.o mysync.o

$(PROJECT): $(PROJECT).c $(MAIN) libproj2.a
	$(COMPILE) -o $(PROJECT) $(MAIN) -lproj2 -L./

%.o: %.c $(PROJECT).h
	$(COMPILE) -c $<

clean:
	rm -f *.o mysync
