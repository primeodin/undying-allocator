#ifndef RUNE_CORE_H
#define RUNE_CORE_H
#include <stddef.h>
typedef struct { double x; double y; double vx; double vy; double mass; int rage; } RuneBody;
typedef struct { int steps; double energy; double drift; int events; } RuneReport;
double rune_clamp(double v,double lo,double hi);
double rune_energy(RuneBody b);
void rune_step(RuneBody *b,double dt);
RuneReport rune_simulate(int steps,double dt);
double rune_formula(int idx,double x,double y);
#endif
