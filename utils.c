#include <stdio.h>
#include <stdlib.h>

void Debug(char *message) {
#ifdef DEBUG
	fprintf(stderr, "[Debug]: %s\n", message);
#endif
}

void DebugPos(double x, double y) {
#ifdef DEBUG
	fprintf(stderr, "[Debug]: Cursor position: %.2f:%.2f\n", x, y);
#endif
}

void Error(char *message) {
	fprintf(stderr, "[Error]: %s\n", message);
}

void Fail(char *message) {
	fprintf(stderr, "[Error] [FATAL]: %s\n", message);
	exit(1);
}
