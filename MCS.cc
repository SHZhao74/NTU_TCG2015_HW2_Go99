#include <math.h>
#include "MCS.hpp"
float UCB (int num_win, int num_play_M, int num_play_v, int c){
	return (float)num_win/num_play_M + c*sqrt(log(num_play_v)/num_play_M);
}
