CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEPS = headerfile.h
OBJ = main.o algun.o
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean

clean:
	rm -f $(OBJ) main
