#pragma once

void Debug(char *message);
void DebugPos(double cx, double cy);

void Error(char *message);

[[noreturn]]
void Fail(char *message);
