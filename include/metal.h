#ifndef METAL_H
#define METAL_H

#include "material.h"

typedef struct
{
    material base;
    color    albedo;
    double   fuzz;
} metal;

void metal_init( metal * mat, color albedo, double fuzz );
bool metal_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec, color * attenuation,
                    ray * scattered );

#endif // METAL_H
