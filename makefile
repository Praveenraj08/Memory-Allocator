all:
		gcc -g -o main.o mem.c memalloc.c

clean:
		$(RM) fs
