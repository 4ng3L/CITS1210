PROJECT = mysync
COMPILE = gcc -std=c99 -Wall -pedantic -Werror
OPT_OBJ = option_a.o option_m.o option_n.o option_p.o option_r.o option_u.o option_v.o
OPT_SOURCE = option_a.c option_m.c option_n.c option_p.c option_r.c option_u.c option_v.c

$(PROJECT): libopt.a $(PROJECT).o
	$(COMPILE) -o mysync mysync_main.c libproj2.a $(PROJECT).o libopt.a

libopt.a: $(OPT_OBJ)
	ar rc libopt.a $(OPT_OBJ)

%.o: %.c $(PROJECT).h
	$(COMPILE) -c $<

clean:
	rm -f *.o
