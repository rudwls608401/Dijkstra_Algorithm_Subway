#define MAX_STATION 398
#include "SubwayGraph.h"

int main()
{
	SubwayGraph run_test;
	run_test.Subway("subway.txt", MAX_STATION);
	return 0; 
}
