CC = g++
#-stdlib=libstdc++
all:go99 randomgo99

MCS:
	$(CC) -c MCS.cc
GTP:
	$(CC) -c GTP.cc
go99:MCS.o GTP.o
	$(CC) main.cc -o go99

randomgo99:
	$(CC) randomgo99.cpp -o randomgo99

clean:
	rm -f go99 randomgo99 *.o
