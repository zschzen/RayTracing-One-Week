#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <time.h>          /* time */

#include "camera.h"
#include "hittable_list.h" /* hittable_list */
#include "sphere.h"        /* sphere */

// Constants
#define ASPECT_RATIO      ( 16.0 / 9.0 )
#define IMAGE_WIDTH       800
#define CHANNELS          3
#define SAMPLES_PER_PIXEL 100 // For anti-aliasing
#define MAX_DEPTH         50  // Ray bounce limit

int
main( void )
{
    srand( time( NULL ) );

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
        double aperture      = 0.0;

        // Initialize camera
        camera_init( &cam, ASPECT_RATIO, 20.0, position, lookat, vup, aperture, dist_to_focus, IMAGE_WIDTH,
                     SAMPLES_PER_PIXEL, MAX_DEPTH );
    }

    // Allocate image buffer
    unsigned char * image_data;
    {
        const size_t image_size = cam.image_width * cam.image_height * CHANNELS;
        image_data              = (unsigned char *)malloc( image_size );
        if( !image_data )
            {
                fprintf( stderr, "Failed to alloc memory\n" );
                return EXIT_FAILURE;
            }
    }

    // Draw
    //--------------------------------------------------------------------------------------
    {
        camera_render( &cam, (struct hittable *)&world, image_data );
    }

    // Output
    //--------------------------------------------------------------------------------------
    {
        // Set compression to 0 for uncompressed TGA
        stbi_write_tga_with_rle = 0;

        const char * filename   = "output.tga";
        if( !stbi_write_tga( filename, cam.image_width, cam.image_height, CHANNELS, image_data ) )
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
