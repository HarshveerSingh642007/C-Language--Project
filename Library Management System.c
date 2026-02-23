/*
    Library Management System
    Author: <Your Name>
    Semester Project (Class 11, Computer Science)
    A basic CLI app for managing books, library members, and borrowing records.
    All arrays are in-memory for simplicity and clarity.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXBOOKS 200
#define MAXMEM 100
#define MAXBORROW 300
#define TITLELEN 60
#define NAMELEN 40

typedef struct {
    int id;
    char title[TITLELEN];
    char author[NAMELEN];
    int year;
    char publisher[NAMELEN];
    int totalCopies;
    int availableCopies;
} Book;

typedef struct {
    int id;
    char name[NAMELEN];
    int memberSince;
    char phone[NAMELEN];
} Member;

typedef struct {
    int id;
    int bookId;
    int memberId;
    char issueDate[20];
    char returnDate[20];
    int returned; // 1 if returned, 0 otherwise
} Borrow;

// Data stores
Book books[MAXBOOKS];
Member members[MAXMEM];
Borrow borrows[MAXBORROW];
int nBooks=0, nMem=0, nBorrow=0;

// Helper function for string search (not case sensitive)
int isSameString(const char *a, const char *b) {
    while(*a && *b) {
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

int askInt(const char *ask, int lo, int hi) {
    int v, ok;
    char ch;
    do {
        printf("%s", ask);
        ok = scanf("%d", &v);
        while((ch = getchar()) != '\n' && ch != EOF);
        if(ok!=1 || v<lo || v>hi) printf("Invalid (%d-%d)\n",lo,hi);
    } while(ok!=1 || v<lo || v>hi);
    return v;
}

// Menus, grouped with readable comments and extra lines
void mainMenu();
void menuBook();
void menuMember();
void menuBorrow();

void addBook();
void listBooks();
void findBookTitle();
void bookAvailable();
void addMember();
void listMembers();
void findMemberName();
void memberSincePeriod();
void borrowBook();
void returnBook();
void showBorrowed();
void booksByMember();

// Main for CLI
int main() {
    printf("Library admin password: ");
    char pass[21];
    scanf("%20s", pass);
    if(strcmp(pass, "lib123")!=0) {
        printf("Incorrect password!\n"); return 0;
    }
    mainMenu();
    return 0;
}

void mainMenu() {
    int ch;
    do {
        printf("\n--- LIBRARY MAIN MENU ---\n");
        printf("1. Books menu\n");
        printf("2. Members menu\n");
        printf("3. Borrow/Return menu\n");
        printf("4. Show Books\n");
        printf("5. Show Members\n");
        printf("6. Show Borrowed/Returned\n");
        printf("7. Quit\n");
        printf("Choose an option: ");
        ch = askInt("", 1, 7);
        if(ch==1) menuBook();
        else if(ch==2) menuMember();
        else if(ch==3) menuBorrow();
        else if(ch==4) listBooks();
        else if(ch==5) listMembers();
        else if(ch==6) showBorrowed();
    } while(ch!=7);
}

// Book Management
void menuBook() {
    int ch;
    do {
        printf("\n--- Books Menu ---\n");
        printf("1. Add new book\n");
        printf("2. List all books\n");
        printf("3. Find by title\n");
        printf("4. Available books\n");
        printf("5. Back\n");
        printf("Select: ");
        ch = askInt("",1,5);
        if(ch==1) addBook();
        else if(ch==2) listBooks();
        else if(ch==3) findBookTitle();
        else if(ch==4) bookAvailable();
    } while(ch!=5);
}

void addBook() {
    Book temp;
    temp.id = nBooks+1;
    printf("Book title: "); scanf(" %[^\n]", temp.title);
    printf("Author: "); scanf(" %[^\n]", temp.author);
    printf("Year: "); temp.year = askInt("",1800,2026);
    printf("Publisher: "); scanf(" %[^\n]", temp.publisher);
    printf("Copies in library: "); temp.totalCopies = askInt("",1,100);
    temp.availableCopies = temp.totalCopies;
    books[nBooks++] = temp;
    printf("Book recorded!\n");
}

void listBooks() {
    printf("\nID\tTitle\tAuthor\tYear\tPublisher\tCopies\tAvailable\n");
    for(int i=0;i<nBooks;++i)
        printf("%d\t%s\t%s\t%d\t%s\t%d\t%d\n", books[i].id, books[i].title, books[i].author,
               books[i].year, books[i].publisher, books[i].totalCopies, books[i].availableCopies);
}

void findBookTitle() {
    char nm[TITLELEN]; int found=0;
    printf("Enter title to search: "); scanf(" %[^\n]", nm);
    for(int i=0;i<nBooks;++i)
        if(isSameString(books[i].title, nm)) {
            printf("ID:%d %s by %s (%d) %s\n", books[i].id, books[i].title, books[i].author,
                   books[i].year, books[i].publisher);
            found=1;
        }
    if(!found) puts("No matching book title.");
}

void bookAvailable() {
    printf("\nBooks in Stock:\n");
    for(int i=0;i<nBooks;++i)
        if(books[i].availableCopies > 0)
            printf("%d: %s [%d available]\n", books[i].id, books[i].title, books[i].availableCopies);
}

// Member Management
void menuMember() {
    int ch;
    do {
        printf("\n--- Members Menu ---\n");
        printf("1. Add member\n");
        printf("2. Show all members\n");
        printf("3. Find member by name\n");
        printf("4. Members since year\n");
        printf("5. Back\n");
        printf("Select: ");
        ch = askInt("",1,5);
        if(ch==1) addMember();
        else if(ch==2) listMembers();
        else if(ch==3) findMemberName();
        else if(ch==4) memberSincePeriod();
    } while(ch!=5);
}

void addMember() {
    Member temp;
    temp.id = nMem+1;
    printf("Name: "); scanf(" %[^\n]", temp.name);
    printf("Membership year: "); temp.memberSince = askInt("",2000,2026);
    printf("Phone: "); scanf(" %[^\n]", temp.phone);
    members[nMem++] = temp;
    printf("Member added!\n");
}

void listMembers() {
    printf("\nID\tName\tSince\tPhone\n");
    for(int i=0;i<nMem;++i)
        printf("%d\t%s\t%d\t%s\n", members[i].id, members[i].name, members[i].memberSince, members[i].phone);
}

void findMemberName() {
    char nm[NAMELEN]; int found=0;
    printf("Search for name: "); scanf(" %[^\n]", nm);
    for(int i=0;i<nMem;++i)
        if(isSameString(members[i].name,nm)) {
            printf("ID:%d %s Since:%d Tel:%s\n", members[i].id, members[i].name, members[i].memberSince, members[i].phone);
            found=1;
        }
    if(!found) puts("No member found.");
}

void memberSincePeriod() {
    int yr = askInt("Year: ",2000,2026);
    for(int i=0;i<nMem;++i)
        if(members[i].memberSince >= yr)
            printf("%d %s since %d\n", members[i].id, members[i].name, members[i].memberSince);
}

// Borrow/Return management
void menuBorrow() {
    int ch;
    do {
        printf("\n--- Borrow Menu ---\n");
        printf("1. Borrow book\n");
        printf("2. Return book\n");
        printf("3. List all borrow/return\n");
        printf("4. Books borrowed by member\n");
        printf("5. Back\n");
        printf("Select: ");
        ch = askInt("",1,5);
        if(ch==1) borrowBook();
        else if(ch==2) returnBook();
        else if(ch==3) showBorrowed();
        else if(ch==4) booksByMember();
    } while(ch!=5);
}

void borrowBook() {
    Borrow temp;
    temp.id = nBorrow+1;
    temp.bookId = askInt("Book ID: ",1,nBooks);
    temp.memberId = askInt("Member ID: ",1,nMem);
    printf("Issue date (dd/mm/yyyy): "); scanf(" %[^\n]", temp.issueDate);

    int i;
    for(i=0;i<nBooks;i++) {
        if(books[i].id == temp.bookId && books[i].availableCopies > 0) {
            books[i].availableCopies--;
            temp.returned = 0;
            strcpy(temp.returnDate,"N/A");
            borrows[nBorrow++] = temp;
            printf("Book issued!\n");
            return;
        }
    }
    printf("Sorry, not available.\n");
}

void returnBook() {
    int bid = askInt("Borrow ID to return: ",1,nBorrow), found=0;
    char rdate[20];
    for(int i=0;i<nBorrow;i++) {
        if(borrows[i].id == bid && borrows[i].returned==0) {
            printf("Return date (dd/mm/yyyy): "); scanf(" %[^\n]", rdate);
            strcpy(borrows[i].returnDate, rdate);
            borrows[i].returned = 1;
            for(int k=0;k<nBooks;k++)
                if(books[k].id == borrows[i].bookId)
                    books[k].availableCopies++;
            printf("Returned!\n");
            found=1;
        }
    }
    if(!found) puts("Record not found/not outstanding.");
}

void showBorrowed() {
    printf("\nID\tBookID\tMemberID\tOut\tIssued\tReturned\n");
    for(int i=0;i<nBorrow;++i)
        printf("%d\t%d\t%d\t%s\t%s\t%s\n", borrows[i].id, borrows[i].bookId, borrows[i].memberId,
            borrows[i].returned?"No":"Yes", borrows[i].issueDate, borrows[i].returnDate);
}

void booksByMember() {
    int mid = askInt("Member ID: ",1,nMem), any=0;
    printf("Books borrowed by member %d:\n", mid);
    for(int i=0;i<nBorrow;i++) {
        if(borrows[i].memberId == mid && borrows[i].returned == 0) {
            for(int j=0;j<nBooks;j++)
                if(books[j].id == borrows[i].bookId)
                    printf("BorrowID:%d %s\n", borrows[i].id, books[j].title);
            any=1;
        }
    }
    if(!any) puts("No outstanding books for that member.");
}
