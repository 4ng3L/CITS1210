PROJECT = mysync
COMPILE = gcc -std=c99 -Wall -pedantic -Werror
OPT_OBJ = option_a.o option_m.o option_n.o option_p.o option_r.o option_u.o option_v.o
MAIN = print.o process.o sync.o

$(PROJECT): $(PROJECT).c $(MAIN) libproj2.a libopt.a
	$(COMPILE) -o mysync $(PROJECT).c $(MAIN) libproj2.a libopt.a

libopt.a: $(OPT_OBJ)
	ar rc libopt.a $(OPT_OBJ)

%.o: %.c $(PROJECT).h
	$(COMPILE) -c $<

clean:
	rm -f *.o
