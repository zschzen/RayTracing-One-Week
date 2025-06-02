#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h> /* timespec, clock_gettime */

int
main( void )
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Image dimensions
    const int image_width      = 256;
    const int image_height     = 256;
    const int channels         = 4; // RGBA

    // Allocate image buffer
    const size_t    image_size = image_width * image_height * channels;
    unsigned char * image_data = (unsigned char *)malloc( image_size );
    if( !image_data )
        {
            fprintf( stderr, "Failed to alloc memory\n" );
            return EXIT_FAILURE;
        }

    // Draw
    //--------------------------------------------------------------------------------------
    {
        struct timespec render_start;
        clock_gettime( CLOCK_MONOTONIC, &render_start );

        // Generate image data
        unsigned char * pixel = image_data;
        for( int j = 0; j < image_height; ++j )
            {
                // Print progress indicator
                fprintf( stdout, "\rScanlines remaining: %d", image_height - j );
                fflush( stdout );

                for( int i = 0; i < image_width; ++i )
                    {
                        // Calculate color values
                        double r = (double)i / ( image_width - 1 );
                        double g = (double)j / ( image_height - 1 );
                        double b = 0.0;
                        double a = 1.0; // Fully opaque alpha

                        // Set pixel components
                        *pixel++ = (unsigned char)( 255.999 * r ); // R
                        *pixel++ = (unsigned char)( 255.999 * g ); // G
                        *pixel++ = (unsigned char)( 255.999 * b ); // B
                        *pixel++ = (unsigned char)( 255.999 * a ); // A
                    }
            }

        // Calculate render time
        struct timespec render_end;
        clock_gettime( CLOCK_MONOTONIC, &render_end );
        double render_time = (double)( render_end.tv_sec - render_start.tv_sec )
                             + (double)( render_end.tv_nsec - render_start.tv_nsec ) / 1000000000.0;

        // Rendering time
        fprintf( stdout, "\rRendering completed in %.4f seconds\n", render_time );
    }

    // Output
    //--------------------------------------------------------------------------------------
    {
        const char * filename = "output.png";
        if( !stbi_write_png( filename, image_width, image_height, channels, image_data, image_width * channels ) )
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
