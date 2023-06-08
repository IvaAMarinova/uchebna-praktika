#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *file_name_generator(const char *name, const char *type)
{
    char *file_name = malloc(strlen(name) + 11);
    if (file_name == NULL) {
        printf("Error allocating memory.\n");
        return NULL;
    }

    if(strcmp(type, "artist") == 0) { 
        strcpy(file_name, "artists/"); 
    } else {
        strcpy(file_name, "fans/");
    }

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
/*
void generate_tickets(size_t capacity, float revenue, FILE *artist)
{
    int rows = (int)(log2(capacity) + 1);
    //float seat_prices[rows];
    float seat = 0, prev_seat = 0, sum = 0;
    size_t middle_seat_index = rows / 2;

    // set the prices of the last row as the minimum
    // price we can possibly give a seat and still hit revenue

    // we set the seat as the middle seat
    seat = revenue / capacity;
    //seat_prices[middle_seat_index] = seat;

    prev_seat = seat;
    sum = seat;
    
    // we go from middle seat to root seat
    for (int i = middle_seat_index - 1; i >= 0; i--) {
        seat = prev_seat * (1 + 0.01 * ((float)rows - i) * 2); 
        sum += seat * pow(2, i);
        seat_prices[i] = seat;
        prev_seat = seat;
    }

    // we go from middle seat to last seat
    prev_seat = seat_prices[middle_seat_index];
    for (int i = middle_seat_index + 1; i < rows; i++) {
        seat = prev_seat * (1 - 0.01 * ((float)rows - i) / 2); 
        sum += seat * pow(2, i);
        seat_prices[i] = seat;
        prev_seat = seat;
    }

    for (int i = 0; i < rows; i++)
    {
        fprintf(artist, "Row %d: %0.01f\n", i + 1, seat_prices[i]);
    }

    fprintf(artist, "Sum: %0.01f\n", sum);
}

int create_concert(size_t capacity, float revenue,
    const char *artist_name, const char *date, const char *location, int state)
{
    FILE *artist;
    char new_artists = 0;

    char *file_name = file_name_generator(artist_name, "artist");
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
        fprintf(artist, "////////////////////////////\n");
    }

    fseek(artist, 1, SEEK_END);
    fprintf(artist, "Location: %s\n", location);
    fprintf(artist, "Date: %s\n", date);
    fprintf(artist, "Capacity: %zu\n", capacity);
    fprintf(artist, "Revenue: %.2f\n", revenue);
    fprintf(artist, "State: %d\n", state);
    fprintf(artist, "Bought tickets: 0\n");
    fprintf(artist, "\n");

    generate_tickets(capacity, revenue, artist);
    
    fprintf(artist, "\n");

    for(size_t i = 1; i <= capacity; i++) {
        fprintf(artist, "%zu - 0\n", i);
    }

    fprintf(artist, "////////////////////////////\n"); // marks end of concert

    free(file_name);
    fclose(artist);
    return 0;
}

int make_concert_public(const char *artist_name, const char *date) {
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            int counter = 0;
            while (fgets(line, sizeof(line), artist)) {
                counter++;
                if(counter == 3)
                {
                    fseek(artist, -2, SEEK_CUR);
                    fprintf(artist, "1");
                    fclose(artist);
                    free(date_formated);
                    free(file_name);
                    return 1;
                }
            }
        }
    }

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
}

int edit_location(const char *artist_name, const char *date, 
    const char *new_location)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    char line[100], prev_line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            fseek(artist, -strlen(line) -strlen(prev_line), SEEK_CUR);
            fprintf(artist, "Location: %s\n", new_location);
            break;
        }
        strcpy(prev_line, line);
    }

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
}

float find_revenue(const char* artist_name, const char *date)
{

}

void recalculate_tickets(const char *artist_name, const char *date, size_t new_capacity)
{
    // delete rows with tickets
    float revenue = find_revenue(artist_name, date);
    generate_tickets(new_capacity, revenue, NULL);
}

int edit_date(const char *artist_name, const char *date, const char *new_date)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            fseek(artist, -strlen(line), SEEK_CUR);
            fprintf(artist, "Date: %s\n", new_date);
            break;
        }
    }

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
}

int edit_capacity(const char *artist_name, const char *date, size_t new_capacity) // not working
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            fgets(line, sizeof(line), artist);
            printf("%s\n", line);
            size_t len = strlen(line);
            fseek(artist, -len, SEEK_CUR);
            for(int i = 0; i < len; i++)
            {
                fprintf(artist, " ");
            }
            fseek(artist, -len, SEEK_CUR);
            fprintf(artist, "Capacity: %zu\n", new_capacity);
            break;
        }
    }


    // add recalculating of tickets

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
}
int edit_revenue(const char *artist_name, const char *date, float new_revenue);
int delete_concert(const char *artist_name, const char *date) {

}


int main()
{
    //create_concert(100, 1000, "Lili Ivanova", "11.11.1111", "Sofia", 0);
    edit_capacity("Lili Ivanova", "11.11.1111", 15);

    return 0;
}
*/