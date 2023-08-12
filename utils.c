#include <stdio.h>
#include <stdlib.h>

void Error(char *message) {
	fprintf(stderr, "[Error]: %s\n", message);
}

void Fail(char *message) {
	fprintf(stderr, "[Error] [FATAL]: %s\n", message);
	exit(1);
}
