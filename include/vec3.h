#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdbool.h>

// Vector constants
#define VEC3_ZERO    ( vec3 ) { 0.0, 0.0, 0.0 }
#define VEC3_ONE     ( vec3 ) { 1.0, 1.0, 1.0 }
#define VEC3_UP      ( vec3 ) { 0.0, 1.0, 0.0 }
#define VEC3_DOWN    ( vec3 ) { 0.0, -1.0, 0.0 }
#define VEC3_LEFT    ( vec3 ) { -1.0, 0.0, 0.0 }
#define VEC3_RIGHT   ( vec3 ) { 1.0, 0.0, 0.0 }
#define VEC3_FORWARD ( vec3 ) { 0.0, 0.0, 1.0 }
#define VEC3_BACK    ( vec3 ) { 0.0, 0.0, -1.0 }

// 3D Vector structure
typedef struct
{
    double x, y, z;
} vec3;

// Alias for vec3. Eseful for geometric.
typedef vec3 point3;

// Constructor functions
static inline vec3
vec3_new( double x, double y, double z )
{
    return (vec3) { x, y, z };
}

static inline vec3
vec3_zero( void )
{
    return (vec3) { 0.0, 0.0, 0.0 };
}

static inline vec3
vec3_one( void )
{
    return (vec3) { 1.0, 1.0, 1.0 };
}

// Basic arithmetic operations
static inline vec3
vec3_add( vec3 a, vec3 b )
{
    return (vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static inline vec3
vec3_sub( vec3 a, vec3 b )
{
    return (vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static inline vec3
vec3_mul( vec3 v, double s )
{
    return (vec3) { v.x * s, v.y * s, v.z * s };
}

static inline vec3
vec3_div( vec3 v, double s )
{
    double inv_s = 1.0 / s;
    return (vec3) { v.x * inv_s, v.y * inv_s, v.z * inv_s };
}

static inline vec3
vec3_negate( vec3 v )
{
    return (vec3) { -v.x, -v.y, -v.z };
}

// Component-wise operations
static inline vec3
vec3_mul_vec( vec3 a, vec3 b )
{
    return (vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

static inline vec3
vec3_div_vec( vec3 a, vec3 b )
{
    return (vec3) { a.x / b.x, a.y / b.y, a.z / b.z };
}

// Vector products
static inline double
vec3_dot( vec3 a, vec3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3
vec3_cross( vec3 a, vec3 b )
{
    return (vec3) { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

// Magnitude and normalization
static inline double
vec3_length_squared( vec3 v )
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline double
vec3_length( vec3 v )
{
    return sqrt( vec3_length_squared( v ) );
}

static inline vec3
vec3_normalize( vec3 v )
{
    double len = vec3_length( v );
    if( len > 0.0 )
        {
            return vec3_div( v, len );
        }
    return vec3_zero();
}

static inline double
vec3_distance( vec3 a, vec3 b )
{
    return vec3_length( vec3_sub( a, b ) );
}

static inline double
vec3_distance_squared( vec3 a, vec3 b )
{
    return vec3_length_squared( vec3_sub( a, b ) );
}

// Interpolation
static inline vec3
vec3_lerp( vec3 a, vec3 b, double t )
{
    return vec3_add( a, vec3_mul( vec3_sub( b, a ), t ) );
}

// Comparison functions
static inline bool
vec3_equals( vec3 a, vec3 b, double epsilon )
{
    return fabs( a.x - b.x ) < epsilon && fabs( a.y - b.y ) < epsilon && fabs( a.z - b.z ) < epsilon;
}

static inline bool
vec3_is_zero( vec3 v, double epsilon )
{
    return vec3_length_squared( v ) < epsilon * epsilon;
}

// Utility functions
static inline vec3
vec3_min( vec3 a, vec3 b )
{
    return (vec3) { fmin( a.x, b.x ), fmin( a.y, b.y ), fmin( a.z, b.z ) };
}

static inline vec3
vec3_max( vec3 a, vec3 b )
{
    return (vec3) { fmax( a.x, b.x ), fmax( a.y, b.y ), fmax( a.z, b.z ) };
}

static inline vec3
vec3_clamp( vec3 v, vec3 min_v, vec3 max_v )
{
    return vec3_min( vec3_max( v, min_v ), max_v );
}

static inline vec3
vec3_abs( vec3 v )
{
    return (vec3) { fabs( v.x ), fabs( v.y ), fabs( v.z ) };
}

// Reflection and projection
static inline vec3
vec3_reflect( vec3 v, vec3 n )
{
    return vec3_sub( v, vec3_mul( n, 2.0 * vec3_dot( v, n ) ) );
}

static inline vec3
vec3_project( vec3 v, vec3 onto )
{
    double dot_product = vec3_dot( v, onto );
    double length_sq   = vec3_length_squared( onto );
    if( length_sq > 0.0 )
        {
            return vec3_mul( onto, dot_product / length_sq );
        }
    return vec3_zero();
}

#endif // VEC3_H
