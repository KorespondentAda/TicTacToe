#pragma once

#ifdef DEBUG
void Debug(char *message);
void DebugPos(double cx, double cy);
#else
#define Debug(x)
#define DebugPos(x, y)
#endif

void Error(char *message);

[[noreturn]]
void Fail(char *message);
