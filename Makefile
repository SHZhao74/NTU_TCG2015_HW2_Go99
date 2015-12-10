all:go99 randomgo99

go99:MCS.cc
	g++ go99.cc -o go99
	g++ randomgo99.cpp -o randomgo99
