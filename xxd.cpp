#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "xxd.hpp"

char *g_argv = nullptr;

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
        return EXIT_FAILURE;
    }
	
	int opt = 0;
	while((opt = getopt(argc, argv, "acEeg:hl:o:prs:uv")) != -1) {

		switch(opt) {
		case 'v':
			
			break;
		}
	}

	
	
	int index = 0;
	for (index = optind ; index < argc ; index++) {
		
	}
	
	return 0;
}
