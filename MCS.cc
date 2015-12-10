#include <math.h>
#include "MCS.hpp"
float compute_UCB (){
	return (float)num_win/num_play_total + c_wight*sqrt(log(num_play_parent)/num_play_total);
}
