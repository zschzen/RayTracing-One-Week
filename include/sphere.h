#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h" /* hittable, hit_record, hit_fn */
#include "vec3.h"     /* point3 */
#include <math.h>     /* fmax, sqrt */

// Sphere structure
typedef struct
{
    hittable base;
    point3   center;
    double   radius;
} sphere;

// Initializes a sphere object
void sphere_init( sphere * s, point3 center_val, double radius_val /*, void* mat */ );

// The hit detection function for spheres
bool sphere_hit_function( const hittable * object, const ray * r, double ray_tmin, double ray_tmax, hit_record * rec );

#endif // SPHERE_H
