#include <math.h>
#include "MCS.hpp"
/*
*Constructor:
*/
MCSnode::MCSnode(){
	wins=total=0;
	x=y=-1;
	move_ = -1;
}
MCSnode::MCSnode(int move, MCSnode* parent){
	wins=total=0;
	x=y=-1;
	move_ = move;
	parent_ = parent;
}

/*
* UCB
*/
float MCSnode::compute_UCB (){
	return (float)wins/total + c_wight*sqrt(log(parent_->total)/total);
}

double MCSnode::getWinRate(){
	if(total > 0) {
        return (double)(wins / total);
    }
    else {
        return 0; /* Should not happen */
    }
}
void MCSnode::genChild(int &num_legal_moves, int* MoveList){
	MCSnode* ptr = NULL;
	for (short i = 0; i < num_legal_moves; i++) {
		if (ptr == NULL){ // if this node have no child
			child = new MCSnode(MoveList[i], this);
			ptr = getChild();
		}
		else{
			ptr->addSibling(new MCSnode(MoveList[i], this));
			ptr = ptr->getSibling();
		}
	}
}
/*
* Simulattion: random pick a move and play until to the end.
* Return how many komi the move win or lose.
*/
int MCSnode::simulation(){
	int move=-1;
	return move;
}

int MCSnode::UCTSearch(int Board[BOUNDARYSIZE][BOUNDARYSIZE] , int turn, int num_legal_moves, int* MoveList, clock_t end_t){
	if (num_legal_moves == 0)
		return 0;
  else{
    int move_id=0;
		MCSnode* ptr = getChild();
		if(ptr == NULL)
			genChild(num_legal_moves, MoveList);
		int b[BOUNDARYSIZE][BOUNDARYSIZE];
		for (int i = 0; i < BOUNDARYSIZE; i++) {
			for (int j = 0; j < BOUNDARYSIZE; j++) {
				b[i][j] = Board[i][j];
			}
		}
		while(end_t != clock()){
			ptr->simulation();
			ptr = ptr->getSibling();
		}
		//BestMove();
    return MoveList[move_id];
  }
	/*
	Board clone;
	for(int i = 0; i < time; i++) {
		clone.copyStateFrom(this);
		clone.playSimulation(*this->root);
	}

	Node* n = getBestChild(*this->root);
	Node* todelete = this->root;
	this->root = n;
	delete(todelete);
	delete(this->root->getSibling());
	this->root->setSibling(NULL);
	std::cout << "(" << n->getX() << "," << n->getY() << ")" << std::endl;
	makeMove(n->getX(), n->getY());*/
}
