all: test
test: test.o hsearch.o
clean:
	rm *.o test

.PHONY: all clean
