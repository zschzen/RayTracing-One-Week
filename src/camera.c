#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "rtweekend.h"
#include <float.h>
#include <math.h> /* tan, M_PI */
#include <stdio.h>

// Computes the color for a given ray
static color
ray_color( const ray * r, const hittable * world, int depth )
{
    hit_record rec;

    if( depth <= 0 )
        {
            return vec3_new( 0, 0, 0 ); // Ray bounce limit reached
        }

    // Check for hits
    // t_min = 0.001 to avoid shadow acne
    if( world->hit( world, r, 0.001, RT_INFINITY, &rec ) )
        {
            ray   scattered;
            color attenuation;
            if( rec.mat_ptr && rec.mat_ptr->scatter( rec.mat_ptr, r, &rec, &attenuation, &scattered ) )
                {
                    return vec3_mul_vec( attenuation, ray_color( &scattered, world, depth - 1 ) );
                }
            return vec3_new( 0, 0, 0 ); // Ray was absorbed
        }

    // Background gradient
    vec3   unit_direction = vec3_normalize( ray_direction( r ) );
    double a              = 0.5 * ( unit_direction.y + 1.0 );

    color white           = vec3_new( 1.0, 1.0, 1.0 );
    color sky_blue        = vec3_new( 0.5, 0.7, 1.0 );
    return vec3_add( vec3_mul( white, 1.0 - a ), vec3_mul( sky_blue, a ) );
}

// Initializes the camera with given parameters.
void
camera_init( camera * cam, double aspect_ratio, double vertical_fov_deg, point3 camera_pos, point3 target_point,
             vec3 world_up_dir, double aperture_diameter, double focus_distance,
             // New render parameters
             int image_width, int samples_per_pixel, int max_depth )
{
    if( !cam ) return;

    cam->aspect_ratio      = aspect_ratio;
    cam->vertical_fov_deg  = vertical_fov_deg;
    cam->position          = camera_pos;
    cam->target            = target_point;
    cam->world_up          = world_up_dir;
    cam->aperture          = aperture_diameter;
    cam->focal_distance    = focus_distance;
    cam->lens_radius       = cam->aperture / 2.0;

    cam->image_width       = image_width;
    cam->samples_per_pixel = samples_per_pixel;
    cam->max_depth         = max_depth;
    cam->image_height      = (int)( image_width / aspect_ratio );
    cam->image_height      = RT_MAX( cam->image_height, 1 );

    double theta           = vertical_fov_deg * RT_DEG2RAD;
    double h               = tan( theta / 2.0 );
    double viewport_h      = 2.0 * h;
    double viewport_w      = aspect_ratio * viewport_h;

    cam->forward           = vec3_normalize( vec3_sub( target_point, camera_pos ) );
    cam->right             = vec3_normalize( vec3_cross( cam->forward, world_up_dir ) );
    cam->up                = vec3_cross( cam->right, cam->forward );

    cam->viewport_width    = vec3_mul( cam->right, viewport_w * focus_distance );
    cam->viewport_height   = vec3_mul( cam->up, viewport_h * focus_distance );

    vec3 half_width        = vec3_div( cam->viewport_width, 2.0 );
    vec3 half_height       = vec3_div( cam->viewport_height, 2.0 );
    vec3 focus_center      = vec3_add( camera_pos, vec3_mul( cam->forward, focus_distance ) );

    cam->viewport_origin   = vec3_sub( focus_center, half_width );
    cam->viewport_origin   = vec3_add( cam->viewport_origin, half_height );
}

void
camera_render( const camera * cam, const struct hittable * world, unsigned char * image_data )
{
    unsigned char * pixel = image_data;
    for( int j = 0; j < cam->image_height; ++j )
        {
            fprintf( stderr, "\rScanlines remaining: %d ", cam->image_height - j - 1 );
            fflush( stderr );

            for( int i = 0; i < cam->image_width; ++i )
                {
                    color pixel_color = vec3_new( 0, 0, 0 );
                    for( int s = 0; s < cam->samples_per_pixel; ++s )
                        {
                            double u    = (double)( i + random_double() ) / ( cam->image_width - 1 );
                            double v    = (double)( j + random_double() ) / ( cam->image_height - 1 );
                            ray    r    = camera_get_ray( cam, u, v );
                            pixel_color = vec3_add( pixel_color, ray_color( &r, (hittable *)world, cam->max_depth ) );
                        }
                    write_color_to_buffer( pixel, pixel_color, cam->samples_per_pixel );
                    pixel += RT_IMAGE_DATA_CHANNELS;
                }
        }

    fprintf( stderr, "\rDone.                                                      \n" );
}

ray
camera_get_ray( const camera * cam, double s, double t )
{
    if( !cam )
        {
            // Return a default ray pointing forward
            // TODO: Handle error properly
            return ray_create( vec3_new( 0, 0, 0 ), vec3_new( 0, 0, -1 ) );
        }

    // Calculate point on viewport corresponding to screen coordinates (s,t)
    vec3 horizontal_offset = vec3_mul( cam->viewport_width, s );
    vec3 vertical_offset   = vec3_mul( cam->viewport_height, -t ); // Negative t: screen Y down, camera up vector up

    point3 viewport_point  = vec3_add( cam->viewport_origin, horizontal_offset );
    viewport_point         = vec3_add( viewport_point, vertical_offset );

    // Apply depth of field by sampling random point on lens aperture
    vec3   lens_sample     = vec3_mul( random_in_unit_disk(), cam->lens_radius );
    vec3   lens_offset     = vec3_add( vec3_mul( cam->right, lens_sample.x ), vec3_mul( cam->up, lens_sample.y ) );
    point3 ray_start       = vec3_add( cam->position, lens_offset );

    vec3 ray_dir           = vec3_sub( viewport_point, ray_start );

    return ray_create( ray_start, ray_dir );
}
