#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file_name_generator(const char *name)
{
    char *file_name = malloc(strlen(name) + 11);
    if (file_name == NULL) {
        printf("Error allocating memory.\n");
        return NULL;
    }

    strcpy(file_name, "artists/");
    strcat(file_name, name);
    size_t len = strlen(file_name);
    for (int i = 0; i < len; i++) {
    if (file_name[i] == ' ') {
        if (file_name[i + 1] != '\0') {
            int j;
            for (j = i + 1; j <= len; j++) {
                file_name[j - 1] = file_name[j];
            }
            i--;
        }
    }
    }
    strcat(file_name, ".txt");
    return file_name;
}

int file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0; 
    }
}

int create_concert(size_t capacity, float revenue,
    const char *artist_name, const char *date, const char *location, char state)
{
    FILE *artist;
    char new_artists = 0;

    char *file_name = file_name_generator(artist_name);
    if (file_name == NULL) {
        return -1;
    }

    if (file_exists(file_name) == 0) {
        new_artists = 1;
    }

    artist = fopen(file_name, "a");
    if (artist == NULL) {
        printf("Error opening file.\n");
        free(file_name);
        return -1;
    }

    if (new_artists == 1) {
        fprintf(artist, "Name: %s\n", artist_name);
        fprintf(artist, "\n");
    }

    fseek(artist, 0, SEEK_END);
    fprintf(artist, "Location: %s\n", location);
    fprintf(artist, "Date: %s\n", date);
    fprintf(artist, "Capacity: %zu\n", capacity);
    fprintf(artist, "Revenue: %.2f\n", revenue);
    fprintf(artist, "State: %c\n", state);
    fprintf(artist, "\n");

    free(file_name);
    fclose(artist);
    return 0;
}

int main()
{
    create_concert(1000, 100000.0, "Lili Ivanova", "01.01.2020", "Sofia", '0');
    create_concert(1002, 200000.0, "Lili Ivanova", "02.02.2020", "Sofia", '0');
    return 0;
}
