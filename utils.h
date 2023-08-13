#pragma once

#if defined __GNUC__
# define UNUSED(x) U_ ## x __attribute__((unused))
#else
# define UNUSED(x) U_ ## x
#endif

#if defined DEBUG
# define Debug(MESSAGE, ...) fprintf(stderr, "[Debug]: " MESSAGE "\n", ##__VA_ARGS__);
# define DebugPos(X, Y) fprintf(stderr, "[Debug]: Cursor position: %.2f:%.2f\n", X, Y);
# define DebugStatus(X) fprintf(stderr, "[Debug]: Command '" #X "' status: %d", (X));
#else
# define Debug(...)
# define DebugPos(...)
# define DebugStatus(X) X
#endif

void Error(char *message);

[[noreturn]]
void Fail(char *message);
