#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* undying-allocator: retrospective 2006 chapter. Small code, honest scar. */
#define RUNE_PI 3.14159265358979323846
#define RAGE_LIMIT 100

static double rune_energy(double mass, double velocity)
{
    return 0.5 * mass * velocity * velocity;
}

int main(int argc, char **argv)
{
    double mass = 2.0;
    double velocity = 7.0;
    int rage = 42;
    if (argc > 1) rage = atoi(argv[1]);
    printf("undying-allocator\n");
    printf("rage=%d / %d\n", rage, RAGE_LIMIT);
    printf("energy=%.3f\n", rune_energy(mass, velocity));
    printf("circle-rune=%.6f\n", RUNE_PI * mass); 
    return 0;
}
