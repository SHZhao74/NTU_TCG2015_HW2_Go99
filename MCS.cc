#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "MCS.h"
using namespace std;
/*
*Constructor:
*/
MCSnode::MCSnode(int& t,int b[BOUNDARYSIZE][BOUNDARYSIZE], int& numLegalMove, int* MoveList){
	wins=total=0;
	x=y=-1;
	move_ = -1;
	turn =t;
	child = NULL;
	sibling=NULL;
	moved_cnt=0;
	num_play_parent = numLegalMove;
	setBoard(b);
	//setLegalMove(numLegalMove);
}
MCSnode::MCSnode(int t, int numLegalMove, int& moveId, int& move, MCSnode* parent){
	wins=total=0;
	x=y=-1;
	move_ = move;
	parent_ = parent;
	turn = t;
	move_id = moveId;
	num_legal_moves = numLegalMove;
	moved_cnt=0;
	int move_x = (move % 100) / 10;
	int move_y = move % 10;
	if (move<100) {
		Board[move_x][move_y] = turn;
	}
	else{
		printf("move>100\n\n");
		remove_piece(Board,move_x,move_y,3-turn);
	}
}
void MCSnode::setBoard(int b[BOUNDARYSIZE][BOUNDARYSIZE]){
	for (int i = 0; i < BOUNDARYSIZE; i++) {
		for (int j = 0; j < BOUNDARYSIZE; j++) {
			Board[i][j] = b[i][j];
		}
	}
}
/*
* UCB
*/
double MCSnode::compute_UCB (){
	return getWinRate() + c_wight*sqrt(log(parent_->total)/total);
}

double MCSnode::getWinRate(){
	if(total > 0) {
        return (double)(wins / total);
    }
    else {
        return 0; /* Should not happen */
    }
}
void MCSnode::genChild(){
	MCSnode* ptr = NULL;
	for (int i = 0; i < num_legal_moves; i++) {
		if (i==move_id)continue;//avoid creating the same move
		if (ptr == NULL){ // if this node have no child
			addChild(new MCSnode(3-turn, num_legal_moves-1, i,MoveList[i], this));
			ptr = getChild();
		}
		else{
			ptr->addSibling(new MCSnode(3-turn++, num_legal_moves-1 ,i, MoveList[i], this));
			ptr = ptr->getSibling();
		}
	}
}

MCSnode* MCSnode::UCTSelect(MCSnode &node){
	MCSnode* res = NULL;
	MCSnode* next = node.getChild();
	double best_uct = 0;

	while(next != NULL) {
		double uctvalue;
		if(next->getTotal() > 0) {
			uctvalue = next->compute_UCB();
		}
		else {
			uctvalue = 10000 + 1000*(rand()%1000);
		}

		if(uctvalue > best_uct) {
			best_uct = uctvalue;
			res = next;
		}
		else if(uctvalue == best_uct && rand()%2 == 0) {
			best_uct = uctvalue;
			res = next;
		}

		next = next->getSibling();
	}

	return res;
}
void MCSnode::makeRandomMove(){
	//rand_pick_move(num_legal_moves, MoveList);
	int move = move_id;
	int color= turn;
	while(move == move_id || !update_board_check(Board,move%100/10, move%100%10, color)){
		move = rand()%num_legal_moves;
	}
	moved_cnt++;
	if (moved_cnt<65){
		this->makeRandomMove();
	}
}
/*int MCSnode::playRandomGame(int &num_legal_moves, int MoveList[HISTORYLENGTH]){
	int cur_player = this->player;
	

	//return influence() == cur_player ? 1 : 0;
}*/
/*
* call by root
*
* Simulattion: random pick a move and play until to the end.
* Return how many komi the move win or lose.
*/
int MCSnode::simulation(){
	//int move=-1;
	int randomresult = 0;
	if(getChild() == NULL && getTotal() < 5) {
		printf("simulation() move_id=%d\n",move_id);
		this->makeRandomMove();

		//randomresult = playRandomGame(num_legal_moves, MoveList);
	}
	else {

		if(this->getChild() == NULL) genChild();
		//MCSnode* next= getChild();
		MCSnode* next = UCTSelect(*this);
		//makeMove(next->getX(), next->getY());
		
		printf("simulation(): move_id=%d sim deeper\n",move_id);
		int res = next->simulation();
		randomresult = 1-res;
	}

	this->update(1-randomresult);
	return randomresult;

	//eturn move;
}

int simCnt=0;
int MCSnode::UCTSearch(unsigned long end_t){
	if (num_legal_moves == 0)
		return 0;
	else{
		int move_id=0;
		MCSnode* ptr = getChild();
		if(ptr == NULL)
			genChild(); //num_legal_moves, MoveList
		while(end_t < clock()){
			simCnt++;
			ptr->simulation();
			ptr->PrintInfo();
			ptr = ptr->getSibling();
		}
		//BestMove();
		printf("simCnt=%d\n", simCnt);
    	return MoveList[move_id];
	}
}
