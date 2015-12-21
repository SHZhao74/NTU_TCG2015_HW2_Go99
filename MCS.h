#ifndef MCS_H
#define MCS_H
#include <ctime>
#include "GTP.h"
double c_wight = 0.44;

class MCSnode{
public:
  MCSnode(int& t,int b[BOUNDARYSIZE][BOUNDARYSIZE], int& numLegalMove, int* MoveList); //root
  MCSnode(int,int,int&,int&, MCSnode*); //gen childs
  ~MCSnode(){
    if (this->child != NULL)
      delete this->child;
    if (sibling != NULL)
      delete sibling;
  }
  void setBoard(int b[BOUNDARYSIZE][BOUNDARYSIZE]);
  void setLegalMove(int& val){num_legal_moves = val;}
  double compute_UCB();
  double getWinRate();
  void makeRandomMove();
  int simulation();
  int UCTSearch(unsigned long end_t);
  MCSnode* UCTSelect(MCSnode &node);
  void update(int val){total++; wins+=val;}
  //void gen_legal_move();
  void genChild();
  MCSnode* getChild(){return child;}
  MCSnode* getSibling(){return sibling;}
  int getTotal(){return total;}
  void addChild(MCSnode* child){this->child = child;}
  void addSibling(MCSnode* s){this->sibling = s;}

  void PrintInfo(){
    printf("move_id=%d, num_legal_moves=%d\n"
            "total=%d, wins=%d\n"
            , move_id, num_legal_moves, total, wins);
  }

private:
  int Board[BOUNDARYSIZE][BOUNDARYSIZE];
  static int MoveList[HISTORYLENGTH];
  int wins;  //num_win
  int total; //num_play_total
  int num_legal_moves;
  int move_id; //no. in the MoveList
  MCSnode* child;
  MCSnode* sibling;
  MCSnode* parent_;

  /*simulation info*/
  int num_play_parent;
  int x,y;         //coor of the move
  int move_;
  int moved_cnt; //used to count how many times haved moved
  int turn; //this node play as which one
    
  
};

#endif
