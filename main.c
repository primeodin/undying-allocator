#include "src/rune_core.h"
#include <stdio.h>
int main(int argc,char **argv){ RuneReport r=rune_simulate(180,0.016); printf("%s report steps=%d energy=%.4f drift=%.4f events=%d\n", "undying-allocator", r.steps,r.energy,r.drift,r.events); printf("formula %.5f\n", rune_formula_010(3.0,4.0)); return 0; }
