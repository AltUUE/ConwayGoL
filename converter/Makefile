CC=g++
CFLAGS=-Wpedantic -O3

convertercli.exe: convertercli.cpp converters.hpp
	$(CC) $(CFLAGS) convertercli.cpp -o $@

clean:
	rm -f *.o convertercli.exe
