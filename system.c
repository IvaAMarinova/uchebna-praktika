#include "system.h"

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

int create_user(const char *name, const char *password, const char *safe_message, const char *type)
{
    char *file_name = file_name_generator(name, type);
    if (file_name == NULL) {
        return -1;
    }

    if (file_exists(file_name) == 1) {
        printf("Artist already exists.\n");
        free(file_name);
        return -1;
    }

    FILE *artist = fopen(file_name, "w");
    if (artist == NULL) {
        printf("Error opening file.\n");
        free(file_name);
        return -1;
    }

    fprintf(artist, "Name: %s\n", name);
    fprintf(artist, "Password: %s\n", password);
    fprintf(artist, "Safe message: %s\n", safe_message);
    if(type == "fan")
    {
        fprintf(artist, "Tickets: \n");
    }
    fprintf(artist, "////////////////////////////\n");

    free(file_name);
    fclose(artist);
    return 1;
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

    artist = fopen(file_name, "a");

    if (artist == NULL) {
        printf("Error opening file.\n");
        free(file_name);
        return -1;
    }

    fseek(artist, 1, SEEK_END);
    fprintf(artist, "Date: %s\n", date);
    fprintf(artist, "Location: %s\n", location);
    fprintf(artist, "Capacity: %zu\n", capacity);
    fprintf(artist, "Revenue: %.2f\n", revenue);
    fprintf(artist, "State: %d\n", state);
    fprintf(artist, "\n");

    generate_tickets(capacity, revenue, artist);
    
    fprintf(artist, "\n");

    for(size_t i = 1; i <= capacity; i++) {
        fprintf(artist, "%zu - 0\n", i);
    }

    fprintf(artist, "////////////////////////////\n"); // marks end of concert

    free(file_name);
    fclose(artist);
    return 1;
}

int make_concert_public(const char *artist_name, const char *date) 
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
            int counter = 0;
            while (fgets(line, sizeof(line), artist)) {
                counter++;
                if(counter == 4)
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

int is_concert_public(const char *artist_name, const char *date) // 0 -no, 1 - yes, -1 - error
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r");
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
                if(counter == 4)
                {
                    fclose(artist);
                    free(date_formated);
                    free(file_name);
                    return line[0] - '0';
                }
            }
        }
    }

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
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

    if(is_concert_public(artist_name, date) == 1) {
        fclose(artist);
        free(file_name);
        printf("This concert is public and you can't edit it.\n");
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
            return 1;
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

    if(is_concert_public(artist_name, date) == 1) {
        fclose(artist);
        free(file_name);
        printf("This concert is public and you can't edit it.\n");
        return -1;
    }

    char *date_formatted = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formatted, "Date: ");
    strcat(date_formatted, date);

    char line[100], prev_line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formatted, strlen(date_formatted)) == 0) {
            printf("found\n");
            fgets(line, sizeof(line), artist);
            size_t len = strlen(line);

            fseek(artist, -len, SEEK_CUR);
            fprintf(artist, "Location: %s", new_location);

            if (strlen(new_location) < len - 1) {
                for (size_t i = strlen(new_location) + strlen("Location: "); i < len - 1; i++) {
                    fprintf(artist, " ");
                }
            }
            return 1;
        }
        strcpy(prev_line, line);
    }

    fclose(artist);
    free(date_formatted);
    free(file_name);
    return -1;
}

float find_revenue(FILE *artist, const char *date_formated) {
    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            fgets(line, sizeof(line), artist);
            fgets(line, sizeof(line), artist);
            fgets(line, sizeof(line), artist);
            fseek(artist, -strlen(line), SEEK_CUR);
            return atof(line);
        }
    }
    return -1.0;
}

void recalculate_tickets(FILE *artist, size_t capacity, float revenue, char const *file_name)
{
    FILE *artist_temp = fopen("artists/temp.txt", "a"), *beginning = fopen(file_name, "r+");
    if (artist_temp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[100];
    while(beginning != artist) {   
        fgets(line, sizeof(line), beginning);
        fprintf(artist_temp, "%s", line);
    }

    generate_tickets(capacity, 0, artist_temp);

    while(fgets(line, sizeof(line), beginning)) {
        while(strncmp(line, "////////////////////////////\n", strlen("////////////////////////////\n")) != 0) {
            fgets(line, sizeof(line), beginning);
        }
    }

    while(fgets(line, sizeof(line), beginning)) {
        fprintf(artist_temp, "%s", line);
    }

    rename("artists/temp.txt", file_name);
    fclose(artist_temp);
}

int edit_capacity(const char *artist_name, const char *date,
    size_t new_capacity)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+"), *temp = fopen("artists/temp.txt", "w");
    FILE *beginnig_artist = fopen(file_name, "r");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    if(is_concert_public(artist_name, date) == 1) {
        fclose(artist);
        free(file_name);
        printf("This concert is public and you can't edit it.\n");
        return -1;
    }

    char *date_formatted = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formatted, "Date: ");
    strcat(date_formatted, date);

    char line[100];
    char found_flag = 0;
    while (fgets(line, sizeof(line), artist)) {
        fprintf(temp, "%s", line);
        if (strncmp(line, date_formatted, strlen(date_formatted)) == 0) {
            printf("found\n");
            found_flag = 1;  
            break;
        }
    }

    if(found_flag == 1)
    {
        fgets(line, sizeof(line), artist); // Location
        fprintf(temp, "%s", line);

        fgets(line, sizeof(line), artist); // Capacity
        size_t len = strlen(line);

        fseek(artist, -len, SEEK_CUR);
        fprintf(temp, "Capacity: %lu\n", new_capacity);
        printf("Capacity: %lu\n", new_capacity);

        int count_chars_new_cap = 0;
        for(; new_capacity < 0; count_chars_new_cap++)
        {
            new_capacity /= 10;
        }
    
        if (count_chars_new_cap + strlen("Capacity: ") < len - 2) {
            for (size_t i = count_chars_new_cap + strlen("Capacity: "); i < len - 1; i++) {
                fprintf(temp, " ");
            }
        }  
    } else {
        fclose(artist);
        fclose(temp);
        fclose(beginnig_artist);
        free(date_formatted);
        free(file_name);
        return -1;
    }

    fgets(line, sizeof(line), artist);

    fgets(line, sizeof(line), artist); // Revenue line
    fprintf(temp, "%s", line);
    
    char revenue_str[100];
    for(size_t i = strlen("Revenue: "), j = 0; i < strlen(line) - 1; i++, j++)
    {
        revenue_str[j] = line[i];
    }

    float revenue = atof(revenue_str);
    printf("Revenue: %f\n", revenue);

    fgets(line, sizeof(line), artist);
    fprintf(temp, "%s", line);

    fgets(line, sizeof(line), artist);
    fprintf(temp, "%s", line);

    fprintf(temp, "\n");

    generate_tickets(new_capacity, revenue, temp);

    fprintf(temp, "\n");

    for(size_t i = 1; i <= new_capacity; i++) {
        fprintf(temp, "%zu - 0\n", i);
    }

    fprintf(temp, "////////////////////////////\n"); // marks end of concert

    rename("artists/temp.txt", file_name);
    fclose(artist);
    free(date_formatted);
    free(file_name);
    return -1;
}

int edit_revenue(const char *artist_name, const char *date, float new_revenue)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r+"), *temp = fopen("artists/temp.txt", "w");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    if(is_concert_public(artist_name, date) == 1) {
        fclose(artist);
        free(file_name);
        printf("This concert is public and you can't edit it.\n");
        return -1;
    }

    char *date_formatted = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formatted, "Date: ");
    strcat(date_formatted, date);

    float capacity = 0;

    char line[100];
    char found_flag = 0;
    while (fgets(line, sizeof(line), artist)) {
        fprintf(temp, "%s", line);
        if (strncmp(line, date_formatted, strlen(date_formatted)) == 0) {
            printf("found\n");
            found_flag = 1;  
            break;
        }
    }

    if(found_flag == 1)
    {
        fgets(line, sizeof(line), artist); // Location
        fprintf(temp, "%s", line);

        char capacity_str[100];
        fgets(line, sizeof(line), artist); // Capacity
        fprintf(temp, "%s", line);
        for(size_t i = strlen("Capacity: "), j = 0; i < strlen(line) - 1; i++, j++)
        {
            capacity_str[j] = line[i];
        }
        capacity = atof(capacity_str);

        fgets(line, sizeof(line), artist); // Revenue
        size_t len = strlen(line);  

        fseek(artist, -len, SEEK_CUR);
        fprintf(temp, "Revenue: %.01f", new_revenue);
        printf("Revenue: %f\n", new_revenue);

        int count_chars_new_rev = 0;
        for(; new_revenue < 0; count_chars_new_rev++)
        {
            new_revenue /= 10;
        }
    
        if (count_chars_new_rev + strlen("Revenue: ") < len - 2) {
            for (size_t i = count_chars_new_rev + strlen("Revenue: "); i < len - 1; i++) {
                fprintf(temp, " ");
            }
        }  
        fprintf(temp, "\n");
    } else {
        fclose(artist);
        fclose(temp);
        free(date_formatted);
        free(file_name);
        return -1;
    }

    fgets(line, sizeof(line), artist);

    fgets(line, sizeof(line), artist);
    fprintf(temp, "%s", line);

    fgets(line, sizeof(line), artist);
    fprintf(temp, "%s", line);

    fprintf(temp, "\n");

    generate_tickets(capacity, new_revenue, temp);

    fprintf(temp, "\n");

    for(size_t i = 1; i <= capacity; i++) {
        fprintf(temp, "%zu - 0\n", i);
    }

    fprintf(temp, "////////////////////////////\n");

    rename("artists/temp.txt", file_name);
    fclose(artist);
    free(date_formatted);
    free(file_name);
    return -1;
}

int delete_concert(const char *artist_name, const char *date) 
{
    FILE *temp = fopen("artists/temp.txt", "w"); 
    FILE *beginning = fopen("artists/artists.txt", "r");

    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        fclose(temp); 
        fclose(beginning);
        return -1;
    }

    FILE *artist = fopen(file_name, "r+");
    if (artist == NULL) {
        free(file_name);
        fclose(temp);
        fclose(beginning);
        return -1;
    }

    if(is_concert_public(artist_name, date) == 1) {
        fclose(artist);
        free(file_name);
        printf("This concert is public and you can't edit it.\n");
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    int delete_flag = 0;
    // flag = 1 - nachalo na koncerta, koito triem
    // flag = 2 - kraq na koncerta, koito triem

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (delete_flag == 0 && strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            delete_flag = 1;
            fseek(artist, -strlen(line), SEEK_CUR);
        }

        if (delete_flag == 1 && strncmp(line, "////////////////////////////\n", strlen("////////////////////////////\n")) == 0) {
            fgets(line, sizeof(line), artist);
            delete_flag = 2;
        }
        if (delete_flag == 0 || delete_flag == 2) {
            fprintf(temp, "%s", line);
        }
    }  

    remove(file_name);
    rename("artists/temp.txt", file_name);

    free(date_formated);
    free(file_name);
    fclose(artist);
    fclose(temp);
    return 0;
}

int print_artist_info(const char *artist_name)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        printf("%s", line);
    }

    fclose(artist);
    free(file_name);
    return 1;
}

int print_concert_info(const char *artist_name, const char *date)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    int print_flag = 0;

    char line[100];
    while (fgets(line, sizeof(line), artist)) {
        if (print_flag == 0 && strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            print_flag = 1;
            while(fgets(line, sizeof(line), artist))
            {
                if (strncmp(line, "////////////////////////////\n", strlen("////////////////////////////\n")) == 0) {
                    fclose(artist);
                    free(date_formated);
                    free(file_name);
                    return 1;
                }
                printf("%s", line);
            }
        }    
    }  

    fclose(artist);
    free(date_formated);
    free(file_name);
    return -1;
}

void first_last_of_row(size_t row, size_t *first_seat, size_t *last_seat)
{
    *first_seat = pow(2, row - 1);
    *last_seat = pow(2, row) - 1;
}

float offer_ticket(const char *artist_name, const char *date, float wanted_price, size_t *row, float *possible_price, size_t *seat)
{
    char *file_name = file_name_generator(artist_name, "artist");
    if (file_name == NULL) {
        return -1;
    }

    FILE *artist = fopen(file_name, "r");
    if (artist == NULL) {
        free(file_name);
        return -1;
    }

    char *date_formated = malloc(strlen(date) + strlen("Date: ") + 1);
    strcpy(date_formated, "Date: ");
    strcat(date_formated, date);

    char line[100];
    float rows[18]; // 17 rows is max
    size_t rows_cnt = 0;

    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            while (fgets(line, sizeof(line), artist)) {
                if (strncmp(line, "\n", strlen("\n")) == 0) {
                    size_t len = strlen("Row x: ");
                    for (; strncmp(line, "Sum", strlen("Sum")) != 0; rows_cnt++, fgets(line, sizeof(line), artist)) {
                        fseek(artist, -strlen(line), SEEK_CUR);
                        if (rows_cnt <= 9) {
                            rows[rows_cnt] = atof(line + len);
                        } else {
                            rows[rows_cnt] = atof(line + len + 1);
                        }
                    }
                }
            }
        }
    }

    // looking for the two numbers the given value is between
    if (rows_cnt == 1) {
        fclose(artist);
        free(file_name);
        free(date_formated);
        return rows[0];
    }

    for (size_t i = 0; i < rows_cnt - 1; i++) {
        if (wanted_price < rows[i] && wanted_price > rows[i + 1]) {
            size_t first_seat, last_seat, curr_seat;
            first_last_of_row(i + 1, &first_seat, &last_seat);
            for (curr_seat = first_seat; curr_seat <= last_seat; curr_seat++) {
                char seat[10];
                sprintf(seat, "%zu", curr_seat);
                strcat(seat, " - 0");
                printf("%s\n", seat);
                if (strncmp(line, seat, strlen(seat)) == 0) {
                    *row = i + 1;
                    *possible_price = rows[i];
                    *seat = curr_seat;
                    fclose(artist);
                    free(file_name);
                    free(date_formated);
                    return 1;
                }
            }
        }
    }
    printf("not found\n");
    // concert wasn't found
    fclose(artist);
    free(file_name);
    free(date_formated);
    return -1;
}



int buy_ticket(const char *artist_name, const char *date, size_t row, float *price, size_t *seat)
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

    size_t first_seat, last_seat, curr_seat;
    first_last_of_row(row, &first_seat, &last_seat);
    last_seat++; // purvi seat na drugiq red
    char str_first_seat[10], str_last_seat[10];
    sprintf(str_first_seat, "%zu -", first_seat);
    sprintf(str_last_seat, "%zu -", last_seat);

    printf("%s\n", str_first_seat);
    printf("%s\n", str_last_seat);  

    char line[100];
    char found_flag = 0;
    float price_row = 0;
    while (fgets(line, sizeof(line), artist)) {
        if (strncmp(line, date_formated, strlen(date_formated)) == 0) {
            printf("found\n");
            while(fgets(line, sizeof(line), artist))
            {
                char row_str[10];
                sprintf(row_str, "Row %zu: ", row);
                if (strncmp(line, row_str, strlen(row_str)) == 0) {
                    price_row = atof(line + strlen(row_str));
                    printf("price row %f\n", price_row);
                }

                if (strncmp(line, str_first_seat, strlen(str_first_seat)) == 0) {
                    found_flag = 1;
                    curr_seat = first_seat;
                }

                if(found_flag == 1)
                {
                    char seat_str[10];
                    sprintf(seat_str, "%zu", curr_seat);
                    strcat(seat_str, " - 0");
                    printf("%s\n", seat_str);
                    if (strncmp(line, seat_str, strlen(seat_str)) == 0) {
                        printf("found2\n");
                        fseek(artist, -2, SEEK_CUR);
                        fprintf(artist, "1");
                        fclose(artist);
                        free(file_name);
                        free(date_formated);
                        *seat = curr_seat;
                        *price = price_row;
                        return 1;
                    }
                }

                if(strncmp(line, "////////////////////////////", strlen("////////////////////////////")) == 0 ||
                    strncmp(line, str_last_seat, strlen(str_last_seat)) == 0) // nqma na tozi row
                {
                    break;
                }
                curr_seat++;
            }
            
        }    
    }  

    fclose(artist);
    free(file_name);
    free(date_formated);
    return -1;
}

int print_artists_lineup()
{
    DIR *dir;
    struct dirent *ent;

    dir = opendir("artists");
    if (dir == NULL) {
        printf("Unable to open directory.\n");
        return -1;
    }

    while ((ent = readdir(dir)) != NULL) {
        char file_path[100];
        strcpy(file_path, "artists/");
        strcat(file_path, ent->d_name);

        FILE *file = fopen(file_path, "r");
        if (file == NULL) {
            return -1;
        }
        char line[100];
        if (fgets(line, sizeof(line), file) != NULL) {
            char name[100];
            snprintf(name, strlen(line) - strlen("Name: ") + 1, "%s", line + strlen("Name: "));
            printf("%s", name);
        }
        fclose(file);
    }

    closedir(dir);
    return 1;
}

int main()
{
    //create_artist("Galena", "piemise", "piq");
    // create_concert(10, 70, "Galena", "10.10.1010", "Sofia - Plaza", 0);
    //create_concert(8, 80, "Galena", "12.10.1010", "Sofia - Eleven", 0);

    //make_concert_public("Galena", "10.10.1010");
    //edit_location("Galena", "10.10.1010", "garata");
    //delete_concert("Galena", "10.10.1010");
    //print_artist_info("Galena");
    //print_concert_info("Galena", "12.10.1010");
    //edit_capacity("Galena", "12.10.1010", 10);

    //edit_revenue("Galena", "12.10.1010", 100);

    //print_artists_lineup();

    //buy_ticket("Galena", "12.10.1023", 2);

    size_t row;
    float possible_price;
    size_t seat;

    //offer_ticket("Galena", "12.10.1023", 10, &row, &possible_price, &seat);
    buy_ticket("Galena", "12.10.1023", 2, &possible_price, &seat);
    printf("row: %d price: %f seat: %ld\n", 2, possible_price, seat);
    
    return 0;
}

