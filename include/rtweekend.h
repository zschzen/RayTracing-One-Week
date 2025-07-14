#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include "vec3.h"
#include <float.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------------------------------------------------
#define RT_INFINITY            INFINITY
#define RT_TAU                 6.28318530717958647692
#define RT_DEG2RAD             ( RT_TAU / 360.0F )

#define RT_IMAGE_DATA_CHANNELS 3

//----------------------------------------------------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------------------------------------------------
// Unused component
#define RT_UNUSED( x )         ( (void)( x ) )

// Clamps a value within a specified range [min, max]
#define RT_CLAMP( v, min, max )                                                                                        \
    ( {                                                                                                                \
        __typeof__( v )   _v   = ( v );                                                                                \
        __typeof__( min ) _min = ( min );                                                                              \
        __typeof__( max ) _max = ( max );                                                                              \
        ( _v < _min ) ? _min : ( ( _v > _max ) ? _max : _v );                                                          \
    } )

//Returns the maximum of two values
#define RT_MAX( a, b )                                                                                                 \
    ( {                                                                                                                \
        __typeof__( a ) _a = ( a );                                                                                    \
        __typeof__( b ) _b = ( b );                                                                                    \
        ( _a > _b ) ? _a : _b;                                                                                         \
    } )

// Returns the minimum of two values
#define RT_MIN( a, b )                                                                                                 \
    ( {                                                                                                                \
        __typeof__( a ) _a = ( a );                                                                                    \
        __typeof__( b ) _b = ( b );                                                                                    \
        ( _a < _b ) ? _a : _b;                                                                                         \
    } )

//----------------------------------------------------------------------------------------------------------------------
// Utility Functions
//----------------------------------------------------------------------------------------------------------------------
// Returns a random real in [0,1)
static inline double
random_double()
{
    return rand() / ( RAND_MAX + 1.0 );
}

// Returns a random real in [min,max)
static inline double
random_double_range( double min, double max )
{
    return min + ( max - min ) * random_double();
}

// Generate a random vec3 inside a unit shape
static inline vec3
random_in_unit( double z_value )
{
    vec3 p;
    do
        {
            p = vec3_new( random_double_range( -1, 1 ), random_double_range( -1, 1 ), z_value );
        }
    while( vec3_length_squared( p ) >= 1 );
    return p;
}

// Returns a random point inside a unit sphere
static inline vec3
random_in_unit_sphere()
{
    return random_in_unit( random_double_range( -1, 1 ) );
}

// Returns a random point inside a unit disk on the XY plane
static inline vec3
random_in_unit_disk()
{
    return random_in_unit( 0.0 );
}

// Returns a random unit vector
static inline vec3
random_unit_vector()
{
    return vec3_normalize( random_in_unit_sphere() );
}

#endif // RTWEEKEND_H
