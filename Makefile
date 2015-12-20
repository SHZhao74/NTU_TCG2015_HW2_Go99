all:go99 randomgo99

go99:
	g++ -stdlib=libstdc++ go99.cc -o go99 

randomgo99:
	g++ randomgo99.cpp -o randomgo99
