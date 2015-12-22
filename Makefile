CC = g++ -Wall
#-stdlib=libstdc++
all:MCS GTP go99 randomgo99

#MCS:MCS.h MCS.cc
#	$(CC) -c MCS.h

GTP:GTP.h GTP.cc
	$(CC) -c GTP.cc

go99:MCS.o GTP.o
	$(CC) main.cc -o $@

randomgo99:
	$(CC) randomgo99.cpp -o randomgo99

clean:
	rm -f go99 randomgo99 *.o
