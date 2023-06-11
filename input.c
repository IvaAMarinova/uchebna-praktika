// user input - main()
#include "system.h"

void verify_account();// proverka dali potrebitelq e v sistemata

enum bool{ false, true };

enum bool check_specials(char* user_name)
{
	if (user_name == NULL || *user_name == '\0')
		return false;
	else
	{
		for (int i = 0; i < strlen(user_name); i++)
		{
			if ((user_name[i] <= 32 || user_name[i] >= 126) && user_name[i] != '\n')
				return false;
		}
	}
    return true;
}

enum bool check_curr(char *date)
{
    struct tm tm;
    sscanf(date, "%d %d %d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
    tm.tm_mon--;
    tm.tm_year -= 1900;
    time_t converted = mktime(&tm);

    time_t current_date;
    time(&current_date);

    return converted < current_date ? false : true;
}

enum bool check_date(char* date) // 01.01.2000 eu date system
{
    if(date == NULL || *date == '\n')
    {
        printf("Invalid date! Try again!\n");
        return false;
    }

    if (date[2] != '.' || date[5] != '.')
    {
        printf("Invalid date format! Try again!\n");
        return false;
    }

    for(int count = 0; count < strlen(date); count++)
    {
        if(count == 2 || count == 5)
            continue;

        if (date[count] < '0' || date[count] > '9')
        {
            printf("Invalid date! Try again!\n");
            return false;
        }
    }

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1111 || year > 9999)
    {
        printf("Invalid date! Try again!\n");
        return false;
    }
    int maxDay = 31;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    } else if (month == 2) {
        // Check for leap year
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        maxDay = isLeapYear ? 29 : 28;
    }

    if (day > maxDay) {
        printf("Invalid date! Try again!\n");
        return false;
    }

    if (!check_curr(date))
    {
        printf("Invalid date! Try again!\n");
        return false;
    }
}

enum bool verify_safe_message(char* safe_message, char* type, char *user_name) // in file
{
    char *file_name = file_name_generator(user_name, type);
    FILE *file = fopen(file_name, "r");

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

enum bool verify_password(char* password, char* type, char *user_name) // in file
{
    char *file_name = file_name_generator(user_name, type);
    FILE *file = fopen(file_name, "r");

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
    char *file_name = file_name_generator(user_name, type);
    FILE *file = fopen(file_name, "r");
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

char *forgotten_password(char* user_name, char* safe_message, char *type)
{
    printf("     Forgotten password    \n");

    rintf("  User name:\n"); // vzimane na user name ot potrebitelq
    fgets(user_name, 20, stdin);
    while(!verify_user_name(user_name, type))
    {
        printf("Invalid user name! Try again!\n");
        printf("  User name:\n");
        fgets(user_name, 20, stdin);
        verify_user_name(user_name, type);
    }

    printf("  Safe message:\n");
    fgets(safe_message, 20, stdin); // vzimane na safe message ot potrebitelq
    while(!verify_safe_message(safe_message, type, user_name))
    {
        printf("Invalid safe message! Try again!\n");
        printf("  Safe message:\n");
        fgets(safe_message, 20, stdin);
        verify_safe_message(safe_message, type, user_name);
    }

    char *new_password;
    printf("Choose your new password:\n");
    fgets(new_password, 20, stdin);

    while(!check_specials(new_password))
    {
        printf("Invalid password! Try again!\n");
        printf("Choose your new password:\n");
        fgets(new_password, 20, stdin);
    }
    // napravi go da potvyrdi parolata
    return new_password;
}

void verify_account(char *user_name,char *password,char *safe_message, char *type)
{
    printf("     Log in    \n");
    printf("Forgotten password? y/n\n");
    char answer;
    scanf("%c", &answer); // da proverqva dali answer e pravilen znak 
    answer == 'y' ? password = forgotten_password(user_name, safe_message, type) : 'n';
    printf("  User name:\n");
    fgets(user_name, 20, stdin);// vzimane na user name ot potrebitelq
    printf("  Password:\n");
    fgets(password, 20, stdin); // vzimane na parolata ot potrebitelq
    int flag = 0;
    do
    {
        if (!check_specials(user_name) && !verify_user_name(user_name, type))
        {
            printf("Invalid username! Try again\n");
            flag = 1;
        }
        if (!check_specials(password) && !verify_password(password, type, user_name))
        {
            printf("Invalid password! Try again\n");
            flag = 1;
        }
        if (flag)
        {
            printf("     Log in    \n");
            printf("Forgotten password? y/n\n");
            scanf("%c", &answer); // da proverqva dali answer e pravilen znak 
            answer == 'y' ? password = forgotten_password(user_name, safe_message, type) : 'n';
            printf("  User name:\n");
            fgets(user_name, 20, stdin);
            printf("  Password:\n");
            fgets(password, 20, stdin);
        }
    } while (flag);
}

void create_account(char *user_name,char *password,char *safe_message)
{
    printf("     Sign in    \n");
    printf("  User name:\n");
    fgets(user_name, 20, stdin);// vzimane na user name ot potrebitelq
    printf("  Password:\n");
    fgets(password, 20, stdin); // vzimane na parolata ot potrebitelq
    printf("  Safe message:\n");
    fgets(safe_message, 20, stdin); // vzimane na parolata ot potrebitelq

}

void login_signin(char* user_name, char* password, char* safe_message, char *type) //yzdavane na funkciq za log in i sign in
{
    printf("Do you have an account? y/n\n");
    char answer;
    scanf("%c", &answer); // da proverqva dali answer e pravilen znak

    answer == 'y' ? verify_account(user_name, password, safe_message, type) : create_account(user_name, password, safe_message);
}

void artists_menu(char *user_name, char* password, char* safe_message)
{
    int choice;
    float revenue;
    char *date;
    char *location;
    int state;
    printf("Welcome, %s!\n", user_name);
    printf("What do you want to do?\n");
    printf("1. Create an event\n");
    printf("2. View your events\n");

    scanf("%d", &choice);
    int flag = 1;
    do{
        switch(choice)
        {
            case 1:
                
                break;
            case 2:
                view_events(user_name);
                break;
            default:
                printf("Invalid choice! Try again!\n");
                scanf("%d", &choice);
                break;
        }
    }while(flag);
}

void menu()
{
	char user_name[20];
	char password[20];
	char safe_message[20];

	printf("Welcome to TixMaestro!\n");

    printf("Are you an artist or a fan? a/f\n");
    char *type = (char*)malloc(10*sizeof(char));
    fgets(type, 10, stdin);
    type == "artists" ? "artists" : "fans";

    login_signin(&user_name,&password, &safe_message, type);

    if(strcmp(type, "artists") == 0)
    {
        artist_menu(&user_name,&password, &safe_message);
    }
    else
    {
        //fan_menu();
    }
}

int main()
{
	menu();
	return 0;
}