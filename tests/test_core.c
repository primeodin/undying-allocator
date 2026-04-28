#include "../src/rune_core.h"
#include <assert.h>
#include <stdio.h>
int main(){ RuneReport r=rune_simulate(32,0.02); assert(r.steps==32); assert(r.energy>=0); assert(rune_formula_001(1,2)==rune_formula_001(1,2)); puts("c tests pass"); return 0; }
