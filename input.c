// user input - main()
#include "system.h"

void verify_account();// proverka dali potrebitelq e v sistemata

enum bool{ false, true };

enum bool check_username(char* user_name)
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
}

enum bool check_password(char* password)
{
	if (password == NULL || *password == '\0')
		return false;
	else
	{
		for (int i = 0; i < strlen(password); i++)
		{
			if ((password[i] <= 32 || password[i] >= 126) && password[i] != '\n')
				return false;
		}
	}
}

void check_safe_massage(char* safe_message)
{
	if (safe_message != "")
	{
		int i;
		for (i = 0; i < strlen(safe_message); i++)
		{
			if (safe_message[i] == ' ')
			{
				printf("Invalid message! Try again!\n");
				printf("  Message:\n");
				fgets(safe_message, 20, stdin);
			}
		}
	}
}

enum bool verify_safe_message(char* safe_message);
enum bool verify_user_name(char* user_name);

char *forgotten_password(char* user_name, char* safe_message)
{
    printf("     Forgotten password    \n");

    rintf("  User name:\n"); // vzimane na user name ot potrebitelq
    fgets(user_name, 20, stdin);
    verify_user_name(user_name);
    while(!verify_user_name(user_name))
    {
        printf("Invalid user name! Try again!\n");
        printf("  User name:\n");
        fgets(user_name, 20, stdin);
        verify_user_name(user_name);
    }

    printf("  Safe message:\n");
    fgets(safe_message, 20, stdin); // vzimane na safe message ot potrebitelq
    while(!verify_safe_message(safe_message))
    {
        printf("Invalid safe message! Try again!\n");
        printf("  Safe message:\n");
        fgets(safe_message, 20, stdin);
        verify_safe_message(safe_message);
    }

    char *new_password;
    printf("Choose your new password:\n");
    fgets(new_password, 20, stdin);

    while(!check_password(new_password))
    {
        printf("Invalid password! Try again!\n");
        printf("Choose your new password:\n");
        fgets(new_password, 20, stdin);
    }
    // napravi go da potvyrdi parolata
    return new_password;
}

void verify_account(char *user_name,char *password,char *safe_message)
{
    printf("     Log in    \n");
    printf("Forgotten password? y/n\n");
    char answer;
    scanf("%c", &answer); // da proverqva dali answer e pravilen znak 
    answer == 'y' ? password = forgotten_password(user_name, safe_message) : 'n';
    printf("  User name:\n");
    fgets(user_name, 20, stdin);// vzimane na user name ot potrebitelq
    printf("  Password:\n");
    fgets(password, 20, stdin); // vzimane na parolata ot potrebitelq
    int flag = 0;
    do
    {
        if (!check_username(user_name) && !verify_user_name(user_name))
        {
            printf("Invalid username! Try again\n");
            flag = 1;
        }
        if (!check_password(password) && !verify_password(password))
        {
            printf("Invalid password! Try again\n");
            flag = 1;
        }
        if (flag)
        {
            printf("     Log in    \n");
            printf("  User name:\n");
            fgets(user_name, 20, stdin);
            printf("  Password:\n");
            fgets(password, 20, stdin);
        }
    } while (flag);
}

void create_account(char *user_name,char *password,char *safe_message)
{
    printf("Are you an artist or a fan? a/f\n");
    char type;
    scanf("%c", &type);
    type == 'a' ? 'a' : 'f';
    printf("     Sign in    \n");
    printf("  User name:\n");
    fgets(user_name, 20, stdin);// vzimane na user name ot potrebitelq
    printf("  Password:\n");
    fgets(password, 20, stdin); // vzimane na parolata ot potrebitelq
    printf("  Safe message:\n");
    fgets(safe_message, 20, stdin); // vzimane na parolata ot potrebitelq
}

void login_signin(char* user_name, char* password, char* safe_message) //yzdavane na funkciq za log in i sign in
{
    printf("Do you have an account? y/n\n");
    char answer;
    scanf("%c", &answer);
    answer == 'y' ? verify_account(user_name, password, safe_message) : create_account(user_name, password, safe_message);
}

void menu()
{
	char user_name[20];
	char password[20];
	char safe_message[20];

	printf("Welcome to TixMaestro!\n");
	login_signin(&user_name,&password,&safe_message);
}

int main()
{
	menu();
	return 0;
}