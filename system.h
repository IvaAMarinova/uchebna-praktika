#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int create_concert(size_t capacity, float revenue, 
    char *artist_name, char *date, char *location, char state);
int make_concert_public(const char *artist_name, const char *date);
char *file_name_generator(const char *name);
int delete_concert(const char *artist_name, const char *date);

