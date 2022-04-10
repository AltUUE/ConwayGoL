CC=g++
CFLAGS=-Wpedantic -O3
IFLAGS=-I"C:\mingwdevlib\cxxopts-2.2.1\include"

convertercli.exe: convertercli.cpp converters.hpp
	$(CC) $(CFLAGS) $(IFLAGS) convertercli.cpp -o $@

clean:
	rm -f *.o convertercli.exe