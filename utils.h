#pragma once

#if defined DEBUG
# define Debug(MESSAGE, ...) fprintf(stderr, "[Debug]: " MESSAGE "\n", ##__VA_ARGS__);
# define DebugPos(X, Y) fprintf(stderr, "[Debug]: Cursor position: %.2f:%.2f\n", X, Y);
#else
# define Debug(...)
# define DebugPos(...)
#endif

void Error(char *message);

[[noreturn]]
void Fail(char *message);
