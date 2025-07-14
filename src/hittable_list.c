#include "hittable_list.h"
#include "hittable.h"

// Default initial capacity
#define DEFAULT_CAPACITY 4

void
hittable_list_init( hittable_list * list, int initial_capacity )
{
    if( NULL == list ) return;

    list->base.hit = hittable_list_hit; // Set the hit function
    list->count    = 0;
    list->capacity = ( initial_capacity > 0 ) ? initial_capacity : DEFAULT_CAPACITY;
    list->objects  = (hittable **)malloc( list->capacity * sizeof( hittable * ) );

    if( NULL == list->objects )
        {
            fprintf( stderr, "ERROR: Failed to allocate memory for hittable_list objects array.\n" );
            list->capacity = 0;
            list->count    = 0;
        }
}

bool
hittable_list_add( hittable_list * list, hittable * object )
{
    // Attempt a late initialization
    if( NULL == list || NULL == list->objects )
        {
            if( list && list->capacity == 0 && list->count == 0 )
                {
                    hittable_list_init( list, 4 );
                    if( NULL == list->objects ) return false;
                }
            else
                {
                    return false;
                }
        }

    // Cannot add a NULL object
    if( NULL == object ) return false;

    // Resize by doubling
    if( list->count >= list->capacity )
        {
            int         new_capacity = ( list->capacity == 0 ) ? 4 : list->capacity * 2;
            hittable ** new_objects  = (hittable **)realloc( list->objects, new_capacity * sizeof( hittable * ) );

            if( NULL == new_objects )
                {
                    fprintf( stderr, "ERROR: Failed to reallocate memory for hittable_list objects array.\n" );
                    return false; // Failed to resize
                }

            list->objects  = new_objects;
            list->capacity = new_capacity;
        }

    list->objects[list->count++] = object;
    return true;
}

void
hittable_list_clear( hittable_list * list )
{
    if( NULL == list ) return;

    for( int i = 0; i < list->count; ++i )
        {
            if( list->objects[i] )
                {
                    // Get hittable object
                    hittable * obj = (hittable *)list->objects[i];

                    // Free the material if it exists
                    if( obj->mat_ptr )
                        {
                            free( obj->mat_ptr );
                            obj->mat_ptr = NULL;
                        }

                    // Free the object itself
                    free( obj );
                    list->objects[i] = NULL;
                }
        }

    // Free the objects array
    free( list->objects );
    list->objects  = NULL;
    list->count    = 0;
    list->capacity = 0;
}

bool
hittable_list_hit( const hittable * list_hittable, const ray * r, double ray_tmin, double ray_tmax, hit_record * rec )
{
    // Cast the generic hittable to hittable_list
    const hittable_list * list = (const hittable_list *)list_hittable;
    hit_record            temp_rec;
    bool                  hit_anything   = false;
    double                closest_so_far = ray_tmax; // Maximum allowed t for intersection

    for( int i = 0; i < list->count; ++i )
        {
            hittable * current_object = list->objects[i];
            if( current_object && current_object->hit )
                {
                    // Pass closest_so_far as the ray_tmax for the current object,
                    // ensuring that we only record hits that are closer than previous ones
                    if( current_object->hit( current_object, r, ray_tmin, closest_so_far, &temp_rec ) )
                        {
                            hit_anything   = true;
                            closest_so_far = temp_rec.t; // Update closest_so_far
                            *rec           = temp_rec;   // Copy the hit record of the closest object
                        }
                }
        }

    return hit_anything;
}
