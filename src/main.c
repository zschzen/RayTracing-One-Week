#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* malloc, free, srand */
#include <time.h>   /* time */

#include "camera.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "rtweekend.h"
#include "sphere.h"

// Constants
#define ASPECT_RATIO      ( 16.0 / 9.0 )
#define IMAGE_WIDTH       1200
#define SAMPLES_PER_PIXEL 10
#define MAX_DEPTH         20

int
main( void )
{
    srand( time( NULL ) );

    // World
    //--------------------------------------------------------------------------------------
    hittable_list world;
    hittable_list_init( &world, 500 );

    // Ground Sphere
    {
        lambertian * mat_ground = malloc( sizeof( lambertian ) );
        lambertian_init( mat_ground, vec3_new( 0.5, 0.5, 0.5 ) );

        sphere * sphere_gnd = malloc( sizeof( sphere ) );
        sphere_init( sphere_gnd, vec3_new( 0.0, -1000, 0 ), 1000.0, (material *)mat_ground );
        hittable_list_add( &world, (hittable *)sphere_gnd );
    }

    // Random Spheres
    for( int a = -11; a < 11; ++a )
        {
            for( int b = -11; b < 11; ++b )
                {
                    double choose_mat = random_double();
                    point3 center     = vec3_new( a + 0.9 * random_double(), 0.2, b + 0.9 * random_double() );

                    if( vec3_length( vec3_sub( center, vec3_new( 4, 0.2, 0 ) ) ) > 0.9 )
                        {
                            sphere * s = malloc( sizeof( sphere ) );

                            if( 0.8 > choose_mat )
                                {
                                    // Diffuse material
                                    lambertian * mat = malloc( sizeof( lambertian ) );
                                    color        albedo
                                        = vec3_mul_vec( vec3_new( random_double(), random_double(), random_double() ),
                                                        vec3_new( random_double(), random_double(), random_double() ) );
                                    lambertian_init( mat, albedo );
                                    sphere_init( s, center, 0.2, (material *)mat );
                                }
                            else if( 0.95 > choose_mat )
                                {
                                    // Metal material
                                    metal * mat = malloc( sizeof( metal ) );
                                    color   albedo
                                        = vec3_new( random_double_range( 0.5, 1 ), random_double_range( 0.5, 1 ),
                                                    random_double_range( 0.5, 1 ) );
                                    double fuzz = random_double_range( 0, 0.5 );
                                    metal_init( mat, albedo, fuzz );
                                    sphere_init( s, center, 0.2, (material *)mat );
                                }
                            else
                                {
                                    // Glass material
                                    dielectric * mat = malloc( sizeof( dielectric ) );
                                    dielectric_init( mat, 1.5 );
                                    sphere_init( s, center, 0.2, (material *)mat );
                                }

                            hittable_list_add( &world, (hittable *)s );
                        }
                }
        }

    // Three large central spheres
    {
        // Glass sphere
        dielectric * mat1 = malloc( sizeof( dielectric ) );
        dielectric_init( mat1, 1.5 );

        sphere * sphere1 = malloc( sizeof( sphere ) );
        sphere_init( sphere1, vec3_new( 0, 1, 0 ), 1.0, (material *)mat1 );
        hittable_list_add( &world, (hittable *)sphere1 );
    }

    {
        // Brown diffuse sphere
        lambertian * mat2 = malloc( sizeof( lambertian ) );
        lambertian_init( mat2, vec3_new( 0.4, 0.2, 0.1 ) );

        sphere * sphere2 = malloc( sizeof( sphere ) );
        sphere_init( sphere2, vec3_new( -4, 1, 0 ), 1.0, (material *)mat2 );
        hittable_list_add( &world, (hittable *)sphere2 );
    }

    {
        // Metal sphere
        metal * mat3 = malloc( sizeof( metal ) );
        metal_init( mat3, vec3_new( 0.7, 0.6, 0.5 ), 0.0 );

        sphere * sphere3 = malloc( sizeof( sphere ) );
        sphere_init( sphere3, vec3_new( 4, 1, 0 ), 1.0, (material *)mat3 );
        hittable_list_add( &world, (hittable *)sphere3 );
    }

    // Camera
    //--------------------------------------------------------------------------------------
    camera cam;
    {
        point3 position      = vec3_new( 13, 2, 3 );
        point3 lookat        = vec3_new( 0, 0, 0 );
        vec3   vup           = vec3_new( 0, 1, 0 );
        double dist_to_focus = 10.0;
        double aperture      = 0.1;
        double vfov          = 20.0;

        camera_init( &cam, ASPECT_RATIO, vfov, position, lookat, vup, aperture, dist_to_focus, IMAGE_WIDTH,
                     SAMPLES_PER_PIXEL, MAX_DEPTH );
    }

    // Allocate image buffer
    unsigned char * image_data;
    {
        const size_t image_size = cam.image_width * cam.image_height * RT_IMAGE_DATA_CHANNELS;
        image_data              = (unsigned char *)malloc( image_size );
        if( !image_data )
            {
                fprintf( stderr, "Failed to alloc memory\n" );
                hittable_list_clear( &world );
                return EXIT_FAILURE;
            }
    }

    // Render
    //--------------------------------------------------------------------------------------
    {
        camera_render( &cam, (struct hittable *)&world, image_data );
    }

    // Output
    //--------------------------------------------------------------------------------------
    {
        const char * filename        = "output.png";
        const int    stride_in_bytes = cam.image_width * RT_IMAGE_DATA_CHANNELS;

        if( !stbi_write_png( filename, cam.image_width, cam.image_height, RT_IMAGE_DATA_CHANNELS, image_data,
                             stride_in_bytes ) )
            {
                fprintf( stderr, "Failed to write output image\n" );
                free( image_data );
                hittable_list_clear( &world );
                return EXIT_FAILURE;
            }

        printf( "Successfully wrote output image to %s\n", filename );
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    hittable_list_clear( &world );
    free( image_data );

    return EXIT_SUCCESS;
}
