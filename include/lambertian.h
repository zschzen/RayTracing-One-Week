#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"

typedef struct
{
    material base;
    color    albedo;
} lambertian;

void lambertian_init( lambertian * mat, color albedo );
bool lambertian_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec,
                         color * attenuation, ray * scattered );

#endif // LAMBERTIAN_H
