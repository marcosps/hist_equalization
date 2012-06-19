all:
	gcc histograma.c -Wall -o h -lm
debug:
	gcc histograma.c -Wall -o h -g -lm
