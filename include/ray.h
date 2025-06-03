#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct
{
    point3 orig;
    vec3   dir;
} ray;

static inline ray
ray_new( void )
{
    return (ray) { vec3_zero(), vec3_zero() };
}

static inline ray
ray_create( point3 origin, vec3 direction )
{
    return (ray) { origin, direction };
}

static inline point3
ray_origin( const ray * r )
{
    return r->orig;
}

static inline vec3
ray_direction( const ray * r )
{
    return r->dir;
}

static inline point3
ray_at( const ray * r, double t )
{
    return vec3_add( r->orig, vec3_mul( r->dir, t ) );
}

#endif // !RAY_H
