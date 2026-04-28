#include "rune_core.h"
#include <math.h>
#include <stdio.h>
double rune_clamp(double v,double lo,double hi){ if(v<lo) return lo; if(v>hi) return hi; return v; }
double rune_energy(RuneBody b){ return 0.5*b.mass*(b.vx*b.vx+b.vy*b.vy); }
void rune_step(RuneBody *b,double dt){ double rage=(double)b->rage/100.0; b->vy += (-9.8 + rage*2.0)*dt; b->x += b->vx*dt; b->y += b->vy*dt; if(b->y<0){ b->y=0; b->vy=-b->vy*(0.45+0.25*rage); b->rage=(int)rune_clamp(b->rage+7,0,100); } }
RuneReport rune_simulate(int steps,double dt){ RuneBody b={0,3,4,0,2,35}; RuneReport r={0,0,0,0}; int i; for(i=0;i<steps;i++){ double before=rune_energy(b); rune_step(&b,dt); r.energy+=rune_energy(b); r.drift+=fabs(rune_energy(b)-before); if(b.y==0) r.events++; r.steps++; } if(steps>0){ r.energy/=steps; r.drift/=steps; } return r; }


double rune_formula(int idx, double x, double y){
    double a;
    double b;
    if(idx < 0) idx = -idx;
    a = sin(x * (idx + 1.0) / 17.0) + cos(y * (idx + 3.0) / 23.0);
    b = sqrt(fabs(x * y) + 1.0);
    return a * b + (double)(idx % 11);
}
