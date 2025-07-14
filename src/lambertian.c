#include "lambertian.h"
#include "hittable.h"
#include "rtweekend.h"

void
lambertian_init( lambertian * mat, color albedo )
{
    if( !mat ) return;
    mat->base.scatter = lambertian_scatter;
    mat->albedo       = albedo;
}

bool
lambertian_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec, color * attenuation,
                    ray * scattered )
{
    RT_UNUSED( r_in );
    const lambertian * self = (const lambertian *)material;

    vec3 scatter_direction  = vec3_add( rec->normal, random_unit_vector() );

    // Catch degenerate scatter direction
    if( vec3_is_zero( scatter_direction, 1e-8 ) )
        {
            scatter_direction = rec->normal;
        }

    *scattered   = ray_create( rec->p, scatter_direction );
    *attenuation = self->albedo;

    return true;
}
