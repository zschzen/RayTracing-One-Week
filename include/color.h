#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <math.h>

// Color is represented as a vec3 with RGB components
typedef vec3 color;

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
write_color_to_buffer( unsigned char * pBuffer, const color pixel_color )
{
    double r                        = linear_to_gamma( pixel_color.x );
    double g                        = linear_to_gamma( pixel_color.y );
    double b                        = linear_to_gamma( pixel_color.z );

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity = { 0.000, 0.999 };
    int                   rbyte     = (int)( 256 * interval_clamp( intensity, r ) );
    int                   gbyte     = (int)( 256 * interval_clamp( intensity, g ) );
    int                   bbyte     = (int)( 256 * interval_clamp( intensity, b ) );

    // Write out the pixel color components to buffer
    pBuffer[0]                      = (unsigned char)rbyte; // R
    pBuffer[1]                      = (unsigned char)gbyte; // G
    pBuffer[2]                      = (unsigned char)bbyte; // B
}

#endif

