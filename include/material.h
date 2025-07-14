#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "ray.h"
#include <stdbool.h>

// Forward declarations
struct hit_record_s;

// The "material" interface struct
typedef struct material_s
{
    // A function pointer for the scattering behavior of the material
    //
    // Parameters:
    //   material   : The material instance
    //   r_in       : The incoming ray that hit the surface
    //   rec        : The hit_record containing details of the intersection
    //   attenuation: Output parameter for the color attenuation of the material
    //   scattered  : Output parameter for the new, scattered ray
    //
    // Returns:
    //   true if the ray is scattered, false if it is absorbed
    bool ( *scatter )( const struct material_s * material, const ray * r_in, const struct hit_record_s * rec,
                       color * attenuation, ray * scattered );
} material;

#endif // !MATERIAL_H
