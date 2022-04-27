CC=g++
cflags=-Wall -O2
iflags=-I"C:/MinGW-SDL/include"
lflags=-L"C:/MinGW-SDL/lib" -lmingw32 -lSDL2main -lSDL2 -lstdc++

main.exe: main.cpp GoL.o Timer.o Generation.o Color.o
	$(CC) $(cflags) $(iflags) $^ $(lflags) -o $@

GoL.o: GoL.cpp GoL.h Generation.h Color.h Timer.h
	$(CC) $(cflags) $(iflags) -c GoL.cpp -o $@

Generation.o: Generation.cpp Generation.h Color.h
	$(CC) $(cflags) -c Generation.cpp -o $@

Timer.o: Timer.cpp Timer.h
	$(CC) $(cflags) $(iflags) -c Timer.cpp -o $@

Color.o: Color.cpp Color.h
	$(CC) $(cflags) -c Color.cpp -o $@

clean:
	rm -f *.o main.exe
