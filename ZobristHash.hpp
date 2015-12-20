#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H
#include "GTP.hpp"
#include <unordered_map>
class Entry{
private:
  int wins;
  int total;
public:
  Entry(){
    //valid=false;
    wins=total=0;
  }
  void setWins(int val){wins=val;}
  int getWins(){return wins;}
  void setTotal(int val){total=val;}
  int getTotal(){return total;}
  //bool valid;

};

std::unordered_map<long int, Entry> HashTable;


#endif
