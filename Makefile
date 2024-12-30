CC=gcc
CFLAGS=-Wall -Werror -I. -g
TARGETS=cat ls
DEPS=

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGETS)
cat: cat.o
	$(CC) -o $@ $^ $(CFLAGS)

ls: ls.o
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install -D $(TARGETS) $(HOME)/.local/bin/

clean:
	rm -rf *.o $(TARGETS)
