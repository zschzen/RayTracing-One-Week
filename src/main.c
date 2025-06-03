#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.h"
#include "vec3.h"

#include <stdio.h>
#include <stdlib.h>

int
main( void )
{
    // Initialization
    //--------------------------------------------------------------------------------------
    // Image dimensions
    const int image_width      = 256;
    const int image_height     = 256;
    const int channels         = 3; // RGB

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
        // Generate image data
        unsigned char * pixel = image_data;
        for( int j = 0; j < image_height; ++j )
            {
                // Print progress indicator
                fprintf( stderr, "\rScanlines remaining: %d ", image_height - j );
                fflush( stderr );

                for( int i = 0; i < image_width; ++i )
                    {
                        // Calculate color values (normalized to [0,1])
                        double r          = (double)i / ( image_width - 1 );
                        double g          = (double)j / ( image_height - 1 );
                        double b          = 0.0;

                        // Create color using vec3
                        color pixel_color = vec3_new( r, g, b );

                        // Use the color.h function to write to buffer
                        write_color_to_buffer( pixel, pixel_color );

                        // Move to next pixel
                        pixel += channels;
                    }
            }

        fprintf( stderr, "\rDone.                             \n" );
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
