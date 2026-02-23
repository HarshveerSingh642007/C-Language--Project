/*
    Zoo Management System
    Project for Class 11 Computer Science
    Author: <Your Name>
    Semester: Fall 2025

    This project manages a simple zoo database with animals, staff, and activities.
    All data is kept in simple arrays. Menu-driven for ease of use.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 50

// For fast string match, case insensitive
int myStringEquals(const char *a, const char *b) {
    while(*a && *b) {
        char ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if(ca != cb) return 0;
        a++; b++;
    }
    return *a == *b;
}

// Animal info
typedef struct {
    int id;
    char name[MAXLEN];
    char type[MAXLEN];
    int years; // Age
    char area[MAXLEN]; // Enclosure
} Animal;

// Zoo employee
typedef struct {
    int id;
    char name[MAXLEN];
    int years; // Age
    char gender;
    char work[MAXLEN]; // Keeper, Doctor, etc.
} Helper;

// Activity log type
typedef struct {
    int id;
    int whoAnimal;
    int whoHelper;
    char date[MAXLEN];
    char what[MAXLEN];
} Log;

Animal animals[101];
Helper helpers[61];
Log logs[201];
int nAnimals = 0, nHelpers = 0, nLogs = 0;

void menuMain(), menuAnimal(), menuHelper(), menuLog();
void addAnimal(), showAnimals(), searchAnimalName(), getAnimalById(), getAnimalEnclosure();
void addHelper(), showHelpers(), searchHelperName(), getHelpersByWork();
void addLog(), showLogs(), getLogByHelper(), getLogByAnimal();
int askInt(const char* ask, int lo, int hi);

int main() {
    char pw[21];
    printf("Zoo admin password: ");
    scanf("%20s", pw);
    if(strcmp(pw,"zoo123") != 0) {
        printf("Sorry, wrong password.\n");
        return 0;
    }
    menuMain();
    return 0;
}

// --- Main Menu, with each option in new line ---
void menuMain() {
    int c;
    do {
        printf("\n==== ZOO MANAGEMENT MENU ====\n");
        printf("1. Animal menu\n");
        printf("2. Staff menu\n");
        printf("3. Activity log menu\n");
        printf("4. List all animals\n");
        printf("5. List all staff\n");
        printf("6. List all logs\n");
        printf("7. Quit\n");
        printf("Choose: ");
        c = askInt("",1,7);
        if(c==1) menuAnimal();
        else if(c==2) menuHelper();
        else if(c==3) menuLog();
        else if(c==4) showAnimals();
        else if(c==5) showHelpers();
        else if(c==6) showLogs();
    } while(c!=7);
}

// --- Animal Section ---
void menuAnimal() {
    int ch;
    do {
        printf("\n-- Animal section --\n");
        printf("1. Add\n");
        printf("2. Show all\n");
        printf("3. Search by name\n");
        printf("4. Get by ID\n");
        printf("5. Get by area\n");
        printf("6. Back\n");
        printf("Pick: ");
        ch = askInt("",1,6);
        if(ch==1) addAnimal();
        else if(ch==2) showAnimals();
        else if(ch==3) searchAnimalName();
        else if(ch==4) getAnimalById();
        else if(ch==5) getAnimalEnclosure();
    } while(ch!=6);
}

// --- Staff Section ---
void menuHelper() {
    int ch;
    do {
        printf("\n-- Staff section --\n");
        printf("1. Add\n");
        printf("2. Show all\n");
        printf("3. Search by name\n");
        printf("4. Get by job\n");
        printf("5. Back\n");
        printf("Option: ");
        ch = askInt("",1,5);
        if(ch==1) addHelper();
        else if(ch==2) showHelpers();
        else if(ch==3) searchHelperName();
        else if(ch==4) getHelpersByWork();
    } while(ch!=5);
}

// --- Log/Activity Section ---
void menuLog() {
    int ch;
    do {
        printf("\n-- Activity (log) section --\n");
        printf("1. Add activity\n");
        printf("2. View all activities\n");
        printf("3. Find for staff\n");
        printf("4. Find for animal\n");
        printf("5. Back\n");
        printf("Your choice: ");
        ch = askInt("",1,5);
        if(ch==1) addLog();
        else if(ch==2) showLogs();
        else if(ch==3) getLogByHelper();
        else if(ch==4) getLogByAnimal();
    } while(ch!=5);
}

// ---- Features ----
void addAnimal() {
    Animal tmp;
    tmp.id = nAnimals+1;
    printf("Name: "); scanf(" %[^\n]", tmp.name);
    printf("Species/type: "); scanf(" %[^\n]", tmp.type);
    tmp.years = askInt("Animal age: ",0,99);
    printf("Area/enclosure: "); scanf(" %[^\n]", tmp.area);
    animals[nAnimals++] = tmp;
    printf("Animal recorded!\n");
}

void showAnimals() {
    printf("\nAnimals list:\n");
    for(int i=0;i<nAnimals;++i)
        printf("ID:%d Name:%s Species:%s Age:%d Area:%s\n",
               animals[i].id, animals[i].name, animals[i].type, animals[i].years, animals[i].area);
}

void searchAnimalName() {
    char nm[MAXLEN]; int ff=0;
    printf("Name to search: "); scanf(" %[^\n]", nm);
    for(int i=0;i<nAnimals;++i)
        if(myStringEquals(animals[i].name, nm)) {
            printf("ID:%d %s %s %d %s\n", animals[i].id, animals[i].name, animals[i].type, animals[i].years, animals[i].area);
            ff=1;
        }
    if(!ff) puts("Name not in record.");
}

void getAnimalById() {
    int id=askInt("ID: ",1,nAnimals), ff=0;
    for(int i=0;i<nAnimals;++i)
        if(animals[i].id==id) {
            printf("ID:%d %s %s %d %s\n", animals[i].id, animals[i].name, animals[i].type, animals[i].years, animals[i].area);
            ff=1;
        }
    if(!ff) puts("ID not found.");
}

void getAnimalEnclosure() {
    char area[MAXLEN]; int ff=0;
    printf("Area/enclosure: "); scanf(" %[^\n]", area);
    for(int i=0;i<nAnimals;++i)
        if(myStringEquals(animals[i].area, area)) {
            printf("ID:%d %s %s %d\n", animals[i].id, animals[i].name, animals[i].type, animals[i].years);
            ff=1;
        }
    if(!ff) puts("Area not found.");
}

void addHelper() {
    Helper tmp;
    tmp.id = nHelpers+1;
    printf("Name: "); scanf(" %[^\n]", tmp.name);
    tmp.years = askInt("Age: ",16,99);
    do{printf("Gender (M/F): "); scanf(" %c",&tmp.gender);tmp.gender=toupper(tmp.gender);}
    while(tmp.gender!='M'&&tmp.gender!='F');
    printf("Role/job: "); scanf(" %[^\n]", tmp.work);
    helpers[nHelpers++] = tmp;
    printf("Staff recorded!\n");
}

void showHelpers() {
    printf("\nStaff list:\n");
    for(int i=0;i<nHelpers;++i)
        printf("ID:%d Name:%s Age:%d Gender:%c Job:%s\n",
               helpers[i].id, helpers[i].name, helpers[i].years, helpers[i].gender, helpers[i].work);
}

void searchHelperName() {
    char nm[MAXLEN]; int ff=0;
    printf("Name to search: "); scanf(" %[^\n]", nm);
    for(int i=0;i<nHelpers;++i)
        if(myStringEquals(helpers[i].name, nm)) {
            printf("ID:%d %s %d %c %s\n", helpers[i].id, helpers[i].name, helpers[i].years, helpers[i].gender, helpers[i].work);
            ff=1;
        }
    if(!ff) puts("Name not present.");
}

void getHelpersByWork() {
    char rk[MAXLEN]; int ff=0;
    printf("Role/job: "); scanf(" %[^\n]", rk);
    for(int i=0;i<nHelpers;++i)
        if(myStringEquals(helpers[i].work, rk)) {
            printf("ID:%d %s %d %c\n", helpers[i].id, helpers[i].name, helpers[i].years, helpers[i].gender);
            ff=1;
        }
    if(!ff) puts("No staff in that role.");
}

void addLog() {
    Log t;
    t.id = nLogs+1;
    t.whoAnimal = askInt("Animal ID: ",1,nAnimals);
    t.whoHelper = askInt("Staff ID: ",1,nHelpers);
    printf("Date (d/m/yyyy): "); scanf(" %[^\n]", t.date);
    printf("Work (what): "); scanf(" %[^\n]", t.what);
    logs[nLogs++] = t;
    printf("Activity entered!\n");
}

void showLogs() {
    printf("\nActivity logs:\n");
    for(int i=0;i<nLogs;++i)
        printf("LogID:%d Ani:%d Staff:%d Date:%s What:%s\n",
               logs[i].id, logs[i].whoAnimal, logs[i].whoHelper, logs[i].date, logs[i].what);
}

void getLogByHelper() {
    int sid = askInt("Staff ID: ",1,nHelpers), ff=0;
    for(int i=0;i<nLogs;++i)
        if(logs[i].whoHelper==sid) {
            printf("LogID:%d Ani:%d Date:%s What:%s\n",
                   logs[i].id, logs[i].whoAnimal, logs[i].date, logs[i].what);
            ff=1;
        }
    if(!ff) puts("No logs for that staff.");
}
void getLogByAnimal() {
    int aid = askInt("Animal ID: ",1,nAnimals), ff=0;
    for(int i=0;i<nLogs;++i)
        if(logs[i].whoAnimal==aid) {
            printf("LogID:%d Staff:%d Date:%s What:%s\n",
                   logs[i].id, logs[i].whoHelper, logs[i].date, logs[i].what);
            ff=1;
        }
    if(!ff) puts("No logs for that animal.");
}

int askInt(const char* ask, int lo, int hi) {
    int val, ok; char ch;
    do { printf("%s",ask); ok=scanf("%d",&val); while((ch=getchar())!='\n'&&ch!=EOF);
        if(ok!=1||val<lo||val>hi) printf("Invalid: %d to %d\n",lo,hi);
    } while(ok!=1||val<lo||val>hi);
    return val;
}
