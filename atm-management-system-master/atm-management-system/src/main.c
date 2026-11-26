#include "header.h"

void mainMenu(struct User u) {
    int option;
    system("clear");
    printf("\n\t[1]- Create account\n\t[2]- Update information\n\t[3]- Check details\n\t[4]- Check all accounts\n\t[5]- Transaction\n\t[6]- Remove account\n\t[7]- Transfer owner\n\t[8]- Exit\n\tSelection: ");
    scanf("%d", &option);
    switch (option) {
        case 1: createNewAcc(u); break;
        case 2: updateAccount(u); break;
        case 3: checkAccountDetails(u); break;
        case 4: checkAllAccounts(u); break;
        case 5: makeTransaction(u); break;
        case 6: removeAccount(u); break;
        case 7: transferOwner(u); break;
        case 8: exit(0);
        default: printf("Invalid!"); mainMenu(u);
    }
}

void initMenu(struct User *u) {
    int option, r = 0;
    system("clear");
    printf("\t[1]- Login\n\t[2]- Register\n\t[3]- Exit\n\tSelection: ");
    while (!r) {
        scanf("%d", &option);
        if (option == 1) {
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0) r = 1;
            else { printf("Error!"); exit(1); }
        } else if (option == 2) {
            registerMenu(u->name, u->password);
            r = 1;
        } else exit(0);
    }
}

int main() {
    struct User u;
    initMenu(&u);
    mainMenu(u);
    return 0;
}