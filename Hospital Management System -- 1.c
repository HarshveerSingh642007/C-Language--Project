#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PATIENT_FILE "patients.txt"
#define DOCTOR_FILE  "doctors.txt"
#define APPOINT_FILE "appointments.txt"

typedef struct {
    int id;
    char name[50];
    int age;
    char gender;
    char disease[50];
} Patient;

typedef struct {
    int id;
    char name[50];
    char specialty[50];
} Doctor;

typedef struct {
    int id;
    int patientId;
    int doctorId;
    char date[15];
} Appointment;

// Dynamic arrays
Patient *patients = NULL;
Doctor *doctors = NULL;
Appointment *appointments = NULL;
int totalPatients = 0, totalDoctors = 0, totalAppointments = 0;

// Function declarations
int login();
void loadData();
void saveAllData();
void mainMenu();
void patientMenu();
void doctorMenu();
void appointmentMenu();
void addPatient();
void showPatients();
void findPatient();
void findPatientById();
void findPatientByDisease();
void addDoctor();
void showDoctors();
void findDoctor();
void findDoctorById();
void findDoctorBySpecialty();
void bookAppointment();
void showAppointments();
int getValidInt(const char* msg, int min, int max);
void clearInputBuffer();
void freeAllMemory();
void savePatients();
void saveDoctors();
void saveAppointments();
void loadPatients();
void loadDoctors();
void loadAppointments();
void printHeader();

int main() {
    printHeader();
    if(!login()) {
        printf("Access denied.\n");
        return 0;
    }
    loadData();
    mainMenu();
    saveAllData();
    freeAllMemory();
    return 0;
}

void printHeader() {
    printf("==================== HOSPITAL MANAGEMENT SYSTEM ====================\n");
    printf("Project: Complete, with file handling, dynamic arrays, comments, documentation\n");
    printf("\n");
}

// ----------- Authentication -----------

int login() {
    char password[20];
    printf("Enter admin password to access: ");
    scanf("%19s", password);
    return strcmp(password, "admin123") == 0;
}

// ----------- Data Persistence -----------

void loadData() {
    loadPatients();
    loadDoctors();
    loadAppointments();
}
void saveAllData() {
    savePatients();
    saveDoctors();
    saveAppointments();
}
void loadPatients() {
    FILE *f = fopen(PATIENT_FILE, "r");
    if(!f) return;
    Patient temp;
    while(fscanf(f, "%d %49s %d %c %49s", &temp.id, temp.name, &temp.age, &temp.gender, temp.disease) == 5) {
        patients = realloc(patients, sizeof(Patient) * (totalPatients+1));
        patients[totalPatients++] = temp;
    }
    fclose(f);
}
void savePatients() {
    FILE *f = fopen(PATIENT_FILE, "w");
    if(!f) return;
    for(int i=0; i < totalPatients; ++i)
        fprintf(f, "%d %s %d %c %s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].disease);
    fclose(f);
}
void loadDoctors() {
    FILE *f = fopen(DOCTOR_FILE, "r");
    if(!f) return;
    Doctor temp;
    while(fscanf(f, "%d %49s %49s", &temp.id, temp.name, temp.specialty) == 3) {
        doctors = realloc(doctors, sizeof(Doctor) * (totalDoctors+1));
        doctors[totalDoctors++] = temp;
    }
    fclose(f);
}
void saveDoctors() {
    FILE *f = fopen(DOCTOR_FILE, "w");
    if(!f) return;
    for(int i=0; i < totalDoctors; ++i)
        fprintf(f, "%d %s %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
    fclose(f);
}
void loadAppointments() {
    FILE *f = fopen(APPOINT_FILE, "r");
    if(!f) return;
    Appointment temp;
    while(fscanf(f, "%d %d %d %14s", &temp.id, &temp.patientId, &temp.doctorId, temp.date) == 4) {
        appointments = realloc(appointments, sizeof(Appointment) * (totalAppointments+1));
        appointments[totalAppointments++] = temp;
    }
    fclose(f);
}
void saveAppointments() {
    FILE *f = fopen(APPOINT_FILE, "w");
    if(!f) return;
    for(int i=0; i < totalAppointments; ++i)
        fprintf(f, "%d %d %d %s\n", appointments[i].id, appointments[i].patientId, appointments[i].doctorId, appointments[i].date);
    fclose(f);
}
void freeAllMemory() {
    free(patients);
    free(doctors);
    free(appointments);
}

// ----------- Menus -----------

void mainMenu() {
    int choice;
    do {
        printf("\n------ Main Menu ------\n");
        printf("1. Patient Menu\n");
        printf("2. Doctor Menu\n");
        printf("3. Appointment Menu\n");
        printf("4. Show All Patients\n");
        printf("5. Show All Doctors\n");
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
        printf("6. Back\n");
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
        printf("5. Find Doctor by Specialty\n");
        printf("6. Back\n");
        choice = getValidInt("Enter choice: ", 1, 6);
        if(choice == 1) addDoctor();
        else if(choice == 2) showDoctors();
        else if(choice == 3) findDoctor();
        else if(choice == 4) findDoctorById();
        else if(choice == 5) findDoctorBySpecialty();
    } while(choice != 6);
}

void appointmentMenu() {
    int choice;
    do {
        printf("\n--- Appointment Menu ---\n");
        printf("1. Book Appointment\n");
        printf("2. Show All Appointments\n");
        printf("3. Back\n");
        choice = getValidInt("Enter choice: ", 1, 3);
        if(choice == 1) bookAppointment();
        else if(choice == 2) showAppointments();
    } while(choice != 3);
}

// ----------- Patient Features -----------

void addPatient() {
    Patient temp;
    temp.id = totalPatients ? patients[totalPatients-1].id + 1 : 1;
    printf("Enter name: ");
    scanf(" %[^\n]", temp.name);

    temp.age = getValidInt("Enter age: ", 1, 120);

    do {
        printf("Enter gender (M/F): ");
        scanf(" %c", &temp.gender);
        temp.gender = toupper(temp.gender);
    } while(temp.gender != 'M' && temp.gender != 'F');

    printf("Enter disease: ");
    scanf(" %[^\n]", temp.disease);

    patients = realloc(patients, sizeof(Patient) * (totalPatients+1));
    patients[totalPatients++] = temp;
    savePatients();
    printf("Patient added.\n");
}

void showPatients() {
    printf("\nID\tName\tAge\tGender\tDisease\n");
    for(int i=0; i < totalPatients; ++i)
        printf("%d\t%s\t%d\t%c\t%s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].disease);
}

void findPatient() {
    char name[50];
    printf("Enter name to search: ");
    scanf(" %[^\n]", name);
    int found = 0;
    for(int i=0;i<totalPatients;++i)
        if(strcasecmp(patients[i].name, name) == 0) {
            printf("--\nID %d, Name: %s, Age: %d, Gender: %c, Disease: %s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].disease);
            found=1;
        }
    if(!found) printf("No patient found.\n");
}

void findPatientById() {
    int id = getValidInt("Enter patient ID: ", 1, patients ? patients[totalPatients-1].id : 1);
    int found = 0;
    for(int i=0;i<totalPatients;++i)
        if(patients[i].id == id) {
            printf("--\nID %d, Name: %s, Age: %d, Gender: %c, Disease: %s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].disease);
            found=1;
        }
    if(!found) printf("No patient found.\n");
}

void findPatientByDisease() {
    char disease[50];
    printf("Enter disease to search: ");
    scanf(" %[^\n]", disease);
    int found = 0;
    for(int i=0;i<totalPatients;++i)
        if(strcasecmp(patients[i].disease, disease) == 0) {
            printf("--\nID %d, Name: %s, Age: %d, Gender: %c, Disease: %s\n", patients[i].id, patients[i].name, patients[i].age, patients[i].gender, patients[i].disease);
            found=1;
        }
    if(!found) printf("No patient found.\n");
}

// ----------- Doctor Features -----------

void addDoctor() {
    Doctor temp;
    temp.id = totalDoctors ? doctors[totalDoctors-1].id + 1 : 1;
    printf("Enter name: ");
    scanf(" %[^\n]", temp.name);
    printf("Enter specialty: ");
    scanf(" %[^\n]", temp.specialty);

    doctors = realloc(doctors, sizeof(Doctor)*(totalDoctors+1));
    doctors[totalDoctors++] = temp;
    saveDoctors();
    printf("Doctor added.\n");
}

void showDoctors() {
    printf("\nID\tName\tSpecialty\n");
    for(int i=0; i<totalDoctors; ++i)
        printf("%d\t%s\t%s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
}

void findDoctor() {
    char name[50];
    printf("Enter name to search: ");
    scanf(" %[^\n]", name);
    int found = 0;
    for(int i=0;i<totalDoctors;++i)
        if(strcasecmp(doctors[i].name, name) == 0) {
            printf("--\nID %d, Name: %s, Specialty: %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
            found=1;
        }
    if(!found) printf("No doctor found.\n");
}

void findDoctorById() {
    int id = getValidInt("Enter doctor ID: ", 1, doctors ? doctors[totalDoctors-1].id : 1);
    int found = 0;
    for(int i=0;i<totalDoctors;++i)
        if(doctors[i].id == id) {
            printf("--\nID %d, Name: %s, Specialty: %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
            found=1;
        }
    if(!found) printf("No doctor found.\n");
}

void findDoctorBySpecialty() {
    char specialty[50];
    printf("Enter specialty to search: ");
    scanf(" %[^\n]", specialty);
    int found=0;
    for(int i=0;i<totalDoctors;++i)
        if(strcasecmp(doctors[i].specialty, specialty) == 0){
            printf("--\nID %d, Name: %s, Specialty: %s\n", doctors[i].id, doctors[i].name, doctors[i].specialty);
            found=1;
        }
    if(!found) printf("No doctor found.\n");
}

// ----------- Appointment Features -----------

void bookAppointment() {
    Appointment temp;
    temp.id = totalAppointments ? appointments[totalAppointments-1].id + 1 : 1;
    temp.patientId = getValidInt("Enter valid patient ID: ", 1, totalPatients ? patients[totalPatients-1].id : 1);
    temp.doctorId = getValidInt("Enter valid doctor ID: ", 1, totalDoctors ? doctors[totalDoctors-1].id : 1);
    printf("Enter date (dd/mm/yyyy): ");
    scanf(" %[^\n]", temp.date);

    appointments = realloc(appointments, sizeof(Appointment)*(totalAppointments+1));
    appointments[totalAppointments++] = temp;
    saveAppointments();
    printf("Appointment booked.\n");
}

void showAppointments() {
    printf("\nID\tPatientID\tDoctorID\tDate\n");
    for(int i=0; i<totalAppointments; ++i)
        printf("%d\t%d\t\t%d\t\t%s\n", appointments[i].id, appointments[i].patientId, appointments[i].doctorId, appointments[i].date);
}

// ----------- Utilities -----------

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
