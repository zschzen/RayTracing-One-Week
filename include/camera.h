#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h" /* ray struct, ray_create, ray_origin, ray_direction, ray_at */

// Forward declaration
struct hittable;

typedef struct
{
    // --- Transform ---
    point3 position; // Camera position in world space
    point3 target;   // Point camera is looking at
    vec3   world_up; // Camera-relative "up" direction

    // --- Camera optics ---
    double vertical_fov_deg; // Vertical field-of-view in degrees
    double aspect_ratio;     // Ratio of image width to height
    double aperture;         // Lens aperture diameter for depth of field
    double focal_distance;   // Distance to plane of perfect focus

    // --- Rendering ---
    int image_width;
    int samples_per_pixel;
    int max_depth;

    // --- Calculated ---
    vec3   right, up, forward; // Orthonormal basis for camera orientation (right, up, -direction)
    vec3   viewport_width;     // Horizontal extent of viewport in world space
    vec3   viewport_height;    // Vertical extent of viewport in world space
    point3 viewport_origin;    // Top-left corner of viewport in world space
    double lens_radius;        // Half of aperture (for sampling)
    int    image_height;       // Calculated image height

} camera;

// Initializes camera with positioning, optics, and depth of field settings.
// Computes internal coordinate system and viewport geometry from parameters.
//
// Parameters:
//  cam: Pointer to camera struct to initialize
//  aspect_ratio: Image width/height ratio
//  vertical_fov_deg: Vertical field of view in degrees
//  camera_pos: World position of the camera
//  target_point: World point the camera looks toward
//  world_up_dir: World space "up" direction vector (typically (0,1,0))
//  aperture_diameter: Lens diameter for depth of field blur
//  focus_distance: Distance to plane of sharpest focus
void camera_init( camera * cam, double aspect_ratio, double vertical_fov_deg, point3 camera_pos, point3 target_point,
                  vec3 world_up_dir, double aperture_diameter, double focus_distance,
                  // Rendering
                  int image_width, int samples_per_pixel, int max_depth );

// Renders the entire scene to the provided image data buffer
void camera_render( const camera * cam, const struct hittable * world, unsigned char * image_data );

// Generates a ray from the camera through a point (s, t) on the image plane.
// s and t are normalized pixel coordinates (0 to 1, where (0,0) is top-left).
ray camera_get_ray( const camera * cam, double s, double t );

#endif // CAMERA_H
