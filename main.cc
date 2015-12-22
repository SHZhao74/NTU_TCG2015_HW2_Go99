#include "GTP.h"
#include <cstring>


int main(int argc, char* argv[]) {
//    int type = GTPVERSION;// 1: local version, 2: gtp version
    //int type = GTPVERSION;// 1: local version, 2: gtp version
    int display = 0; // 1: display, 2 nodisplay
    if (argc > 1) {
		if (strcmp(argv[1], "-display")==0) {
		    display = 1;
		}
		if (strcmp(argv[1], "-nodisplay")==0) {
		    display = 0;
		}
    }
    gtp_main(display);
    return 0;
}
