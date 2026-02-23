#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ========== Structures ==========

typedef struct {
    int patientId;
    char patientName[50];
    int patientAge;
    char patientGender;
    char patientDisease[50];
} Patient;

typedef struct {
    int doctorId;
    char doctorName[50];
    char doctorSpeciality[50];
} Doctor;

typedef struct {
    int appointmentId;
    int patientId;
    int doctorId;
    char appointmentDate[15];
} Appointment;

// ========== Global Lists and Counters ==========

Patient allPatients[100];
Doctor allDoctors[50];
Appointment allAppointments[100];
int totalPatients = 0, totalDoctors = 0, totalAppointments = 0;

// ========== Function Prototypes ==========

void mainMenu();
int login();
void patientMenu();
void doctorMenu();
void appointmentMenu();
void addPatient();
void showPatients();
void findPatient();
void findPatientByDisease();
void findPatientById();
void addDoctor();
void showDoctors();
void findDoctor();
void findDoctorBySpeciality();
void findDoctorById();
void bookAppointment();
void showAppointments();
int getValidInt(const char* message, int min, int max);
void clearInputBuffer();

// ========== Main Function ==========

int main() {
    printf("========== Hospital Management System ==========\n");
    if(!login()) {
        printf("Access denied.\n");
        return 0;
    }
    mainMenu();
    return 0;
}

// ========== Authentication (Simple Password) ==========

int login() {
    char password[20];
    printf("Enter admin password to access: ");
    scanf("%19s", password);
    if(strcmp(password, "admin123") == 0) {
        printf("Welcome, Admin!\n");
        return 1;
    } else {
        printf("Wrong password!\n");
        return 0;
    }
}

// ========== Menus ==========

void mainMenu() {
    int choice;
    do {
        printf("\n------ Main Menu ------\n");
        printf("1. Patient Menu\n");
        printf("2. Doctor Menu\n");
        printf("3. Appointment Menu\n");
        printf("4. Print All Patients List\n");
        printf("5. Print All Doctors List\n");
        printf("6. Exit\n");
        choice = getValidInt("Enter your choice: ", 1, 6);
        if(choice == 1) patientMenu();
        else if(choice == 2) doctorMenu();
        else if(choice == 3) appointmentMenu();
        else if(choice == 4) showPatients();
        else if(choice == 5) showDoctors();
    } while(choice != 6);
}

void patientMenu() {
    int choice;
    do {
        printf("\n--- Patient Menu ---\n");
        printf("1. Add Patient\n");
        printf("2. Show All Patients\n");
        printf("3. Find Patient by Name\n");
        printf("4. Find Patient by ID\n");
        printf("5. Find Patient by Disease\n");
        printf("6. Back to Main Menu\n");
        choice = getValidInt("Enter choice: ", 1, 6);
        if(choice == 1) addPatient();
        else if(choice == 2) showPatients();
        else if(choice == 3) findPatient();
        else if(choice == 4) findPatientById();
        else if(choice == 5) findPatientByDisease();
    } while(choice != 6);
}

void doctorMenu() {
    int choice;
    do {
        printf("\n--- Doctor Menu ---\n");
        printf("1. Add Doctor\n");
        printf("2. Show All Doctors\n");
        printf("3. Find Doctor by Name\n");
        printf("4. Find Doctor by ID\n");
        printf("5. Find Doctor by Speciality\n");
        printf("6. Back to Main Menu\n");
        choice = getValidInt("Enter choice: ", 1, 6);
        if(choice == 1) addDoctor();
        else if(choice == 2) showDoctors();
        else if(choice == 3) findDoctor();
        else if(choice == 4) findDoctorById();
        else if(choice == 5) findDoctorBySpeciality();
    } while(choice != 6);
}

void appointmentMenu() {
    int choice;
    do {
        printf("\n--- Appointment Menu ---\n");
        printf("1. Book Appointment\n");
        printf("2. Show All Appointments\n");
        printf("3. Back to Main Menu\n");
        choice = getValidInt("Enter choice: ", 1, 3);
        if(choice == 1) bookAppointment();
        else if(choice == 2) showAppointments();
    } while(choice != 3);
}

// ========== Patient Operations ==========

void addPatient() {
    Patient newPatient;
    newPatient.patientId = totalPatients + 1;

    printf("Enter patient name: ");
    scanf(" %[^\n]", newPatient.patientName);

    newPatient.patientAge = getValidInt("Enter patient age: ", 1, 120);

    do {
        printf("Enter patient gender (M/F): ");
        scanf(" %c", &newPatient.patientGender);
        newPatient.patientGender = toupper(newPatient.patientGender);
    } while(newPatient.patientGender != 'M' && newPatient.patientGender != 'F');

    printf("Enter patient disease: ");
    scanf(" %[^\n]", newPatient.patientDisease);

    allPatients[totalPatients++] = newPatient;
    printf("Patient added!\n");
}

void showPatients() {
    printf("\n--- Patient List ---\n");
    if(totalPatients == 0) {
        printf("No patients available.\n");
        return;
    }
    printf("ID\tName\t\tAge\tGender\tDisease\n");
    for(int i = 0; i < totalPatients; i++) {
        printf("%d\t%s\t%d\t%c\t%s\n",
            allPatients[i].patientId,
            allPatients[i].patientName,
            allPatients[i].patientAge,
            allPatients[i].patientGender,
            allPatients[i].patientDisease);
    }
}

void findPatient() {
    char name[50];
    int found = 0;
    printf("Enter name to search: ");
    scanf(" %[^\n]", name);
    for(int i = 0; i < totalPatients; i++) {
        if(strcasecmp(allPatients[i].patientName, name) == 0) {
            printf("Found: ID %d, Name: %s, Age: %d, Gender: %c, Disease: %s\n",
                allPatients[i].patientId, allPatients[i].patientName,
                allPatients[i].patientAge, allPatients[i].patientGender,
                allPatients[i].patientDisease);
            found = 1;
        }
    }
    if(!found) printf("No patient found by that name.\n");
}

void findPatientById() {
    int id = getValidInt("Enter patient ID: ", 1, totalPatients);
    int found = 0;
    for(int i = 0; i < totalPatients; i++) {
        if(allPatients[i].patientId == id) {
            printf("Found: ID %d, Name: %s, Age: %d, Gender: %c, Disease: %s\n",
                allPatients[i].patientId,
                allPatients[i].patientName,
                allPatients[i].patientAge,
                allPatients[i].patientGender,
                allPatients[i].patientDisease);
            found = 1;
        }
    }
    if(!found) printf("No patient found with that ID.\n");
}

void findPatientByDisease() {
    char disease[50];
    int found = 0;
    printf("Enter disease to search: ");
    scanf(" %[^\n]", disease);
    for(int i = 0; i < totalPatients; i++) {
        if(strcasecmp(allPatients[i].patientDisease, disease) == 0) {
            printf("Found: ID %d, Name: %s, Disease: %s\n",
                allPatients[i].patientId, allPatients[i].patientName,
                allPatients[i].patientDisease);
            found = 1;
        }
    }
    if(!found) printf("No patient found with that disease.\n");
}

// ========== Doctor Operations ==========

void addDoctor() {
    Doctor newDoctor;
    newDoctor.doctorId = totalDoctors + 1;

    printf("Enter doctor name: ");
    scanf(" %[^\n]", newDoctor.doctorName);

    printf("Enter doctor speciality: ");
    scanf(" %[^\n]", newDoctor.doctorSpeciality);

    allDoctors[totalDoctors++] = newDoctor;
    printf("Doctor added!\n");
}

void showDoctors() {
    printf("\n--- Doctor List ---\n");
    if(totalDoctors == 0) {
        printf("No doctors available.\n");
        return;
    }
    printf("ID\tName\t\tSpeciality\n");
    for(int i = 0; i < totalDoctors; i++) {
        printf("%d\t%s\t\t%s\n",
            allDoctors[i].doctorId,
            allDoctors[i].doctorName,
            allDoctors[i].doctorSpeciality);
    }
}

void findDoctor() {
    char name[50];
    int found = 0;
    printf("Enter name to search: ");
    scanf(" %[^\n]", name);
    for(int i = 0; i < totalDoctors; i++) {
        if(strcasecmp(allDoctors[i].doctorName, name) == 0) {
            printf("Found: ID %d, Name: %s, Speciality: %s\n",
                allDoctors[i].doctorId, allDoctors[i].doctorName,
                allDoctors[i].doctorSpeciality);
            found = 1;
        }
    }
    if(!found) printf("No doctor found by that name.\n");
}

void findDoctorById() {
    int id = getValidInt("Enter doctor ID: ", 1, totalDoctors);
    int found = 0;
    for(int i = 0; i < totalDoctors; i++) {
        if(allDoctors[i].doctorId == id) {
            printf("Found: ID %d, Name: %s, Speciality: %s\n",
                allDoctors[i].doctorId, allDoctors[i].doctorName,
                allDoctors[i].doctorSpeciality);
            found = 1;
        }
    }
    if(!found) printf("No doctor found with that ID.\n");
}

void findDoctorBySpeciality() {
    char speciality[50];
    int found = 0;
    printf("Enter speciality to search: ");
    scanf(" %[^\n]", speciality);
    for(int i = 0; i < totalDoctors; i++) {
        if(strcasecmp(allDoctors[i].doctorSpeciality, speciality) == 0) {
            printf("Found: ID %d, Name: %s, Speciality: %s\n",
                allDoctors[i].doctorId, allDoctors[i].doctorName,
                allDoctors[i].doctorSpeciality);
            found = 1;
        }
    }
    if(!found) printf("No doctor found with that speciality.\n");
}

// ========== Appointment Operations ==========

void bookAppointment() {
    Appointment newApp;
    newApp.appointmentId = totalAppointments + 1;
    newApp.patientId = getValidInt("Enter valid patient ID: ", 1, totalPatients);
    newApp.doctorId = getValidInt("Enter valid doctor ID: ", 1, totalDoctors);
    printf("Enter appointment date (dd/mm/yyyy): ");
    scanf(" %[^\n]", newApp.appointmentDate);

    allAppointments[totalAppointments++] = newApp;
    printf("Appointment booked!\n");
}

void showAppointments() {
    printf("\n--- Appointments List ---\n");
    if(totalAppointments == 0) {
        printf("No appointments scheduled.\n");
        return;
    }
    printf("ID\tPatientID\tDoctorID\tDate\n");
    for(int i = 0; i < totalAppointments; i++) {
        printf("%d\t%d\t\t%d\t\t%s\n",
            allAppointments[i].appointmentId,
            allAppointments[i].patientId,
            allAppointments[i].doctorId,
            allAppointments[i].appointmentDate);
    }
}

// ========== Utility Functions ==========

int getValidInt(const char* message, int min, int max) {
    int value, valid;
    char ch;
    do {
        printf("%s", message);
        valid = scanf("%d", &value);
        while((ch = getchar()) != '\n' && ch != EOF); // clear buffer
        if(valid != 1 || value < min || value > max)
            printf("Invalid input! Enter a number between %d and %d.\n", min, max);
    } while(valid != 1 || value < min || value > max);
    return value;
}

