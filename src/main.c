#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

int
main( void )
{
    // Image dimensions
    const int image_width      = 256;
    const int image_height     = 256;
    const int channels         = 4; // RGBA

    // Allocate image buffer
    const size_t    image_size = image_width * image_height * channels;
    unsigned char * image_data = (unsigned char *)malloc( image_size );
    if( !image_data )
        {
            return EXIT_FAILURE;
        }

    // Generate image data
    for( int j = 0; j < image_height; j++ )
        {
            for( int i = 0; i < image_width; i++ )
                {
                    // Calculate pixel index
                    const int index       = ( j * image_width + i ) * channels;

                    // Calculate color values
                    double r              = (double)i / ( image_width - 1 );
                    double g              = (double)j / ( image_height - 1 );
                    double b              = 0.0;
                    double a              = 1.0; // Fully opaque alpha

                    // Set pixel components using array index
                    image_data[index + 0] = (unsigned char)( 255.999 * r ); // R
                    image_data[index + 1] = (unsigned char)( 255.999 * g ); // G
                    image_data[index + 2] = (unsigned char)( 255.999 * b ); // B
                    image_data[index + 3] = (unsigned char)( 255.999 * a ); // A
                }
        }

    // Write output PNG file
    const char * filename = "output.png";
    if( !stbi_write_png( filename, image_width, image_height, channels, image_data, image_width * channels ) )
        {
            fprintf( stderr, "Failed to write output image\n" );
            free( image_data );
            return EXIT_FAILURE;
        }

    printf( "Successfully wrote output image to %s\n", filename );
    free( image_data );

    return EXIT_SUCCESS;
}
