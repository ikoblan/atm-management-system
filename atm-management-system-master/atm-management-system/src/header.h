#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

struct Date {
    int month, day, year;
};

struct Record {
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[20];
    int accountNbr;
    double amount;
    struct Date deposit;
};

struct User {
    int id;
    char name[50];
    char password[50];
};

void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void updateAccount(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwner(struct User u);

void success(struct User u);
void stayOrReturn(int notGood, void f(struct User u), struct User u);
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);

#endif