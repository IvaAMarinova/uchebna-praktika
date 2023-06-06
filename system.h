#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

int create_concert(size_t capacity, float revenue, 
    char *artist_name, char *date, char *location, int state);
int make_concert_public(const char *artist_name, const char *date);
char *file_name_generator(const char *name, const char *type);
int delete_concert(const char *artist_name, const char *date);
int file_exists(const char *file_name);

// functions for editing concert info
// CONCERT MUST STILL BE PRIVATE
int edit_location(const char *artist_name, const char *date, 
    const char *new_location);
int edit_date(const char *artist_name, const char *date, const char *new_date);
int edit_capacity(const char *artist_name, const char *date, size_t new_capacity);
int edit_revenue(const char *artist_name, const char *date, float new_revenue);