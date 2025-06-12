
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <float.h>
#include <time.h>  /* time */

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

#include "camera.h"

// Constants
#define ASPECT_RATIO             ( 16.0 / 9.0 )
#define IMAGE_WIDTH              800
#define CHANNELS                 3
#define SAMPLES_PER_PIXEL        100 // For anti-aliasing
#define MAX_DEPTH                50  // Ray bounce limit

// Macros
#define LERP( a, b, t )          ( ( a ) + ( ( b ) - ( a ) ) * ( t ) )
#define MIN( a, b )              ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define MAX( a, b )              ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define CLAMP( x, lower, upper ) ( MIN( ( upper ), MAX( ( x ), ( lower ) ) ) )

color
ray_color( const ray * r, const hittable * world, int depth )
{
    hit_record rec = { 0 };

    if( depth <= 0 )
        {
            return vec3_new( 0, 0, 0 ); // Ray bounce limit reached
        }

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
    srand( time( NULL ) );

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
    camera cam;
    {
        point3 position      = vec3_new( -2, 2, 1 );
        point3 lookat        = vec3_new( 0, 0, -1 );
        vec3   vup           = vec3_new( 0, 1, 0 );
        double dist_to_focus = vec3_length( vec3_sub( position, lookat ) );
        double aperture      = 0;

        camera_init( &cam, ASPECT_RATIO, 20.0, position, lookat, vup, aperture, dist_to_focus );
    }

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
                        color pixel_color = vec3_new( 0, 0, 0 );
                        for( int s = 0; s < SAMPLES_PER_PIXEL; ++s )
                            {
                                double u    = (double)( i + random_double_camera() ) / ( IMAGE_WIDTH - 1 );
                                double v    = (double)( j + random_double_camera() ) / ( image_height - 1 );
                                ray    r    = camera_get_ray( &cam, u, v );
                                pixel_color = vec3_add( pixel_color, ray_color( &r, (hittable *)&world, MAX_DEPTH ) );
                            }
                        write_color_to_buffer( pixel, pixel_color, SAMPLES_PER_PIXEL );
                        pixel += CHANNELS;
                    }
            }

        fprintf( stderr, "\rDone.                                                \n" );
    }

    // Output
    //--------------------------------------------------------------------------------------
    {
        // Set compression to 0 for uncompressed TGA
        stbi_write_tga_with_rle = 0;

        const char * filename   = "output.tga";
        if( !stbi_write_tga( filename, IMAGE_WIDTH, image_height, CHANNELS, image_data ) )
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
