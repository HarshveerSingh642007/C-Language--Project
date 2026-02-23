#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUS_FILE       "buses.txt"
#define PASSENGER_FILE "passengers.txt"
#define BOOKING_FILE   "bookings.txt"

// ----- Structures -----
typedef struct {
    int id;
    char name[50];
    char source[30];
    char destination[30];
    int totalSeats;
    int availableSeats;
} Bus;

typedef struct {
    int id;
    char name[50];
    int age;
    char gender;
    char mobile[15];
} Passenger;

typedef struct {
    int id;
    int busId;
    int passengerId;
    int seatNo;
    char date[15];
} Booking;

// ----- Dynamic Arrays -----
Bus *buses = NULL; Passenger *passengers = NULL; Booking *bookings = NULL;
int totalBuses = 0, totalPassengers = 0, totalBookings = 0;

// ----- Function Prototypes -----
int login();
void loadData(), saveAllData(), freeAllMemory();
void mainMenu(), busMenu(), passengerMenu(), bookingMenu();
void addBus(), showBuses(), findBus(), findBusById(), findBusByRoute();
void addPassenger(), showPassengers(), findPassenger(), findPassengerById();
void bookSeat(), showBookings(), findBookingByPassenger(), findBookingByBus();
int getValidInt(const char* msg, int min, int max);

// ----- Main -----
int main() {
    printf("================= BUS RESERVATION SYSTEM =================\n");
    printf("Project includes: File handling, dynamic memory, input validation, menu, auth\n");
    if(!login()) { printf("Access denied!\n"); return 0; }
    loadData();
    mainMenu();
    saveAllData();
    freeAllMemory();
    return 0;
}

// ----- Authentication -----
int login() {
    char pwd[20];
    printf("Enter admin password to access: ");
    scanf("%19s", pwd);
    return strcmp(pwd, "bus123") == 0;
}

// ----- Data Load/Save/Free -----
void loadData() {
    FILE *f;
    Bus bTemp; Passenger pTemp; Booking kTemp;
    // Buses
    f = fopen(BUS_FILE, "r");
    while(f && fscanf(f, "%d %49s %29s %29s %d %d", &bTemp.id, bTemp.name, bTemp.source, bTemp.destination, &bTemp.totalSeats, &bTemp.availableSeats) == 6) {
        buses = realloc(buses, sizeof(Bus)*(totalBuses+1));
        buses[totalBuses++] = bTemp;
    }
    if(f) fclose(f);
    // Passengers
    f = fopen(PASSENGER_FILE, "r");
    while(f && fscanf(f, "%d %49s %d %c %14s", &pTemp.id, pTemp.name, &pTemp.age, &pTemp.gender, pTemp.mobile) == 5) {
        passengers = realloc(passengers, sizeof(Passenger)*(totalPassengers+1));
        passengers[totalPassengers++] = pTemp;
    }
    if(f) fclose(f);
    // Bookings
    f = fopen(BOOKING_FILE, "r");
    while(f && fscanf(f, "%d %d %d %d %14s", &kTemp.id, &kTemp.busId, &kTemp.passengerId, &kTemp.seatNo, kTemp.date) == 5) {
        bookings = realloc(bookings, sizeof(Booking)*(totalBookings+1));
        bookings[totalBookings++] = kTemp;
    }
    if(f) fclose(f);
}
void saveAllData() {
    FILE *f;
    int i;
    // Buses
    f = fopen(BUS_FILE, "w");
    for(i=0;i<totalBuses;++i)
        fprintf(f, "%d %s %s %s %d %d\n", buses[i].id, buses[i].name, buses[i].source, buses[i].destination, buses[i].totalSeats, buses[i].availableSeats);
    if(f) fclose(f);
    // Passengers
    f = fopen(PASSENGER_FILE, "w");
    for(i=0;i<totalPassengers;++i)
        fprintf(f, "%d %s %d %c %s\n", passengers[i].id, passengers[i].name, passengers[i].age, passengers[i].gender, passengers[i].mobile);
    if(f) fclose(f);
    // Bookings
    f = fopen(BOOKING_FILE, "w");
    for(i=0;i<totalBookings;++i)
        fprintf(f, "%d %d %d %d %s\n", bookings[i].id, bookings[i].busId, bookings[i].passengerId, bookings[i].seatNo, bookings[i].date);
    if(f) fclose(f);
}
void freeAllMemory() {
    free(buses); free(passengers); free(bookings);
}

// ----- Main Menu -----
void mainMenu() {
    int ch;
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Bus Menu\n2. Passenger Menu\n3. Booking Menu\n4. Show All Buses\n5. Show All Bookings\n6. Exit\n");
        ch = getValidInt("Enter your choice: ", 1, 6);
        if(ch==1) busMenu();
        else if(ch==2) passengerMenu();
        else if(ch==3) bookingMenu();
        else if(ch==4) showBuses();
        else if(ch==5) showBookings();
    } while(ch!=6);
}
// ----- Bus Features -----
void busMenu() {
    int ch;
    do {
        printf("\n--- Bus Menu ---\n");
        printf("1. Add Bus\n2. Show All Buses\n3. Find Bus by Name\n4. Find Bus by ID\n5. Find Bus by Route\n6. Back\n");
        ch = getValidInt("Enter choice: ", 1, 6);
        if(ch==1) addBus();
        else if(ch==2) showBuses();
        else if(ch==3) findBus();
        else if(ch==4) findBusById();
        else if(ch==5) findBusByRoute();
    } while(ch!=6);
}
void addBus() {
    Bus temp;
    temp.id = totalBuses ? buses[totalBuses-1].id+1 : 1;
    printf("Enter bus name: ");
    scanf(" %[^\n]", temp.name);
    printf("Enter source city: ");
    scanf(" %[^\n]", temp.source);
    printf("Enter destination city: ");
    scanf(" %[^\n]", temp.destination);
    temp.totalSeats = getValidInt("Total seats on bus: ", 1, 100);
    temp.availableSeats = temp.totalSeats;
    buses = realloc(buses, sizeof(Bus)*(totalBuses+1));
    buses[totalBuses++] = temp;
    saveAllData();
    printf("Bus added!\n");
}
void showBuses() {
    int i;
    printf("\nID\tName\tSource\tDestination\tTotal\tAvailable\n");
    for(i=0;i<totalBuses;++i)
        printf("%d\t%s\t%s\t%s\t%d\t%d\n", buses[i].id, buses[i].name, buses[i].source, buses[i].destination, buses[i].totalSeats, buses[i].availableSeats);
}
void findBus() {
    char n[50]; int found=0;
    printf("Enter bus name to search: ");
    scanf(" %[^\n]", n);
    for(int i=0;i<totalBuses;++i)
        if(strcasecmp(buses[i].name, n)==0) {
            printf("Bus Found: ID %d, %s (%s -> %s, Seats: %d/%d)\n", buses[i].id, buses[i].name, buses[i].source, buses[i].destination, buses[i].availableSeats, buses[i].totalSeats);
            found=1;
        }
    if(!found) printf("No bus found!\n");
}
void findBusById() {
    int id = getValidInt("Enter bus ID: ", 1, buses ? buses[totalBuses-1].id : 1), found=0;
    for(int i=0;i<totalBuses;++i)
        if(buses[i].id==id) {
            printf("Bus Found: ID %d, %s (%s -> %s, Seats: %d/%d)\n", buses[i].id, buses[i].name, buses[i].source, buses[i].destination, buses[i].availableSeats, buses[i].totalSeats);
            found=1;
        }
    if(!found) printf("No bus found!\n");
}
void findBusByRoute() {
    char src[30], dst[30]; int found=0;
    printf("Enter source: "); scanf(" %[^\n]", src);
    printf("Enter destination: "); scanf(" %[^\n]", dst);
    for(int i=0;i<totalBuses;++i)
        if(strcasecmp(buses[i].source, src)==0 && strcasecmp(buses[i].destination, dst)==0) {
            printf("Bus Found: ID %d, %s (%s -> %s, %d/%d seats left)\n", buses[i].id, buses[i].name, buses[i].source, buses[i].destination, buses[i].availableSeats, buses[i].totalSeats);
            found=1;
        }
    if(!found) printf("No bus for this route found!\n");
}

// ----- Passenger Features -----
void passengerMenu() {
    int ch;
    do {
        printf("\n--- Passenger Menu ---\n");
        printf("1. Add Passenger\n2. Show All Passengers\n3. Find Passenger by Name\n4. Find Passenger by ID\n5. Back\n");
        ch = getValidInt("Enter choice: ", 1, 5);
        if(ch==1) addPassenger();
        else if(ch==2) showPassengers();
        else if(ch==3) findPassenger();
        else if(ch==4) findPassengerById();
    } while(ch!=5);
}
void addPassenger() {
    Passenger temp;
    temp.id = totalPassengers ? passengers[totalPassengers-1].id+1 : 1;
    printf("Enter passenger name: ");
    scanf(" %[^\n]", temp.name);
    temp.age = getValidInt("Enter age: ", 1, 120);
    do {
        printf("Enter gender (M/F): ");
        scanf(" %c", &temp.gender);
        temp.gender = toupper(temp.gender);
    } while(temp.gender!='M' && temp.gender!='F');
    printf("Enter mobile number: ");
    scanf(" %14s", temp.mobile);
    passengers = realloc(passengers, sizeof(Passenger)*(totalPassengers+1));
    passengers[totalPassengers++] = temp;
    saveAllData();
    printf("Passenger added!\n");
}
void showPassengers() {
    printf("\nID\tName\tAge\tGender\tMobile\n");
    for(int i=0;i<totalPassengers;++i)
        printf("%d\t%s\t%d\t%c\t%s\n", passengers[i].id, passengers[i].name, passengers[i].age, passengers[i].gender, passengers[i].mobile);
}
void findPassenger() {
    char n[50]; int found=0;
    printf("Enter passenger name: ");
    scanf(" %[^\n]", n);
    for(int i=0;i<totalPassengers;++i)
        if(strcasecmp(passengers[i].name, n)==0) {
            printf("Passenger Found: ID %d, %s, %d years, %c, Mobile: %s\n", passengers[i].id, passengers[i].name, passengers[i].age, passengers[i].gender, passengers[i].mobile);
            found=1;
        }
    if(!found) printf("No passenger found!\n");
}
void findPassengerById() {
    int id = getValidInt("Enter passenger ID: ", 1, passengers ? passengers[totalPassengers-1].id : 1), found=0;
    for(int i=0;i<totalPassengers;++i)
        if(passengers[i].id==id) {
            printf("Passenger Found: ID %d, %s, %d years, %c, Mobile: %s\n", passengers[i].id, passengers[i].name, passengers[i].age, passengers[i].gender, passengers[i].mobile);
            found=1;
        }
    if(!found) printf("No passenger found!\n");
}

// ----- Booking Features -----
void bookingMenu() {
    int ch;
    do {
        printf("\n--- Booking Menu ---\n");
        printf("1. Book Seat\n2. Show All Bookings\n3. Find Booking by Passenger\n4. Find Booking by Bus\n5. Back\n");
        ch = getValidInt("Enter choice: ", 1, 5);
        if(ch==1) bookSeat();
        else if(ch==2) showBookings();
        else if(ch==3) findBookingByPassenger();
        else if(ch==4) findBookingByBus();
    } while(ch!=5);
}
void bookSeat() {
    Booking temp;
    temp.id = totalBookings ? bookings[totalBookings-1].id+1 : 1;
    temp.busId = getValidInt("Enter bus ID: ", 1, buses ? buses[totalBuses-1].id : 1);
    // check seat availability
    int busIdx=-1;
    for(int i=0;i<totalBuses;++i) if(buses[i].id==temp.busId) busIdx=i;
    if(busIdx==-1) { printf("Bus not found!\n"); return; }
    if(buses[busIdx].availableSeats==0) { printf("No seats available on this bus!\n"); return; }
    temp.passengerId = getValidInt("Enter passenger ID: ", 1, passengers ? passengers[totalPassengers-1].id:1);
    temp.seatNo = buses[busIdx].totalSeats - buses[busIdx].availableSeats + 1;
    printf("Enter journey date (dd/mm/yyyy): ");
    scanf(" %[^\n]", temp.date);
    buses[busIdx].availableSeats -= 1;
    bookings = realloc(bookings, sizeof(Booking)*(totalBookings+1));
    bookings[totalBookings++] = temp;
    saveAllData();
    printf("Booking successful! Seat number: %d\n", temp.seatNo);
}
void showBookings() {
    printf("\nBookingID\tBusID\tPassengerID\tSeatNo\tDate\n");
    for(int i=0;i<totalBookings;++i)
        printf("%d\t\t%d\t%d\t\t%d\t%s\n", bookings[i].id, bookings[i].busId, bookings[i].passengerId, bookings[i].seatNo, bookings[i].date);
}
void findBookingByPassenger() {
    int pid=getValidInt("Enter passenger ID: ", 1, passengers ? passengers[totalPassengers-1].id:1), found=0;
    for(int i=0;i<totalBookings;++i)
        if(bookings[i].passengerId==pid) {
            printf("Booking: ID %d, Bus %d, Seat %d, Date %s\n", bookings[i].id, bookings[i].busId, bookings[i].seatNo, bookings[i].date);
            found=1;
        }
    if(!found) printf("No booking found for this passenger!\n");
}
void findBookingByBus() {
    int bid=getValidInt("Enter bus ID: ", 1, buses ? buses[totalBuses-1].id:1), found=0;
    for(int i=0;i<totalBookings;++i)
        if(bookings[i].busId==bid) {
            printf("Booking: ID %d, Seat %d, Passenger %d, Date %s\n", bookings[i].id, bookings[i].seatNo, bookings[i].passengerId, bookings[i].date);
            found=1;
        }
    if(!found) printf("No booking found for this bus!\n");
}

// ----- Utilities -----
int getValidInt(const char* msg, int min, int max) {
    int val, ok;
    char ch;
    do {
        printf("%s", msg);
        ok = scanf("%d", &val);
        while((ch=getchar())!='\n' && ch!=EOF);
        if(ok!=1 || val<min || val>max)
            printf("Invalid! Enter between %d and %d.\n", min, max);
    } while(ok!=1 || val<min || val>max);
    return val;
}
