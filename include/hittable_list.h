#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h" /* struct, hit_record, hit_fn */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>   /* malloc, realloc, free */

typedef struct
{
    hittable    base;     // base.hit will point to hittable_list_hit
    hittable ** objects;  // Dynamic array of pointers to hittable objects
    size_t      count;    // Number of objects currently in the list
    size_t      capacity; // Allocated capacity of the objects array
} hittable_list;

// Initializes a hittable_list
void hittable_list_init( hittable_list * list, int initial_capacity );

// Adds a pointer to a hittable object to the list
// Returns true on success, false on failure.
bool hittable_list_add( hittable_list * list, hittable * object );

// Clears the list: frees the internal array of pointers.
// WARN: Objects must be freed independently
void hittable_list_clear( hittable_list * list );

// Iterates through all objects in the list and checks for the closest hit
bool hittable_list_hit( const hittable * list_hittable, const ray * r, double ray_tmin, double ray_tmax,
                        hit_record * rec );

#endif // !HITTABLE_LIST_H
