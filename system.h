#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

// done
int create_user(const char *artist_name, const char *password, const char *safe_message, const char *type);
int create_concert(size_t capacity, float revenue, 
    const char *artist_name, const char *date, const char *location, int state);
int make_concert_public(const char *artist_name, const char *date);
char *file_name_generator(const char *name, const char *type);
int file_exists(const char *file_name);
int print_concert_info(const char *artist_name, const char *date);
int print_artist_info(const char *artist_name);
int print_artists_lineup();

// functions for editing concert info
// CONCERT MUST STILL BE PRIVATE
int edit_location(const char *artist_name, const char *date, 
    const char *new_location);
int edit_date(const char *artist_name, const char *date, const char *new_date);
int edit_capacity(const char *artist_name, const char *date, size_t new_capacity);
int edit_revenue(const char *artist_name, const char *date, float new_revenue);
int delete_concert(const char *artist_name, const char *date);



// left to do
float offer_ticket(const char *artist_name, const char *date, float wanted_price, size_t *row, float *possible_price);
int buy_ticket(const char *artist_name, const char *date, size_t row);