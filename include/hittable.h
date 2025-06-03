#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"  /* ray, _direction */
#include "vec3.h" /* vec3, _dot, _mul */
#include <stdbool.h>

// Forward declaration
struct hittable_s;

// Store intersection information of a ray-object
typedef struct hit_record_s
{
    point3 p;          // Intersection point
    vec3   normal;     // Surface normal, oriented against the incident ray
    double t;          // Ray parameter t at intersection
    bool   front_face; // True if ray hits the front face, false if it hits the back face
    // void* material; // Pointer to the material of the hit object
} hit_record;

// Hit function pointer
typedef bool ( *hit_fn )( const struct hittable_s * object, const ray * r, double ray_tmin, double ray_tmax,
                          hit_record * rec );

// The "hittable" interface struct
typedef struct hittable_s
{
    hit_fn hit;
} hittable;

// Set the hit record's normal vector and front_face flag
static inline void
hit_record_set_face_normal( hit_record * rec, const ray * r, const vec3 * outward_normal )
{
    // Determine if the ray is outside (hitting the front face) or inside (hitting a back face)
    rec->front_face = ( 0.0 > vec3_dot( ray_direction( r ), *outward_normal ) );
    rec->normal     = *outward_normal;
    if( false == rec->front_face )
        {
            // Ray is inside the sphere, normal should point inward
            rec->normal = vec3_mul( rec->normal, -1.0 );
        }
}

#endif // !HITTABLE_H
