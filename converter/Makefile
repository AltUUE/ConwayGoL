CC=g++
CFLAGS=-Wpedantic -O3

convertercli: convertercli.cpp converters.hpp
	$(CC) $(CFLAGS) convertercli.cpp -o $@

clean:
	rm -f *.o convertercli
