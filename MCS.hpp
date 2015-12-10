#ifndef MCS_H
#define MCS_H
int c_wight = 1;

class MCSnode{
public:
  MCSnode(){}
  
private:
  float UCB;
  int num_win;
  int num_play_total;
  int num_play_parent;
  MCSnode* child;
};

#endif
