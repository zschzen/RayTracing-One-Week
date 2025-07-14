#include "dielectric.h"
#include "hittable.h"
#include "rtweekend.h"
#include <math.h>

// Schlick approximation for reflectance
static double
reflectance( double cosine, double ref_idx )
{
    double r0 = ( 1.0 - ref_idx ) / ( 1.0 + ref_idx );
    r0        = r0 * r0;
    return r0 + ( 1.0 - r0 ) * pow( ( 1.0 - cosine ), 5.0 );
}

void
dielectric_init( dielectric * mat, double index_of_refraction )
{
    if( !mat ) return;
    mat->base.scatter = dielectric_scatter;
    mat->ir           = index_of_refraction;
}

bool
dielectric_scatter( const material * material, const ray * r_in, const struct hit_record_s * rec, color * attenuation,
                    ray * scattered )
{
    const dielectric * self = (const dielectric *)material;
    *attenuation            = vec3_new( 1.0, 1.0, 1.0 ); // Glass is clear
    double refraction_ratio = rec->front_face ? ( 1.0 / self->ir ) : self->ir;

    vec3   unit_direction   = vec3_normalize( ray_direction( r_in ) );
    double cos_theta        = fmin( vec3_dot( vec3_negate( unit_direction ), rec->normal ), 1.0 );
    double sin_theta        = sqrt( 1.0 - cos_theta * cos_theta );

    bool cannot_refract     = ( refraction_ratio * sin_theta ) > 1.0;
    vec3 direction;

    if( cannot_refract || reflectance( cos_theta, refraction_ratio ) > random_double() )
        {
            direction = vec3_reflect( unit_direction, rec->normal );
        }
    else
        {
            direction = vec3_refract( unit_direction, rec->normal, refraction_ratio );
        }

    *scattered = ray_create( rec->p, direction );
    return true;
}
