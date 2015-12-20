#ifndef MCS_H
#define MCS_H
#include <ctime>
#include "GTP.hpp"
double c_wight = 0.44;

class MCSnode{
public:
  float UCB;
  MCSnode();
  MCSnode(int, MCSnode*);
  ~MCSnode(){
    if (this->child != NULL)
      delete this->child;
    if (sibling != NULL)
      delete sibling;
  }
  float compute_UCB();
  double getWinRate();
  int simulation();
  int UCTSearch(int[BOUNDARYSIZE][BOUNDARYSIZE], int turn , int num_legal_moves, int* MoveList, clock_t end_t);

  //void gen_legal_move();
  void genChild(int&num_legal_moves, int *MoveList);
  MCSnode* getChild(){return child;}
  MCSnode* getSibling(){return sibling;}
  void addChild(MCSnode* child){this->child = child;}
  void addSibling(MCSnode* s){this->sibling = s;}
private:

  int wins;  //num_win
  int total; //num_play_total
  int num_play_parent;
  int x,y;         //coor of the move
  int move_;
  int Board[BOUNDARYSIZE][BOUNDARYSIZE];
  MCSnode* child;
  MCSnode* sibling;
  MCSnode* parent_;
};

#endif
