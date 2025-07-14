#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

typedef struct
{
    material base;
    double   ir; // Index of Refraction
} dielectric;

void dielectric_init( dielectric * mat, double index_of_refraction );
bool dielectric_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec,
                         color * attenuation, ray * scattered );

#endif // DIELECTRIC_H
