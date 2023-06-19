// user input - main()
#include "system.h"

enum bool{ false, true };

enum bool check_specials(char* user_name)
{
    if (user_name == NULL || *user_name == '\0')
        return false;
    else
    {
        for (int i = 0; i < strlen(user_name); i++)
        {
            if (user_name[i] <= 32 || user_name[i] >= 126)
                return false;
        }
    }
    return true;
}

enum bool check_number(char* number)
{
    if(number == NULL || *number == '\0')
		return false;
    else
    {
        int point = 0;
        for (int i = 0; i < strlen(number); i++)
        {
			if (number[i] < '0' || number[i] > '9')
				return false;
            if(number[i] == '.')
				point++;
		}
        if (point > 1)
			return false;
	}
    return true;
}

enum bool check_curr(char* date) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm)); // Initialize struct tm

    sscanf(date, "%d.%d.%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
    tm.tm_mon--;
    tm.tm_year -= 1900;

    time_t converted = mktime(&tm);
    time_t current_date;
    time(&current_date);

    return (converted < current_date) ? false : true;
}

enum bool check_date(char* date) // 01.01.2000 eu date system
{
    if (date == NULL || *date == '\n')
        return false;

    if (date[2] != '.' || date[5] != '.')
        return false;

    for (int count = 0; count < strlen(date); count++)
    {
        if (count == 2 || count == 5)
            continue;

        if (date[count] < '0' || date[count] > '9')
            return false;
    }

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 9999)
        return false;

    int maxDay = 31;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    }
    else if (month == 2) {
        // Check for leap year
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        maxDay = isLeapYear ? 29 : 28;
    }

    if (day > maxDay)
        return false;

    if (!check_curr(date))
        return false;
}

enum bool verify_safe_message(char* safe_message, char* type, char* user_name) // in file
{
    char* file_name = NULL; // file_name_generator(user_name, type);
    FILE* file = fopen(file_name, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Safe Message: ") != NULL) {
            char* message = strchr(line, ':');
            if (message != NULL && strcmp(message + 2, safe_message) == 0) {
                fclose(file);
                return true;  // namerilo se e imeto
            }
        }
    }

    free(file_name);
    fclose(file);

    return false;  // No matching safe message found
}

enum bool verify_password(char* password, char* type, char* user_name) // in file
{
    char* file_name = NULL; // file_name_generator(user_name, type);
    FILE* file = fopen(file_name, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Password: ") != NULL) {
            char* pass = strchr(line, ':');
            if (pass != NULL && strcmp(pass + 2, password) == 0) {
                fclose(file);
                return true;  // namerila se e parolata
            }
        }
    }

    free(file_name);
    fclose(file);

    return false;  // No matching password found
}

enum bool verify_user_name(char* user_name, char* type)
{
    char* file_name = NULL; // file_name_generator(user_name, type);
    FILE* file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Invalid user name! Try again!\n");
        return false;
    }
    else
    {
        fclose(file);
        return true;
    }
}
void input_location(char *location)
{
    do
    {
        printf("Enter the location of the concert: \n");
        fgets(location, sizeof(location), stdin);
        location[strlen(location) - 1] = '\0';
        if (!check_specials(location))
        {
            printf("Invalid location! Try again!\n");
            continue;
        }
        else
            break;

    }while(1);
}
enum bool input_budget(char* budget)
{
    fgets(budget, sizeof(budget), stdin);
    budget[strlen(budget) - 1] = '\0';
    if(!check_number(budget))
    {
        printf("Invalid budget! Try again!\n");
        return false;
    }
    return true;
}
void input_date(char *date)
{
    do
    {
        printf("Enter the date of the concert: \n");
        fgets(date, sizeof(date), stdin);
        getchar();
        if(!check_date(date))
        {
            printf("Invalid date! Try again!\n");
            continue;
        }
        else
            break;
    }while(1);
}



void usual_input(char* user_name, char* password)
{
    printf("  User name:\n");
    fgets(user_name, sizeof(user_name), stdin); // vzimane na user name ot potrebitelq
    user_name[strlen(user_name) - 1] = '\0'; // maham \n ot user name
    printf("  Password:\n");
    fgets(password, sizeof(password), stdin); // vzimane na parolata ot potrebitelq
    password[strlen(password) - 1] = '\0'; // maham \n ot parolata
}

char* forgotten_password(char* user_name, char* safe_message, char* type)
{
    printf("     Forgotten password menu    \n");

    do
    {
        printf("  User name:\n");
        fgets(user_name, sizeof(user_name), stdin); // vzimane na user name ot potrebitelq
        user_name[strlen(user_name) - 1] = '\0'; // maham \n ot user name

        if (!check_specials(user_name) && !verify_user_name(user_name, type))
        {
            printf("Invalid user name! Try again!\n");
            continue;
        }
        else
            break;

    } while (1);

    do
    {
		printf("  Safe message:\n");
		fgets(safe_message, sizeof(safe_message), stdin); // vzimane na safe message ot potrebitelq
		safe_message[strlen(safe_message) - 1] = '\0'; // maham \n ot safe message
        if (!check_specials(safe_message) && !verify_safe_message(safe_message, type, user_name))
        {
			printf("Invalid safe message! Try again!\n");
			continue;
		}
		else
			break;

	} while (1);

    char* new_password = NULL;
    printf("Choose your new password:\n");
    fgets(new_password, 20, stdin);
    new_password[strlen(new_password) - 1] = '\0';

    while (!check_specials(new_password))
    {
        printf("Invalid password! Try again!\n");
        printf("Choose your new password:\n");
        fgets(new_password, 20, stdin);
    }
    char* confirm = NULL;

    do
    {
        printf("Please confirm the new password:\n");
        fgets(confirm, 20, stdin);
        confirm[strlen(confirm) - 1] = '\0';
        if (strcmp(new_password, confirm) != 0)
        {
			printf("Passwords do not match! Try again!\n");
			continue;
		}
		else
			break;

    } while (1);
    // napravi go da potvyrdi parolata

    return new_password;
}

void verify_account(char* user_name, char* password, char* safe_message, char* type)
{
    printf("     Log in    \n\n");
    char answer[3];
    while (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0) // proverka dali sa korektni dannite
    {
        printf("Forgot your password? y/n\n");
        fgets(answer, sizeof(answer), stdin);
        answer[strlen(answer) - 1] = '\0';
        if (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0)
        {
            printf("Invalid input! Try again!\n");
            continue;
        }
    }
    strcmp(answer, "y") == 0 ? password = forgotten_password(user_name, safe_message, type) : 'n';

    usual_input(user_name, password);

    while((!check_specials(user_name) && !verify_user_name(user_name, type)) || (!check_specials(password) && !verify_password(password, type, user_name)))
    {
        if (!check_specials(user_name) && !verify_user_name(user_name, type))
        {
            printf("Invalid username! Try again\n");
        }
        if (!check_specials(password) && !verify_password(password, type, user_name))
        {
            printf("Invalid password! Try again\n");
        }

        printf("Log in    \n");

        while (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0) // proverka dali sa korektni dannite
        {
            printf("Forgot your password? y/n\n");
            fgets(answer, sizeof(answer), stdin);
            answer[strlen(answer) - 1] = '\0';
            if (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0)
            {
                printf("Invalid input! Try again!\n");
                continue;
            }
        }
        strcmp(answer, "y") == 0 ? password = forgotten_password(user_name, safe_message, type) : 'n';

        usual_input(user_name, password);
    }
}

void create_account(char* user_name, char* password, char* safe_message, char* budget, char* type)
{   
    usual_input(user_name, password);

    printf("  Safe message:\n");
    fgets(safe_message, 20, stdin); // vzimane na parolata ot potrebitelq
    safe_message[strlen(safe_message) - 1] = '\0'; // maham \n ot parolata

    while ((!check_specials(user_name) && verify_user_name(user_name, type)) || !check_specials(password) || !check_specials(safe_message)){

        printf("Invalid input! Try again!\n");
        printf("     Sign in    \n");
        usual_input(user_name, password, safe_message);
        printf("  Safe message:\n");
        fgets(safe_message, 20, stdin); // vzimane na parolata ot potrebitelq
        safe_message[strlen(safe_message) - 1] = '\0'; // maham \n ot parolata
    }
    if (strcmp(type, "fans") == 0)
    {
        do
        {
            printf("Enter your budget: \n");
            if (!input_budget(budget))
            {
				printf("Invalid budget! Try again!\n");
				continue;
			}
			else
				break;
        } while (1);
    }
    if (create_user(user_name, password, safe_message, type))
    {
        printf("Account created successfully!\n");
    }
    else
    {
        printf("Account creation failed!\n");
    }
}

void edit_concert(char* user_name, char* date, char* location, char* capacity, char* revenue)
{
    char choice[3];
    do
    {
        printf("What do you want to edit?\n");
        printf("1. Location\n");
        printf("2. Date\n");
        printf("3. Capacity\n");
        printf("4. Revenue\n");
        printf("5. Return\n");

        fgets(choice, sizeof(choice), stdin);
        choice[strlen(choice) - 1] = '\0';

        if(strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0)
        {
            printf("Invalid input! Try again!\n");
            continue;
        }
        else
            break;
    }while(1);
    

    switch (atoi(choice))
    {
        case 1 :
            input_location(location);

            if (edit_location(user_name, date, location))
            {
                printf("Location edited successfully!\n");
            }
            else
            {
                printf("Location editing failed!\n");
            }
            break;

        case 2 :

            char new_date[11];
            input_date(date);

            if (edit_date(user_name, date, date))
            {
                printf("Date edited successfully!\n");
            }
            else
            {
                printf("Date editing failed!\n");
            }
            break;

        case 3 :
            size_t new_capacity = 0;
            do
            {
                printf("Enter the new capacity of the event: \n");
                fgets(capacity, 20, stdin);
                capacity[strlen(capacity) - 1] = '\0';

                if(capacity[0] < '0' || capacity[0] > '9')
                {
                    printf("Invalid capacity! Try again!\n");
                    continue;
                }
                else
                {
                    new_capacity = atoi(capacity);
                    break;
                }
            } while (1);

            if (edit_capacity(user_name, date, new_capacity))
            {
                printf("Capacity edited successfully!\n");
            }
            else
            {
                printf("Capacity editing failed!\n");
            }
            break;

        case 4 :
            float new_revenue = 0;
            do
            {
                printf("Enter the new revenue of the event: \n");
                if(!input_budget(revenue))
                {
                    printf("Invalid revenue! Try again!\n");
                    continue;
                }
                else{
                    new_revenue = atof(revenue);
                    break;
                }
            } while (1);

            if (edit_revenue(user_name, date, new_revenue))
            {
                printf("Revenue edited successfully!\n");
            }
            else
            {
                printf("Revenue editing failed!\n");
            }
            break;
        
        case 5 :
            break;
    }
}

void artists_menu(char* user_name, char* password, char* safe_message)
{
    char choice[3];
    char revenue[20];
    char date[11];
    char location[20];
    char state[20];
    char capacity[20];
    printf("Welcome, %s!\n\n", user_name);
    do
    {
        printf("What do you want to do?\n");
        printf("1. Create an event\n");
        printf("2. View your events\n");
        printf("3. Exit\n");

        fgets(choice, sizeof(choice), stdin);
        choice[strlen(choice) - 1] = '\0';
        if (choice[0] != '1' && choice[0] != '2' && choice[3] != '3' && choice[1] != '\0')
        {
			printf("Invalid choice! Try again!\n");
			continue;
		}
		else
			break;

    } while (1);

    do {
        switch (atoi(choice))
        {
            case 1:

                input_date(date);

                input_location(location);

                size_t new_capacity = 0;

                do
                {
                    printf("Enter the capacity of the event: \n");
                    fgets(capacity, 20, stdin);
                    capacity[strlen(capacity) - 1] = '\0';

                    for (int i = 0; i < strlen(capacity); i++)
                    {
                        if (capacity[i] < '0' || capacity[i] > '9')
                        {
							printf("Invalid capacity! Try again!\n");
							continue;
						}
					}
                    new_capacity = atoi(capacity);
                    break;
                } while (1);
                
                float new_revenue = 0;

                do
                {
                    printf("Enter the revenue of the concert: \n");
                    if(!input_budget(revenue))
                    {
                        printf("Invalid revenue! Try again!\n");
                        continue;
                    }
                    else
                    {
                        new_revenue = atof(revenue);
                        break;
                    }

                } while (1);

                int new_state = 0;

                do
                {
                    printf("Do you want to make your concert public, or are you going to make changes later?\n\n");
                    printf("1 - public / 0 - private\n");
                    fgets(state, sizeof(state), stdin);
                    state[strlen(state) - 1] = '\0';
                    if ((state[0] != '0' || state[0] != '1') && state[1] != '\0')
                    {
					    printf("Invalid input! Try again!\n");
						continue;
					}
                    else
                    {
                        new_state = atoi(state);
                        break;
                    }

                } while (1);

                if (create_concert(new_capacity, new_revenue, user_name, date, location, new_state))
                {
                    printf("Event created successfully!\n");
                }
                else
                {
                    printf("Event creation failed!\n");
                }

                break;

            case 2:

                printf("Here is the list of your concerts:\n\n");
                if(print_all_concerts(user_name) == -1)
                {
                    printf("No concerts available!\n");
                    break;
                }
                char concert_chice[3];
                do
                {
                    printf("You can:\n");
                    printf("1. Edit a concert\n");
                    printf("2. Delete a concert\n");
                    printf("3. Make a concert public\n");
                    printf("4. Return\n");

                    fgets(concert_chice, sizeof(concert_chice), stdin);
                    concert_chice[strlen(concert_chice) - 1] = '\0';

                    if(strcmp(concert_chice, "1") != 0 && strcmp(concert_chice, "2") != 0 && strcmp(concert_chice, "3") != 0 && strcmp(concert_chice, "4") != 0)
                    {
                        printf("Invalid input! Try again!\n");
                        continue;
                    }
                    else
                        break;

                }while(1);
                
                switch (atoi(concert_chice))
                {
                    case 1 :
                        if(is_concert_public(user_name, date))
                        {
                            printf("You can't edit a public concert!\n");
                            break;
                        }

                        edit_concert(user_name, date, location, capacity, revenue);

                        break;
                    case 2 :
                        if(is_concert_public(user_name, date))
                        {
                            printf("You can't delete a public concert!\n");
                            break;
                        }

                        if(delete_concert(user_name, date))
                        {
                            printf("Concert deleted successfully!\n");
                        }
                        else
                        {
                            printf("Concert deletion failed!\n");
                        }

                        break;
                    case 3 :
                        if(is_concert_public(user_name, date))
                        {
                            printf("Concert is already public!\n");
                            break;
                        }

                        if(make_concert_public(user_name, date))
                        {
                            printf("Concert made public successfully!\n");
                        }
                        else
                        {
                            printf("Concert public failed!\n");
                        }
                        break;

                    case 4 :

                        break;

                }
        }
    } while (*choice != '3');
}

enum bool change_budget(char* user_name, char* budget)
{
    char *file_name = file_name_generator(user_name, "fans");
    FILE* fan = fopen(file_name, "r+");
    if (fan == NULL)
    {
        printf("Error opening file!\n");
        free(file_name);
        return false;
    }

    char line[200];
    while (fgets(line, sizeof(line), fan))
    {
        if (strncmp(line, "Budget: ", 8) == 0)
        {
            fseek(fan, -strlen(line), SEEK_CUR);
            fprintf(fan, "Budget: %s\n", budget);
            break;
        }
    }

    fclose(fan);
    free(file_name);
    return true;
}

enum bool print_ticket(char* artist_name, char* date, char* row, char* user_name, char* price, size_t *seat)
{
    char* file_name = file_name_generator(artist_name, "concerts");
    FILE* fan;
    if(file_name == NULL)
    {
        return false;
    }

    fan = fopen(file_name, "a");
    if(fan == NULL)
    {
        printf("Error opening file!\n");
        free(fan);
        return false;
    }

    fseek(fan, 1, SEEK_END);

    fprintf(fan, "Artist: %s\n", artist_name);
    fprintf(fan, "Date: %s\n", date);
    fprintf(fan, "Row: %s\n", row);
    fprintf(fan, "Seat: %d\n", seat);
    fprintf(fan, "Price: %s\n", price);
    fprintf(fan, "////////////////////////////\n");

    free(file_name);
    fclose(fan);
    return true;
}

void fans_menu(char* user_name, char* password, char* safe_message, char* strbudget)
{
    printf("Welcome, %s!\n\n", user_name);

    char choice[3];
    char revenue[20];

    do
    {
        printf("What do you want to do?\n");
        printf("1. Buy a ticket\n");
        printf("2. View your account\n");

        fgets(choice, sizeof(choice), stdin);
        choice[strlen(choice) - 1] = '\0';
        if (choice[0] != '1' && choice[0] != '2' && choice[1] != '\0')
        {
            printf("Invalid choice! Try again!\n");
            continue;
        }
        else
            break;

    } while (1);

    do {

        switch (atoi(choice))
        {
            case 1:
                printf("Here is the list of available artists:\n\n");
                if(print_artists_lineup() == -1)
                {
                    printf("No artists available!\n");
                    break;
                }

                char artist_name[20];

                do
                {
                    printf("Enter the name of the artist: \n");
                    fgets(artist_name, 20, stdin);
                    artist_name[strlen(artist_name) - 1] = '\0';
                    if (!check_specials(artist_name) || !verify_user_name(artist_name, "artist"))
                    {
						printf("Invalid artist name! Try again!\n");
						continue;
					}
					else
						break;
                } while (1);

                char answer[3];

                do{
                    printf("Do you want to see all the available concerts for the artist?\n");
                    printf("Or do you want to see the best ticket for your budget? 1/2 \n");
                    fgets(answer, sizeof(answer), stdin);
                    answer[strlen(answer) - 1] = '\0';

                    if(strcmp(answer, "1") != 0 && strcmp(answer, "2") != 0)
                    {
                        printf("Invalid input! Try again!\n");
                        continue;
                    }
                    else
                        break;

                }while(1);

				char date[11];
                char row[20];

                if(strcmp(answer, "1") == 0)
                {
                    printf("Here is the list of available concerts for %s:\n\n", artist_name);
                    if(print_concerts(artist_name) == -1)
                    {
                        printf("No concerts available!\n");
                        break;
                    }
                    else{
                        input_date(date);
                    }

                    printf("Here is the list of available rows for %s:\n\n", artist_name);
                    if(print_rows(artist_name, date) == -1)
                    {
                        printf("No rows available!\n");
                        break;
                    }
                    else{
                        do
                        {
                            printf("Enter the row that you want a ticket for: \n");
                            fgets(row, 20, stdin);
                            row[strlen(row) - 1] = '\0';
                            for(int i = 0; i < strlen(row); i++)
                            {
                                if(row[i] < '0' || row[i] > '9')
                                {
                                    printf("Invalid row! Try again!\n");
                                    continue;
                                }
                                else
                                    break;
                            }
                        }while(1);
                    }

                }
                else if(strcmp(answer, "2") == 0)
                {
                    do
                    {
                        printf("How much money are you willing to spend?\n");
                        
                        if(!input_budget(revenue))
                        {
                            printf("Invalid some! Try again!\n");
                            continue;
                        }
                        if (atof(strbudget) < atof(revenue))
                        {
                            printf("You don't have money! Try again!\n");
                            continue;
                        }
                        float new_revenue = 0;
                        new_revenue = atof(revenue);
                        break;
                    } while (1);

                    // tuk vikam na iva funkciqta koqto shte vurne naj dobroto mqsto za cenata na potrebitelq
                }
                size_t seat = 0;

                if(!print_ticket(artist_name, date, row, user_name, revenue, seat))
                {
                    printf("Ticket bought successfully!\n");
                }

				break;

            case 2:
                printf("Here is your account information:\n");
                printf("Username: %s\n", user_name);
                printf("Password: %s\n", password);
                printf("Budget: %s\n", strbudget);

                printf("Do you want to add to your budget? y/n\n");
                char answer[3];
                fgets(answer, sizeof(answer), stdin);
                answer[strlen(answer) - 1] = '\0';

                char temp_budget[20];

                if (strcmp(answer, "y") == 0)
                {
                    do
                    {
                        printf("How much money do you want to add?\n");
                        if(!input_budget(temp_budget))
                        {
                            printf("Invalid budget! Try again!\n");
                            continue;
                        }

						float new_budget = 0, budget = atof(strbudget);
						new_budget = atof(temp_budget);
                            
                        budget += new_budget;
                        sprintf(strbudget, "%f", budget);

						break;
					} while(1);

                    change_budget(user_name, strbudget);

				}
                else if (strcmp(answer, "n") != 0)
                {
					printf("Invalid input! Try again!\n");
					continue;
				}
                    
                break;
        }

    } while (1);
}

void login_signin(char* user_name, char* password, char* safe_message, char* budget,  char* type) //yzdavane na funkciq za log in i sign in
{
    char answer[3];
    while (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0)
    {
        printf("Do you have an account? y/n\n");
        fgets(answer, sizeof(answer), stdin);
        answer[strlen(answer) - 1] = '\0';// da proverqva dali answer e pravilen znak
        if (strcmp(answer, "y") != 0 && strcmp(answer, "n") != 0)
        {
            printf("Invalid input! Try again!\n");
            continue;
        }
    }

    strcmp(answer, "y") == 0 ? verify_account(user_name, password, safe_message, type) : create_account(user_name, password, safe_message, budget, type);
}

void menu()
{
    char user_name[20];
    char password[20];
    char safe_message[20];
    char budget[20];

    printf("Welcome to TixMaestro!\n");

    char* type = (char*)malloc(10 * sizeof(char));
    printf("Are you an artist or a fan? a/f\n");
    fgets(type, sizeof(type), stdin);
    type[strlen(type) - 1] = '\0';

    while (strcmp(type, "a") != 0 && strcmp(type, "f") != 0)
    {
        printf("Invalid input! Try again!\n");
        printf("Are you an artist or a fan? a/f\n");
        fgets(type, sizeof(type), stdin);
        type[strlen(type) - 1] = '\0';

    }
   
    if (strcmp(type, "a") == 0) {
        strcpy(type, "artists");
    }
    else {
        strcpy(type, "fans");
    }

    login_signin(user_name, password, safe_message, budget, type);

    if (strcmp(type, "artists") == 0)
    {
        artists_menu(user_name, password, safe_message);
    }
    else
    {
        fans_menu(user_name, password, safe_message, budget);
    }
}

int main()
{
    menu();
}