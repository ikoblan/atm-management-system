#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50]) {
    struct termios oflags, nflags;
    system("clear");
    printf("\n\n\t\t\tUser Login: ");
    scanf("%s", a);

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    tcsetattr(fileno(stdin), TCSANOW, &nflags);

    printf("\t\t\tEnter password: ");
    scanf("%s", pass);

    tcsetattr(fileno(stdin), TCSANOW, &oflags);
}

void registerMenu(char a[50], char pass[50]) {
    struct User u;
    FILE *fp = fopen(USERS, "a+");
    int lastId = -1;

    if (fp == NULL) {
        printf("Error: Could not open users file. Check if 'data' folder exists.\n");
        exit(1);
    }

    system("clear");
    printf("\n\t\tRegistration\n\t\tNew username: ");
    scanf("%s", a);

    rewind(fp);
    while (fscanf(fp, "%d %s %s", &u.id, u.name, u.password) == 3) {
        if (strcmp(u.name, a) == 0) {
            printf("\n✖ User already exists!\n");
            fclose(fp);
            printf("\nPress Enter to exit...");
            getchar(); getchar(); 
            exit(1);
        }
        lastId = u.id;
    }

    printf("\t\tNew password: ");
    scanf("%s", pass);

    fprintf(fp, "%d %s %s\n", lastId + 1, a, pass);
    
    fflush(fp);
    fclose(fp);

    printf("\n✔ Success! User %s registered.\n", a);
    printf("Please restart the app or login now.\n");
    printf("\nPress Enter to continue...");
    getchar(); getchar();
}

const char *getPassword(struct User u) {
    FILE *fp = fopen(USERS, "r");
    static char foundPass[50];
    int id;
    char name[50];

    strcpy(foundPass, "no user found");

    if (!fp) return "no user found";

    while (fscanf(fp, "%d %s %s", &id, name, foundPass) == 3) {
        if (strcmp(name, u.name) == 0) {
            fclose(fp);
            return foundPass;
        }
    }

    fclose(fp);
    return "no user found"; 
}