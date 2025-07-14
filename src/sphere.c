#include "sphere.h"
#include "material.h"

void
sphere_init( sphere * s, point3 center_val, double radius_val, material * mat )
{
    if( !s ) return;
    s->base.hit     = sphere_hit_function; // Assign the sphere's hit function
    s->base.mat_ptr = mat;                 // Assign material
    s->center       = center_val;
    s->radius       = radius_val;
}

bool
sphere_hit_function( const hittable * object, const ray * r, double ray_tmin, double ray_tmax, hit_record * rec )
{
    const sphere * s       = (const sphere *)object;

    // Get ray properties
    point3 r_origin_val    = ray_origin( r );
    vec3   r_direction_val = ray_direction( r );

    vec3   oc              = vec3_sub( r_origin_val, s->center );
    double a               = vec3_length_squared( r_direction_val );
    double h               = vec3_dot( oc, r_direction_val );
    double c               = vec3_length_squared( oc ) - ( s->radius * s->radius );

    double discriminant    = h * h - a * c;
    if( discriminant < 0.0 )
        {
            return false;
        }
    double sqrtd = sqrt( discriminant );

    // Find the nearest root that lies in the acceptable range [ray_tmin, ray_tmax]
    double root  = ( -h - sqrtd ) / a;
    if( root <= ray_tmin || ray_tmax <= root )
        {
            root = ( -h + sqrtd ) / a;
            if( root <= ray_tmin || ray_tmax <= root )
                {
                    return false;
                }
        }

    // Intersection found, populate the hit_record
    rec->t              = root;
    rec->p              = ray_at( r, rec->t );
    rec->mat_ptr        = s->base.mat_ptr; // Assign material

    // Calculate the normal
    vec3 outward_normal = vec3_div( vec3_sub( rec->p, s->center ), s->radius );

    // Set the hit record's normal and front_face flag
    hit_record_set_face_normal( rec, r, &outward_normal );

    return true;
}
