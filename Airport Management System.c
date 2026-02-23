#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FLIGHT_FILE "flights.txt"
#define PASSENGER_FILE "air_passengers.txt"
#define TICKET_FILE "air_tickets.txt"
#define LOG_FILE "airport_log.txt"
#define CSV_FILE "flight_report.csv"
#define MAXLEN 50

typedef struct {
    int id;
    char airline[MAXLEN], source[MAXLEN], destination[MAXLEN], departure[MAXLEN];
    int totalSeats, availableSeats;
} Flight;

typedef struct {
    int id;
    char name[MAXLEN], passport[MAXLEN];
    int age;
    char gender;
} Passenger;

typedef struct {
    int id, flightId, passengerId, seatNo;
    char date[MAXLEN];
    int cancelled; // 0 for active, 1 for cancelled
} Ticket;

// Data arrays
Flight *flights = NULL;
Passenger *passengers = NULL;
Ticket *tickets = NULL;
int totalFlights = 0, totalPassengers = 0, totalTickets = 0;

// Prototypes
void logAction(const char *info);
int passportValid(const char *p);
int dateValid(const char *d);
int login();
void loadData(), saveAllData(), freeAllMemory();
void mainMenu(), flightMenu(), passengerMenu(), ticketMenu();
void addFlight(), showFlights(), findFlight(), findFlightById(), findFlightByRoute();
void reportFlights(), exportCSV();
void addPassenger(), showPassengers(), findPassenger(), findPassengerById();
void bookTicket(), showTickets(), cancelTicket(), editTicket(), findTicketByPassenger(), findTicketByFlight();
int getValidInt(const char* msg, int min, int max);

// ---------- LOGGING ----------
void logAction(const char *info) {
    FILE *f=fopen(LOG_FILE,"a");
    if(!f) return;
    time_t now=time(0);
    fprintf(f,"%s | %s", info, ctime(&now));
    fclose(f);
}

// ---------- VALIDATION ----------
int passportValid(const char *p) {
    int len=strlen(p);
    if(len<6||len>18) return 0;
    for(int i=0;i<len;++i)
        if(!isalnum(p[i])) return 0;
    return 1;
}
int dateValid(const char *d) {
    int dd,mm,yy;
    return sscanf(d,"%2d/%2d/%4d", &dd,&mm,&yy)==3 && dd>=1 && dd<=31 && mm>=1 && mm<=12 && yy>=2023;
}

// ---------- AUTH ----------
int login() {
    char pass[20];
    printf("Enter admin password to access: ");
    scanf("%19s", pass);
    return strcmp(pass, "airport123")==0;
}

// ---------- LOAD/SAVE ----------
void loadData() {
    FILE *f; Flight fo; Passenger po; Ticket to;
    f=fopen(FLIGHT_FILE,"r");
    while(f&&fscanf(f,"%d %49s %49s %49s %49s %d %d",&fo.id,fo.airline,fo.source,fo.destination,fo.departure,&fo.totalSeats,&fo.availableSeats)==7){
        flights=realloc(flights,sizeof(Flight)*(totalFlights+1)); flights[totalFlights++]=fo;
    } if(f) fclose(f);
    f=fopen(PASSENGER_FILE,"r");
    while(f&&fscanf(f,"%d %49s %d %c %29s",&po.id,po.name,&po.age,&po.gender,po.passport)==5){
        passengers=realloc(passengers,sizeof(Passenger)*(totalPassengers+1)); passengers[totalPassengers++]=po;
    } if(f) fclose(f);
    f=fopen(TICKET_FILE,"r");
    while(f&&fscanf(f,"%d %d %d %d %49s %d",&to.id,&to.flightId,&to.passengerId,&to.seatNo,to.date,&to.cancelled)==6){
        tickets=realloc(tickets,sizeof(Ticket)*(totalTickets+1)); tickets[totalTickets++]=to;
    } if(f) fclose(f);
}
void saveAllData() {
    FILE *f; int i;
    f=fopen(FLIGHT_FILE,"w");
    for(i=0;i<totalFlights;++i) fprintf(f,"%d %s %s %s %s %d %d\n", flights[i].id,flights[i].airline,flights[i].source,flights[i].destination,flights[i].departure,flights[i].totalSeats,flights[i].availableSeats); if(f) fclose(f);
    f=fopen(PASSENGER_FILE,"w");
    for(i=0;i<totalPassengers;++i) fprintf(f,"%d %s %d %c %s\n", passengers[i].id,passengers[i].name,passengers[i].age,passengers[i].gender,passengers[i].passport); if(f) fclose(f);
    f=fopen(TICKET_FILE,"w");
    for(i=0;i<totalTickets;++i) fprintf(f,"%d %d %d %d %s %d\n", tickets[i].id,tickets[i].flightId,tickets[i].passengerId,tickets[i].seatNo,tickets[i].date,tickets[i].cancelled); if(f) fclose(f);
}
void freeAllMemory() { free(flights); free(passengers); free(tickets); }

// ---------- MENUS ----------
void mainMenu() {
    int ch;
    do {
        printf("\n--- Airport Main Menu ---\n");
        printf("1. Flight Menu 2. Passenger Menu 3. Ticket Menu 4. Flight Reports 5. Export Flight CSV 6. Show All Flights 7. Show All Tickets 8. Exit\n");
        ch=getValidInt("Enter choice: ",1,8);
        if(ch==1) flightMenu();
        else if(ch==2) passengerMenu();
        else if(ch==3) ticketMenu();
        else if(ch==4) reportFlights();
        else if(ch==5) exportCSV();
        else if(ch==6) showFlights();
        else if(ch==7) showTickets();
    }while(ch!=8);
    logAction("System exited.");
}
void flightMenu() {
    int ch;
    do {
        printf("\n--- Flight Menu ---\n");
        printf("1.Add Flight 2.Show All 3.Find by Airline 4.Find by ID 5.Find by Route 6.Edit Flight Seats 7.Back\n");
        ch=getValidInt("Enter choice: ",1,7);
        if(ch==1) addFlight();
        else if(ch==2) showFlights();
        else if(ch==3) findFlight();
        else if(ch==4) findFlightById();
        else if(ch==5) findFlightByRoute();
        else if(ch==6){
            int id=getValidInt("Enter ID to edit seats:",1,flights?flights[totalFlights-1].id:1);
            for(int i=0;i<totalFlights;++i)
                if(flights[i].id==id){
                    flights[i].availableSeats=getValidInt("Set available seats (<=total):",1,flights[i].totalSeats);
                    printf("Seats updated!\n");
                    logAction("Flight seats edited.");
                    break;
                }
        }
    }while(ch!=7);
}
void reportFlights() {
    printf("\n--- Flights on record ---\n");
    int i; int occupied=0, total=0;
    printf("ID Airline Route Time Total Available\n");
    for(i=0;i<totalFlights;++i){
        printf("%d %s %s->%s %s %d %d\n", flights[i].id,flights[i].airline,flights[i].source,flights[i].destination,flights[i].departure,flights[i].totalSeats,flights[i].availableSeats);
        total+=flights[i].totalSeats; occupied+=(flights[i].totalSeats-flights[i].availableSeats);
    }
    printf("Total seats: %d, Occupied: %d, Free: %d\n", total,occupied,total-occupied);
    logAction("Report generated.");
}
void exportCSV() {
    FILE *f=fopen(CSV_FILE,"w");
    if(!f) return;
    int i;
    fprintf(f,"ID,Airline,Source,Dest,Departure,Total,Available\n");
    for(i=0;i<totalFlights;++i){
        fprintf(f,"%d,%s,%s,%s,%s,%d,%d\n", flights[i].id,flights[i].airline,flights[i].source,flights[i].destination,flights[i].departure,flights[i].totalSeats,flights[i].availableSeats);
    }
    fclose(f);
    printf("CSV export ready: %s\n", CSV_FILE);
    logAction("CSV exported.");
}
void addFlight() {
    Flight temp;
    temp.id = totalFlights?flights[totalFlights-1].id+1:1;
    printf("Airline: "); scanf(" %[^\n]", temp.airline);
    printf("Source airport: "); scanf(" %[^\n]", temp.source);
    printf("Destination airport: "); scanf(" %[^\n]", temp.destination);
    printf("Departure time (HH:MM): "); scanf(" %[^\n]", temp.departure);
    temp.totalSeats=getValidInt("Total seats (50-400):",50,400);
    temp.availableSeats=temp.totalSeats;
    flights=realloc(flights,sizeof(Flight)*(totalFlights+1));
    flights[totalFlights++]=temp;
    saveAllData();
    printf("Flight Added!\n");
    logAction("Flight added.");
}
void showFlights() {
    printf("\nID Airline Route Time Total Free\n");
    for(int i=0;i<totalFlights;++i)
        printf("%d %s %s->%s %s %d %d\n", flights[i].id,flights[i].airline,flights[i].source,flights[i].destination,flights[i].departure,flights[i].totalSeats,flights[i].availableSeats);
}
void findFlight() {
    char airline[MAXLEN]; int found=0;
    printf("Airline to search: "); scanf(" %[^\n]", airline);
    for(int i=0;i<totalFlights;++i)
        if(strcasecmp(flights[i].airline, airline)==0){
            printf("Flight %d: %s->%s on %s, %d/%d seats\n",flights[i].id,flights[i].source,flights[i].destination,flights[i].departure,flights[i].availableSeats,flights[i].totalSeats); found=1;
        }
    if(!found) printf("No flight found!\n");
}
void findFlightById() {
    int id=getValidInt("Flight ID: ",1,flights?flights[totalFlights-1].id:1), found=0;
    for(int i=0;i<totalFlights;++i)
        if(flights[i].id==id){
            printf("Flight %d: %s->%s %s, Airline %s, Seats %d/%d\n",flights[i].id,flights[i].source,flights[i].destination,flights[i].departure,flights[i].airline,flights[i].availableSeats,flights[i].totalSeats); found=1;
        }
    if(!found) printf("No flight found!\n");
}
void findFlightByRoute() {
    char src[MAXLEN], dst[MAXLEN]; int found=0;
    printf("Source: "); scanf(" %[^\n]", src);
    printf("Destination: "); scanf(" %[^\n]", dst);
    for(int i=0;i<totalFlights;++i)
        if(strcasecmp(flights[i].source, src)==0 && strcasecmp(flights[i].destination, dst)==0){
            printf("Flight %d: %s->%s %s, Airline %s, Seats %d/%d\n",flights[i].id,flights[i].source,flights[i].destination,flights[i].departure,flights[i].airline,flights[i].availableSeats,flights[i].totalSeats); found=1;
        }
    if(!found) printf("No flight for this route!\n");
}
void passengerMenu() {
    int ch;
    do {
        printf("\n--- Passenger Menu ---\n");
        printf("1.Add Passenger 2.Show All 3.Find by Name 4.Find by ID 5.Back\n");
        ch=getValidInt("Enter choice: ",1,5);
        if(ch==1) addPassenger();
        else if(ch==2) showPassengers();
        else if(ch==3) findPassenger();
        else if(ch==4) findPassengerById();
    }while(ch!=5);
}
void addPassenger() {
    Passenger temp;
    temp.id=totalPassengers?passengers[totalPassengers-1].id+1:1;
    printf("Name: "); scanf(" %[^\n]", temp.name);
    temp.age=getValidInt("Age (1-120):",1,120);
    do{printf("Gender (M/F): "); scanf(" %c",&temp.gender); temp.gender=toupper(temp.gender);}while(temp.gender!='M'&&temp.gender!='F');
    do{printf("Passport No: "); scanf(" %[^\n]", temp.passport);}while(!passportValid(temp.passport));
    passengers=realloc(passengers,sizeof(Passenger)*(totalPassengers+1)); passengers[totalPassengers++]=temp;
    saveAllData();
    printf("Passenger Added!\n");
    logAction("Passenger added.");
}
void showPassengers() {
    printf("\nID Name Age Gender Passport\n");
    for(int i=0;i<totalPassengers;++i)
        printf("%d %s %d %c %s\n", passengers[i].id,passengers[i].name,passengers[i].age,passengers[i].gender,passengers[i].passport);
}
void findPassenger() {
    char n[MAXLEN]; int found=0;
    printf("Passenger Name: "); scanf(" %[^\n]", n);
    for(int i=0;i<totalPassengers;++i)
        if(strcasecmp(passengers[i].name, n)==0){
            printf("Passenger %d: %s, Age %d, Gender %c, Passport %s\n",passengers[i].id,passengers[i].name,passengers[i].age,passengers[i].gender,passengers[i].passport); found=1;
        }
    if(!found) printf("Passenger not found!\n");
}
void findPassengerById() {
    int id=getValidInt("ID: ",1,passengers?passengers[totalPassengers-1].id:1), found=0;
    for(int i=0;i<totalPassengers;++i)
        if(passengers[i].id==id){
            printf("Passenger %d: %s, Age %d, Gender %c, Passport %s\n",passengers[i].id,passengers[i].name,passengers[i].age,passengers[i].gender,passengers[i].passport); found=1;
        }
    if(!found) printf("Passenger not found!\n");
}
void ticketMenu() {
    int ch;
    do {
        printf("\n--- Ticket Menu ---\n");
        printf("1.Book Ticket 2.Show Tickets 3.Cancel Ticket 4.Edit Ticket Date 5.Find by Passenger 6.Find by Flight 7.Back\n");
        ch=getValidInt("Enter choice: ",1,7);
        if(ch==1) bookTicket();
        else if(ch==2) showTickets();
        else if(ch==3) cancelTicket();
        else if(ch==4) editTicket();
        else if(ch==5) findTicketByPassenger();
        else if(ch==6) findTicketByFlight();
    }while(ch!=7);
}
void bookTicket() {
    Ticket temp; int fid,pid,idx=-1;
    temp.id=totalTickets?tickets[totalTickets-1].id+1:1;
    temp.flightId = getValidInt("Flight ID: ", 1, flights?flights[totalFlights-1].id:1);
    for(int i=0;i<totalFlights;++i) if(flights[i].id==temp.flightId) idx=i;
    if(idx==-1||flights[idx].availableSeats==0){printf("No seat!\n");return;}
    temp.passengerId = getValidInt("Passenger ID: ", 1, passengers?passengers[totalPassengers-1].id:1);
    temp.seatNo=flights[idx].totalSeats-flights[idx].availableSeats+1;
    do{printf("Date (dd/mm/yyyy): "); scanf(" %[^\n]", temp.date);}while(!dateValid(temp.date));
    temp.cancelled=0; flights[idx].availableSeats-=1;
    tickets=realloc(tickets,sizeof(Ticket)*(totalTickets+1)); tickets[totalTickets++]=temp;
    saveAllData();
    printf("Ticket booked! Seat %d\n", temp.seatNo);
    logAction("Ticket booked.");
}
void showTickets() {
    printf("\nID Flight Passenger Seat Date Cancelled\n");
    for(int i=0;i<totalTickets;++i)
        printf("%d %d %d %d %s %s\n", tickets[i].id, tickets[i].flightId, tickets[i].passengerId, tickets[i].seatNo, tickets[i].date, tickets[i].cancelled?"Cancelled":"Active");
}
void cancelTicket() {
    int id=getValidInt("Ticket ID to cancel:",1,tickets?tickets[totalTickets-1].id:1),found=0;
    for(int i=0;i<totalTickets;++i)
        if(tickets[i].id==id&&!tickets[i].cancelled){
            tickets[i].cancelled=1;
            // restore seat
            for(int j=0;j<totalFlights;++j) if(flights[j].id==tickets[i].flightId) flights[j].availableSeats+=1;
            printf("Ticket cancelled!\n"); logAction("Ticket cancelled."); found=1; break;
        }
    if(!found) printf("Ticket not found or already cancelled!\n");
}
void editTicket() {
    int id=getValidInt("Ticket ID to edit date:",1,tickets?tickets[totalTickets-1].id:1),found=0;
    for(int i=0;i<totalTickets;++i)
        if(tickets[i].id==id&&!tickets[i].cancelled){
            do{printf("New Date (dd/mm/yyyy): "); scanf(" %[^\n]", tickets[i].date);}while(!dateValid(tickets[i].date));
            printf("Date updated!\n"); logAction("Ticket date changed."); found=1; break;
        }
    if(!found) printf("Ticket not found or cancelled!\n");
}
void findTicketByPassenger() {
    int pid=getValidInt("Passenger ID:",1,passengers?passengers[totalPassengers-1].id:1),found=0;
    for(int i=0;i<totalTickets;++i)
        if(tickets[i].passengerId==pid&&!tickets[i].cancelled){
            printf("Ticket %d: Flight %d, Seat %d, Date %s\n",tickets[i].id,tickets[i].flightId,tickets[i].seatNo,tickets[i].date); found=1;
        }
    if(!found) printf("No ticket found!\n");
}
void findTicketByFlight() {
    int fid=getValidInt("Flight ID:",1,flights?flights[totalFlights-1].id:1),found=0;
    for(int i=0;i<totalTickets;++i)
        if(tickets[i].flightId==fid&&!tickets[i].cancelled){
            printf("Ticket %d: Passenger %d, Seat %d, Date %s\n",tickets[i].id,tickets[i].passengerId,tickets[i].seatNo,tickets[i].date); found=1;
        }
    if(!found) printf("No ticket found!\n");
}
// ---------- Utility ----------
int getValidInt(const char* msg, int min, int max) {
    int val,ok;char ch;
    do{printf("%s",msg); ok=scanf("%d",&val); while((ch=getchar())!='\n'&&ch!=EOF);
        if(ok!=1||val<min||val>max) printf("Invalid! Range: %d-%d\n",min,max);
    }while(ok!=1||val<min||val>max);
    return val;
}
