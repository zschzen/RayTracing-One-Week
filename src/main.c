#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <stdio.h>
#include <stdlib.h>

// Constans
#define ASPECT_RATIO             ( 16.0 / 9.0 )
#define IMAGE_WIDTH              800

#define CHANNELS                 3

// Macros
#define LERP( a, b, t )          ( ( a ) + ( ( b ) - ( a ) ) * ( t ) )

#define MIN( a, b )              ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#define MAX( a, b )              ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define CLAMP( x, lower, upper ) ( MIN( ( upper ), MAX( ( x ), ( lower ) ) ) )

color
ray_color( const ray * r )
{
    vec3   unit_direction = vec3_normalize( ray_direction( r ) );
    double a              = 0.5 * ( unit_direction.y + 1.0 );

    color white           = (color) { 1.0, 1.0, 1.0 };
    color blue            = (color) { 0.5, 0.7, 1.0 };

    // Linear interpolation
    color result;
    result.x = LERP( white.x, blue.x, a );
    result.y = LERP( white.y, blue.y, a );
    result.z = LERP( white.z, blue.z, a );

    return result;
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

    // Camera
    double focal_length      = 1.0;
    double viewport_height   = 2.0;
    double viewport_width    = viewport_height * ( (double)IMAGE_WIDTH / image_height );
    point3 camera_center     = vec3_new( 0.0, 0.0, 0.0 );

    // Viewport edge vectors
    vec3 viewport_u          = vec3_new( viewport_width, 0.0, 0.0 );   // Horizontal edge
    vec3 viewport_v          = vec3_new( 0.0, -viewport_height, 0.0 ); // Vertical edge (downward)

    // Pixel-to-pixel step vectors
    vec3 pixel_delta_u       = vec3_div( viewport_u, IMAGE_WIDTH );  // Horizontal step
    vec3 pixel_delta_v       = vec3_div( viewport_v, image_height ); // Vertical step

    // Calculate viewport upper-left corner
    vec3 focal_point         = vec3_new( 0.0, 0.0, focal_length );
    vec3 viewport_center     = vec3_sub( camera_center, focal_point );
    vec3 half_viewport_u     = vec3_div( viewport_u, 2.0 );
    vec3 half_viewport_v     = vec3_div( viewport_v, 2.0 );

    vec3 viewport_upper_left = vec3_sub( vec3_sub( viewport_center, half_viewport_u ), half_viewport_v );

    // First pixel center location (pixel [0,0])
    vec3 half_pixel_offset   = vec3_mul( vec3_add( pixel_delta_u, pixel_delta_v ), 0.5 );
    vec3 pixel00_loc         = vec3_add( viewport_upper_left, half_pixel_offset );

    // Draw
    //--------------------------------------------------------------------------------------
    {
        // Generate image data
        unsigned char * pixel = image_data;
        for( int j = 0; j < image_height; ++j )
            {
                // Print progress indicator
                fprintf( stderr, "\rScanlines remaining: %d ", image_height - j );
                fflush( stderr );

                for( int i = 0; i < IMAGE_WIDTH; ++i )
                    {
                        vec3 pixel_center  = vec3_add( vec3_add( pixel00_loc, vec3_mul( pixel_delta_u, i ) ),
                                                       vec3_mul( pixel_delta_v, j ) );

                        vec3 ray_direction = vec3_sub( pixel_center, camera_center );
                        ray  r             = ray_create( camera_center, ray_direction );

                        // Use the color.h function to write to buffer
                        write_color_to_buffer( pixel, ray_color( &r ) );

                        // Move to next pixel
                        pixel += CHANNELS;
                    }
            }

        fprintf( stderr, "\rDone.                             \n" );
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
