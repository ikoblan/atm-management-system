#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    int read = fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                      &r->id, &r->userId, name, &r->accountNbr,
                      &r->deposit.month, &r->deposit.day, &r->deposit.year,
                      r->country, &r->phone, &r->amount, r->accountType);
    return (read == 11);
}

void createNewAcc(struct User u)
{
    struct Record r, cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    int lastId = -1;

    if (pf == NULL) {
        printf("Error: Could not open records file!\n");
        return;
    }

    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("\nEnter today's date(mm/dd/yyyy): ");
    if (scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year) != 3) {
        printf("Invalid date format!\n");
        fclose(pf);
        return;
    }

    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNbr);

    rewind(pf);
    while (getAccountFromFile(pf, userName, &cr)) {
        if (cr.accountNbr == r.accountNbr) {
            printf("✖ This Account already exists\n");
            fclose(pf);
            stayOrReturn(0, createNewAcc, u);
            return;
        }
        lastId = cr.id;
    }

    r.id = lastId + 1;
    r.userId = u.id;

    printf("\nEnter the country: "); scanf("%s", r.country);
    printf("\nEnter the phone number: "); scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $"); scanf("%lf", &r.amount);
    printf("\nChoose type (savings/current/fixed01/fixed02/fixed03): "); scanf("%s", r.accountType);

    fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id, u.id, u.name, r.accountNbr,
            r.deposit.month, r.deposit.day, r.deposit.year,
            r.country, r.phone, r.amount, r.accountType);

    fflush(pf);
    fclose(pf);
    success(u);
}

void updateAccount(struct User u)
{
    int accId, choice, found = 0;
    struct Record r;
    char userName[100];
    FILE *oldF = fopen(RECORDS, "r");
    FILE *newF = fopen("./data/temp.txt", "w");

    if (oldF == NULL || newF == NULL) {
        printf("Error opening files!\n");
        return;
    }

    printf("\nEnter account ID to update: "); 
    scanf("%d", &accId);

    while (getAccountFromFile(oldF, userName, &r)) {
        if (r.accountNbr == accId && strcmp(userName, u.name) == 0) {
            found = 1;
            printf("\n1. Phone\n2. Country\nChoice: "); 
            scanf("%d", &choice);
            while (getchar() != '\n'); 

            if (choice == 1) {
                printf("Enter new phone: ");
                scanf("%d", &r.phone);
            } else {
                printf("Enter new country: ");
                scanf("%s", r.country);
            }
        }
        fprintf(newF, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", 
                r.id, r.userId, userName, r.accountNbr, 
                r.deposit.month, r.deposit.day, r.deposit.year, 
                r.country, r.phone, r.amount, r.accountType);
    }
    fclose(oldF); fclose(newF);
    remove(RECORDS); rename("./data/temp.txt", RECORDS);

    if (found) success(u); 
    else {
        printf("\n✖ Account not found or access denied!");
        stayOrReturn(0, updateAccount, u);
    }
}

void checkAccountDetails(struct User u) {
    struct Record r; char name[50]; int id, found = 0;
    FILE *pf = fopen(RECORDS, "r");
    printf("\nEnter Account ID: "); scanf("%d", &id);
    while (getAccountFromFile(pf, name, &r)) {
        if (r.accountNbr == id && strcmp(name, u.name) == 0) {
            found = 1;
            printf("\nType: %s, Balance: $%.2f\n", r.accountType, r.amount);
            if (strcmp(r.accountType, "current") == 0) printf("No interest for current account.\n");
            else {
                double rate = (strcmp(r.accountType, "savings") == 0) ? 0.07 : (strcmp(r.accountType, "fixed01") == 0) ? 0.04 : (strcmp(r.accountType, "fixed02") == 0) ? 0.05 : 0.08;
                printf("You will get $%.2f as interest on day %d of every month\n", (r.amount * rate) / 12, r.deposit.day);
            }
        }
    }
    fclose(pf); if (!found) stayOrReturn(0, checkAccountDetails, u); else success(u);
}

void checkAllAccounts(struct User u) {
    char name[50]; struct Record r; FILE *pf = fopen(RECORDS, "r");
    system("clear"); printf("--- Accounts for %s ---\n", u.name);
    while (getAccountFromFile(pf, name, &r)) {
        if (strcmp(name, u.name) == 0) printf("Acc: %d | Balance: $%.2f | Type: %s\n", r.accountNbr, r.amount, r.accountType);
    }
    fclose(pf); success(u);
}

void makeTransaction(struct User u) {
    struct Record r; char name[50]; int id, found = 0, opt; double val;
    FILE *oldF = fopen(RECORDS, "r"), *newF = fopen("./data/temp.txt", "w");
    printf("\nEnter Account ID: "); scanf("%d", &id);
    while (getAccountFromFile(oldF, name, &r)) {
        if (r.accountNbr == id && strcmp(name, u.name) == 0) {
            found = 1;
            if (strncmp(r.accountType, "fixed", 5) == 0) printf("✖ Error: Fixed accounts restricted!\n");
            else {
                printf("1. Withdraw\n2. Deposit\nChoice: "); scanf("%d", &opt);
                printf("Amount: "); scanf("%lf", &val);
                if (opt == 1) r.amount -= val; else r.amount += val;
            }
        }
        fprintf(newF, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", r.id, r.userId, name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
    }
    fclose(oldF); fclose(newF); remove(RECORDS); rename("./data/temp.txt", RECORDS);
    if (found) success(u); else stayOrReturn(0, makeTransaction, u);
}

void removeAccount(struct User u) {
    struct Record r; char name[50]; int id, found = 0;
    FILE *oldF = fopen(RECORDS, "r"), *newF = fopen("./data/temp.txt", "w");
    printf("\nEnter Account ID to REMOVE: "); scanf("%d", &id);
    while (getAccountFromFile(oldF, name, &r)) {
        if (r.accountNbr == id && strcmp(name, u.name) == 0) { found = 1; continue; }
        fprintf(newF, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", r.id, r.userId, name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
    }
    fclose(oldF); fclose(newF); remove(RECORDS); rename("./data/temp.txt", RECORDS);
    if (found) success(u); else stayOrReturn(0, removeAccount, u);
}

void transferOwner(struct User u) {
    struct Record r; char name[50], targetU[50]; int id, targetId, found = 0;
    FILE *oldF = fopen(RECORDS, "r"), *newF = fopen("./data/temp.txt", "w");
    printf("\nAccount ID: "); scanf("%d", &id);
    printf("New Owner Username: "); scanf("%s", targetU);
    printf("New Owner ID: "); scanf("%d", &targetId);
    while (getAccountFromFile(oldF, name, &r)) {
        if (r.accountNbr == id && strcmp(name, u.name) == 0) {
            found = 1;
            fprintf(newF, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", r.id, targetId, targetU, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
        } else fprintf(newF, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n", r.id, r.userId, name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
    }
    fclose(oldF); fclose(newF); remove(RECORDS); rename("./data/temp.txt", RECORDS);
    if (found) success(u); else stayOrReturn(0, transferOwner, u);
}

void success(struct User u) { 
    printf("\n✔ Success!\nEnter 1 for menu, 0 to exit: "); 
    int o; scanf("%d", &o); 
    if (o==1) mainMenu(u); else exit(0); 
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
    printf("\nPress Enter to try again...");
    getchar(); getchar();  
    f(u);
}