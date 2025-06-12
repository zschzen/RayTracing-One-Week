#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

#include <stdlib.h> /* rand, RAND_MAX */

typedef struct
{
    // Positioning and orientation
    point3 position; // Camera position in world space
    point3 target;   // Point camera is looking at
    vec3   world_up; // Camera-relative "up" direction

    // Camera optics
    double vertical_fov_deg; // Vertical field-of-view in degrees
    double aspect_ratio;     // Ratio of image width to height
    double aperture;         // Lens aperture diameter for depth of field
    double focal_distance;   // Distance to plane of perfect focus

    // Calculated private parameters
    point3 origin;             // Camera position (copy of position for convenience)
    vec3   right, up, forward; // Orthonormal basis for camera orientation (right, up, -direction)
    vec3   viewport_width;     // Horizontal extent of viewport in world space
    vec3   viewport_height;    // Vertical extent of viewport in world space
    point3 viewport_origin;    // Top-left corner of viewport in world space
    double lens_radius;        // Half of aperture (for sampling)

} camera;

// Initializes camera with positioning, optics, and depth of field settings.
// Computes internal coordinate system and viewport geometry from parameters.
//
// Parameters:
//   cam: Pointer to camera struct to initialize
//   aspect_ratio: Image width/height ratio
//   vertical_fov_deg: Vertical field of view in degrees
//   camera_pos: World position of the camera
//   target_point: World point the camera looks toward
//   world_up_dir: World space "up" direction vector (typically (0,1,0))
//   aperture_diameter: Lens diameter for depth of field blur
//   focus_distance: Distance to plane of sharpest focus
void camera_init( camera * cam, double aspect_ratio, double vertical_fov_deg, point3 camera_pos, point3 target_point,
                  vec3 world_up_dir, double aperture_diameter, double focus_distance );

// Generates a ray from the camera through a point (s, t) on the image plane.
// s and t are normalized pixel coordinates (0 to 1, where (0,0) is top-left).
ray camera_get_ray( const camera * cam, double s, double t );

// Random double in [0,1)
static inline double
random_double_camera()
{
    return rand() / ( RAND_MAX + 1.0 );
}

// Generates a random point in a unit disk.
static inline vec3
random_in_unit_disk()
{
    while( true )
        {
            vec3 p = vec3_new( random_double_camera() * 2.0 - 1.0, random_double_camera() * 2.0 - 1.0, 0.0 );
            if( vec3_length_squared( p ) < 1.0 )
                {
                    return p;
                }
        }
}

#endif // CAMERA_H
