#include <float.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h" /* typedef vec3 color, write_color_to_buffer */
#include "ray.h"   /* ray struct, ray_create, ray_origin, ray_direction, ray_at */
#include "vec3.h"  /* vec3/point3 struct, vec3_new, _sub, _add, _mul, _div, _length_squared, _dot, _normalize */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hittable.h"      /* hit_record, hittable interface */
#include "hittable_list.h" /* hittable_list */
#include "sphere.h"        /* sphere */

#include "hittable.h"
#include "sphere.h"

// Constants
#define ASPECT_RATIO             ( 16.0 / 9.0 )
#define IMAGE_WIDTH              800
#define CHANNELS                 3

// Macros
#define LERP( a, b, t )          ( ( a ) + ( ( b ) - ( a ) ) * ( t ) )
#define MIN( a, b )              ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define MAX( a, b )              ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define CLAMP( x, lower, upper ) ( MIN( ( upper ), MAX( ( x ), ( lower ) ) ) )

color
ray_color( const ray * r, const hittable * world )
{
    hit_record rec;

    // Check for hits
    // t_min = 0.001 to avoid shadow acne
    // t_max = INFINITY to consider hits at any distance
    if( world->hit( world, r, 0.001, INFINITY, &rec ) )
        {
            // Already oriented correctly by set_face_normal.
            return vec3_mul( (color) { rec.normal.x + 1.0, rec.normal.y + 1.0, rec.normal.z + 1.0 }, 0.5 );
        }

    // Background gradient
    vec3   unit_direction = vec3_normalize( ray_direction( r ) );
    double a              = 0.5 * ( unit_direction.y + 1.0 );

    color white           = vec3_new( 1.0, 1.0, 1.0 );
    color sky_blue        = vec3_new( 0.5, 0.7, 1.0 );
    return vec3_add( vec3_mul( white, 1.0 - a ), vec3_mul( sky_blue, a ) );
}

int
main( void )
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Image dimensions
    int image_height           = (int)( IMAGE_WIDTH / ASPECT_RATIO );
    image_height               = MAX( image_height, 1 );

    // Allocate image buffer
    const size_t    image_size = IMAGE_WIDTH * image_height * CHANNELS;
    unsigned char * image_data = (unsigned char *)malloc( image_size );
    if( !image_data )
        {
            fprintf( stderr, "Failed to alloc memory\n" );
            return EXIT_FAILURE;
        }

    // World setup
    //--------------------------------------------------------------------------------------
    hittable_list world;
    hittable_list_init( &world, 4 );

    // Spheres
    sphere sphere_gnd; // Ground sphere
    sphere_init( &sphere_gnd, vec3_new( 0.0, -100.5, -1.0 ), 100.0 );
    hittable_list_add( &world, (hittable *)&sphere_gnd );

    sphere sphere_ctr; // Center sphere
    sphere_init( &sphere_ctr, vec3_new( 0.0, 0.0, -1.0 ), 0.5 );
    hittable_list_add( &world, (hittable *)&sphere_ctr );

    sphere sphere_left; // Left sphere
    sphere_init( &sphere_left, vec3_new( -1.0, 0.0, -1.0 ), 0.5 );
    hittable_list_add( &world, (hittable *)&sphere_left );

    sphere sphere_right; // Right sphere
    sphere_init( &sphere_right, vec3_new( 1.0, 0.0, -1.0 ), 0.5 );
    hittable_list_add( &world, (hittable *)&sphere_right );

    // Camera
    //--------------------------------------------------------------------------------------
    double focal_length    = 1.0;
    double viewport_height = 2.0;
    double viewport_width  = viewport_height * ( (double)IMAGE_WIDTH / image_height );
    point3 camera_center   = vec3_new( 0.0, 0.0, 0.0 );

    // Viewport edge vectors
    vec3 viewport_u        = vec3_new( viewport_width, 0.0, 0.0 );   // Horizontal edge
    vec3 viewport_v        = vec3_new( 0.0, -viewport_height, 0.0 ); // Vertical edge (downward)

    // Pixel-to-pixel step vectors
    vec3 pixel_delta_u     = vec3_div( viewport_u, IMAGE_WIDTH );  // Horizontal step
    vec3 pixel_delta_v     = vec3_div( viewport_v, image_height ); // Vertical step

    // Calculate viewport upper-left corner
    vec3 focal_vector = vec3_new( 0.0, 0.0, focal_length ); // Vector from camera to focal plane along view direction

    vec3   viewport_center_offset = vec3_new( 0.0, 0.0, -focal_length ); // Assuming camera at origin looks along -Z
    point3 viewport_plane_center  = vec3_add( camera_center, viewport_center_offset );

    vec3 half_viewport_u          = vec3_div( viewport_u, 2.0 );
    vec3 half_viewport_v          = vec3_div( viewport_v, 2.0 ); // Note: viewport_v is already negative Y

    // Upper-left corner of the viewport plane
    vec3 viewport_upper_left      = vec3_sub(
        vec3_sub( vec3_add( camera_center, vec3_new( 0, 0, -focal_length ) ), half_viewport_u ), half_viewport_v );

    // First pixel center location (pixel [0,0])
    vec3 half_pixel_step = vec3_mul( vec3_add( pixel_delta_u, pixel_delta_v ), 0.5 );
    vec3 pixel00_loc     = vec3_add( viewport_upper_left, half_pixel_step );

    // Draw
    //--------------------------------------------------------------------------------------
    {
        unsigned char * pixel = image_data;
        for( int j = 0; j < image_height; ++j )
            {
                fprintf( stderr, "\rScanlines remaining: %d ", image_height - j - 1 );
                fflush( stderr );

                for( int i = 0; i < IMAGE_WIDTH; ++i )
                    {
                        // Calculate current pixel's center on the viewport
                        vec3   pixel_center_offset_u = vec3_mul( pixel_delta_u, (double)i );
                        vec3   pixel_center_offset_v = vec3_mul( pixel_delta_v, (double)j );
                        point3 pixel_center
                            = vec3_add( vec3_add( pixel00_loc, pixel_center_offset_u ), pixel_center_offset_v );

                        // Calculate ray direction from camera center to pixel center
                        vec3 r_direction = vec3_sub( pixel_center, camera_center );
                        ray  r           = ray_create( camera_center, r_direction );

                        // Get color for this ray
                        color pixel_c    = ray_color( &r, (hittable *)&world );

                        // Write color to image buffer
                        write_color_to_buffer( pixel, pixel_c );

                        pixel += CHANNELS; // Move to the next pixel in the buffer
                    }
            }

        fprintf( stderr, "\rDone.                                       \n" );
    }

    // Output
    //--------------------------------------------------------------------------------------
    {
        const char * filename = "output.png";
        if( !stbi_write_png( filename, IMAGE_WIDTH, image_height, CHANNELS, image_data, IMAGE_WIDTH * CHANNELS ) )
            {
                fprintf( stderr, "Failed to write output image\n" );
                free( image_data );
                return EXIT_FAILURE;
            }

        printf( "Successfully wrote output image to %s\n", filename );
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free( image_data );

    return EXIT_SUCCESS;
}
