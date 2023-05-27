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

void generate_tickets(size_t capacity, float revenue, FILE *artist)
{
    int rows = (int)(log2(capacity) + 1);
    float seat_prices[rows];
    float seat = 0, prev_seat = 0, sum = 0;
    size_t middle_seat_index = rows / 2;

    // set the prices of the last row as the minimum
    // price we can possibly give a seat and still hit revenue

    // we set the seat as the middle seat
    seat = revenue / capacity;
    seat_prices[middle_seat_index] = seat;

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
    fprintf(artist, "\n");
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

int delete_concert(const char *artist_name, const char *date) {
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
    long int delete_start = -1;
    long int delete_end = -1;
    long int line_start = 0;
    long int line_end = 0;
    long int truncate_pos = 0;
    int found_end_line = 0;

    while (fgets(line, sizeof(line), artist)) {
        line_end = ftell(artist);

        if (sscanf(line, "Date: %10s", concert_date) == 1) {
            if (strcmp(concert_date, date) == 0) {
                delete_start = line_start;
                delete_end = line_end;
                break;
            }
        }

        if (strcmp(line, "////////////////////////////\n") == 0) {
            found_end_line = 1;
            break;
        }

        line_start = line_end;
    }

    if (delete_start != -1 && delete_end != -1) {
        if (found_end_line) {
            truncate_pos = delete_start;
        } else {
            // Move to the end of the file
            fseek(artist, 0, SEEK_END);
            truncate_pos = ftell(artist);
        }

        fseek(artist, 0, SEEK_SET);
        FILE *temp = tmpfile();

        if (temp == NULL) {
            fclose(artist);
            free(file_name);
            return -1;
        }

        char ch;
        while (ftell(artist) < truncate_pos && (ch = fgetc(artist)) != EOF) {
            fputc(ch, temp);
        }

        fclose(artist);
        fclose(temp);

        if (rename(file_name, "temp.txt") != 0) {
            free(file_name);
            return -1;
        }

        if (rename("temp.txt", file_name) != 0) {
            free(file_name);
            return -1;
        }

        free(file_name);
        return 0;
    }

    fclose(artist);
    free(file_name);
    return -1;
}




int main()
{
    create_concert(100, 4500, "Eminem", "10.12.2012", "Sofia", 'a');

    return 0;
}
