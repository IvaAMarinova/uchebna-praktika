#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

float calculate_ticket_price(int totalSeats, float totalRevenue, int seatLevel) {
    float rootPrice = totalRevenue / totalSeats;
    int priceFactor = 1 << seatLevel; // Calculate price factor using bitwise left shift operator

    float ticketPrice = rootPrice * priceFactor;
    if (ticketPrice < 1.0) {
        ticketPrice = 1.0;
    }

    return ticketPrice;
}


void generate_tickets(size_t capacity, float revenue, FILE *artist)
{
    float sum = 0;
    int rows = (int)(log2(capacity) + 1);
    int seats_in_row = 1;

    for(size_t i = 1; i <= rows; i++, seats_in_row *= 2) {
        fprintf(artist, "Row %zu: - %.2f\n", i, calculate_ticket_price(capacity, revenue, i));
        sum += calculate_ticket_price(capacity, revenue, i) * seats_in_row;
    }
    fprintf(artist, "Total: %.2f\n", sum);
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
    fprintf(artist, "Bought tickets: 0\n");
    fprintf(artist, "\n");

    generate_tickets(capacity, revenue, artist); // doesnt currently work :)
    fprintf(artist, "\n");

    for(size_t i = 1; i <= capacity; i++) {
        fprintf(artist, "%zu - 0\n", i);
    }

    free(file_name);
    fclose(artist);
    return 0;
}

int make_concert_public(const char *artist_name, const char *date) {
    char *file_name = file_name_generator(artist_name);
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char line[100];
    char concert_date[11];

    while (fgets(line, sizeof(line), artist)) {
        if (sscanf(line, "Date: %10s", concert_date) == 1) {
            if (strcmp(concert_date, date) == 0) {
                fseek(artist, -2, SEEK_CUR);
                fprintf(artist, "1\n");
                fclose(artist);
                free(file_name);
                return 1;
            }
        }
    }
    return -1;
}

int main()
{
    create_concert(15, 100000.0, "Lili Ivanova", "01.01.2020", "Sofia", '0');
    make_concert_public("Lili Ivanova", "01.01.2020");

    return 0;
}
