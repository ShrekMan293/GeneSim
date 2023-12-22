#include "common.h"
#include "simulation.h"

using std::ifstream;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: [genesim] [outFile]\n");
		return 1;
	}

	simoptions = getCFG();
	return 0;
}