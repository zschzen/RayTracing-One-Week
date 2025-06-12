#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <math.h>

// Color is represented as a vec3 with RGB components
typedef vec3 color;

static inline double
clamp( double x, double min_val, double max_val )
{
    if( x < min_val ) return min_val;
    if( x > max_val ) return max_val;
    return x;
}

// Interval structure for clamping
typedef struct
{
    double min, max;
} interval;

static inline interval
interval_new( double min, double max )
{
    return (interval) { min, max };
}

static inline double
interval_clamp( interval i, double x )
{
    if( x < i.min ) return i.min;
    if( x > i.max ) return i.max;
    return x;
}

static inline double
linear_to_gamma( double linear_component )
{
    if( linear_component > 0 ) return sqrt( linear_component );
    return 0;
}

static inline void
write_color_to_buffer( unsigned char * pBuffer, const color pixel_c, int samples_per_pixel )
{
    double r     = pixel_c.x;
    double g     = pixel_c.y;
    double b     = pixel_c.z;

    // Divide the color by the number of samples for anti-aliasing
    double scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply gamma correction (gamma=2.0). Transforms the color to linear space.
    r          = sqrt( r );
    g          = sqrt( g );
    b          = sqrt( b );

    // Translate the [0,1] component values to the byte range [0,255] and clamp.
    pBuffer[0] = (unsigned char)( 256 * clamp( r, 0.0, 0.999 ) );
    pBuffer[1] = (unsigned char)( 256 * clamp( g, 0.0, 0.999 ) );
    pBuffer[2] = (unsigned char)( 256 * clamp( b, 0.0, 0.999 ) );
}

#endif
