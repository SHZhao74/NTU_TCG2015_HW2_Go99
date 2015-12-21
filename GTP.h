#ifndef GTP_H
#define GTP_H

#include <ctime>

#define BOARDSIZE        9
#define BOUNDARYSIZE    11
#define COMMANDLENGTH 1000
#define DEFAULTTIME     10
#define DEFAULTKOMI      7

#define MAXGAMELENGTH 1000
#define MAXSTRING       50
#define MAXDIRECTION     4

#define NUMINTERSECTION 81
#define HISTORYLENGTH   200

//#define EMPTY            0
//#define BLACK            1
//#define WHITE            2
//#define BOUNDARY         3
enum {
  EMPTY = 0,
  BLACK = 1,
  WHITE = 2,
  BOUNDARY
};
#define SELF             1
#define OPPONENT         2

#define NUMGTPCOMMANDS      15

#define LOCALVERSION      1
#define GTPVERSION        2

int _board_size = BOARDSIZE;
int _board_boundary = BOUNDARYSIZE;
double _komi =  DEFAULTKOMI;
const int DirectionX[MAXDIRECTION] = {-1, 0, 1, 0};
const int DirectionY[MAXDIRECTION] = { 0, 1, 0,-1};
const char LabelX[]="0ABCDEFGHJ";

/*
 * This function return the total number of liberity of the string of (X, Y) and
 * the string will be label with 'label'.
 * */
int find_liberty(int X, int Y, int label, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE]) {
    // Label the current intersection
    ConnectBoard[X][Y] |= label;
    int total_liberty = 0;
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// Check this intersection has been visited or not
	if ((ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] & (1<<label) )!= 0) continue;

	// Check this intersection is not visited yet
	ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] |=(1<<label);
	// This neighboorhood is empty
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == EMPTY){
	    total_liberty++;
	}
	// This neighboorhood is self stone
	else if (Board[X+DirectionX[d]][Y+DirectionY[d]] == Board[X][Y]) {
	    total_liberty += find_liberty(X+DirectionX[d], Y+DirectionY[d], label, Board, ConnectBoard);
	}
    }
    return total_liberty;
}

/*
 * This function count the liberties of the given intersection's neighboorhod
 * */
void count_liberty(int X, int Y, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int Liberties[MAXDIRECTION]) {
    int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    // Initial the ConnectBoard
    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
	for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
	    ConnectBoard[i][j] = 0;
	}
    }
    // Find the same connect component and its liberity
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	Liberties[d] = 0;
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == BLACK ||
	    Board[X+DirectionX[d]][Y+DirectionY[d]] == WHITE    ) {
	    Liberties[d] = find_liberty(X+DirectionX[d], Y+DirectionY[d], d, Board, ConnectBoard);
	}
    }
}


/*
 * This function remove the connect component contains (X, Y) with color "turn"
 * And return the number of remove stones.
 * */
int remove_piece(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    int remove_stones = (Board[X][Y]==EMPTY)?0:1;
    Board[X][Y] = EMPTY;
    for (int d = 0; d < MAXDIRECTION; ++d) {
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == turn) {
	    remove_stones += remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], turn);
	}
    }
    return remove_stones;
}
/*
 * This function randomly selects one move from the MoveList.
 * int rand_pick_move(int num_legal_moves, int MoveList[HISTORYLENGTH]) {
    if (num_legal_moves == 0)
      return 0;
    else {
      int move_id = rand()%num_legal_moves;
      return MoveList[move_id];
    }
}*/

/*
 * This function count the number of empty, self, opponent, and boundary intersections of the neighboorhod
 * and saves the type in NeighboorhoodState.
 * */
void count_neighboorhood_state(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn, int* empt, int* self, int* oppo ,int* boun, int NeighboorhoodState[MAXDIRECTION]) {
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// check the number of nonempty neighbor
	switch(Board[X+DirectionX[d]][Y+DirectionY[d]]) {
	    case EMPTY:    (*empt)++;
			   NeighboorhoodState[d] = EMPTY;
			   break;
	    case BLACK:    if (turn == BLACK) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case WHITE:    if (turn == WHITE) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case BOUNDARY: (*boun)++;
			   NeighboorhoodState[d] = BOUNDARY;
			   break;
	}
    }
}

/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will check if (X, Y) is a legal move or not.
 * */
int update_board_check(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    // Check the given coordination is legal or not
    if ( X < 1 || X > BOARDSIZE || Y < 1 || Y > BOARDSIZE || Board[X][Y]!=EMPTY)
	return 0;
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int Liberties[4];
    int NeighboorhoodState[4];
    count_neighboorhood_state(Board, X, Y, turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
    // Check if the move is a legal move
    // Condition 1: there is a empty intersection in the neighboorhood
    int legal_flag = 0;
    count_liberty(X, Y, Board, Liberties);
    if (num_neighborhood_empt != 0) {
	legal_flag = 1;
    }
    else {
	// Condition 2: there is a self string has more than one liberty
	for (int d = 0; d < MAXDIRECTION; ++d) {
	    if (NeighboorhoodState[d] == SELF && Liberties[d] > 1) {
		legal_flag = 1;
	    }
	}
	if (legal_flag == 0) {
	// Condition 3: there is a opponent string has exactly one liberty
	    for (int d = 0; d < MAXDIRECTION; ++d) {
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
		    legal_flag = 1;
		}
	    }
	}
    }

    if (legal_flag == 1) {
    // check if there is opponent piece in the neighboorhood
	if (num_neighborhood_oppo != 0) {
	    for (int d = 0 ; d < MAXDIRECTION; ++d) {
		// check if there is opponent component only one liberty
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1 && Board[X+DirectionX[d]][Y+DirectionY[d]]!=EMPTY) {
		    remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], Board[X+DirectionX[d]][Y+DirectionY[d]]);
		}
	    }
	}
	Board[X][Y] = turn;
    }

    return (legal_flag==1)?1:0;
}
void gtp_main(int display);
#endif
