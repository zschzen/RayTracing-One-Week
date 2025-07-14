#include "metal.h"
#include "hittable.h"
#include "rtweekend.h"

void
metal_init( metal * mat, color albedo, double fuzz )
{
    if( !mat ) return;
    mat->base.scatter = metal_scatter;
    mat->albedo       = albedo;
    mat->fuzz         = RT_MIN( fuzz, 1 );
}

bool
metal_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec, color * attenuation,
               ray * scattered )
{
    const metal * self      = (const metal *)material;
    vec3          reflected = vec3_reflect( vec3_normalize( ray_direction( r_in ) ), rec->normal );

    *scattered   = ray_create( rec->p, vec3_add( reflected, vec3_mul( random_in_unit_sphere(), self->fuzz ) ) );
    *attenuation = self->albedo;

    return ( vec3_dot( ray_direction( scattered ), rec->normal ) > 0 );
}
