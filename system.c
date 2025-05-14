// Vehicle Parking Management System: Handles check-in, check-out and parking history with time-stamping and rate billing.


// Preprocessors
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>


// Declaration of the structure
struct Vehicle{
    char plateNumber[20];
    char vehicleType[10];
    char checkInTime[30];
    char checkOutTime[30];
    int slotNumber;
    float charge;
};


// Functions declaration
void getCurrentTime(char *buffer);
void checkIn();
void checkOut();
void viewHistory();


// Main program
void main() {
    int choice;
    
    // Main Console
    printf("\n");
    printf("**************************************\n");
    printf("* Vechicle Parking Management System *\n");
    printf("**************************************\n");

    do {
        printf("\nWhat do you want to do?:\n");
        printf("1. Vehicle Check-in\n");
        printf("2. Vehicle Check-out\n");
        printf("3. View Parking History\n");
        printf("4. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: checkIn(); break;
            case 2: checkOut(); break;
            case 3: viewHistory(); break;
            case 4: printf("\nExited from the System\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 4);
}


// Function to get current time
void getCurrentTime(char *buffer) {
    time_t t;
    struct tm *tm_info;
    t = time(NULL);
    tm_info = localtime(&t);
    strftime(buffer, 30, "%Y-%m-%d_%H:%M:%S", tm_info);
}


// Vehicle check-in function
void checkIn() {
    FILE *fp = fopen("parking.txt", "a");     // Opening text file

    if (!fp) {
        printf("Error opening file for check-in.\n");
        return;
    }

    struct Vehicle v;

    // Reading the vehicle information
    printf("\nEnter Plate Number: ");
    scanf("%s", v.plateNumber);
    printf("Enter Vehicle Type (car/bike): ");
    scanf("%s", v.vehicleType);
    printf("Enter Slot Number: ");
    scanf("%d", &v.slotNumber);

    getCurrentTime(v.checkInTime);     // Automatically note the current time
    strcpy(v.checkOutTime, "N/A");
    v.charge = 0.0;

    // Inserting the data in the text file
    fprintf(fp, "%s %s %d %s %s %.2f\n", v.plateNumber, v.vehicleType, v.slotNumber, v.checkInTime, v.checkOutTime, v.charge);

    fclose(fp);     // Closing text file

    printf("\n");
    printf("Check-in successful\n");
}


// Vehicle check-out function
void checkOut() {
    FILE *fp = fopen("parking.txt", "r");     // Opening text file
    FILE *temp = fopen("temp.txt", "w");     // Opening temporary text file

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    // Variable declaration
    struct Vehicle v;
    char plate[20];
    int found = 0;

    printf("Enter Plate Number for Check-out: ");
    scanf("%s", plate);

    // Searching in the entire file and making required changes
    while (fscanf(fp, "%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF) {
        if (strcmp(v.plateNumber, plate) == 0 && strcmp(v.checkOutTime, "N/A") == 0) {
            found = 1;
            getCurrentTime(v.checkOutTime);
            v.charge = 50.0; // Flat charge
            printf("Parking charge: Rs. %.2f\n", v.charge);
        }

        // Updating the data in the text file
        fprintf(temp, "%s %s %d %s %s %.2f\n", v.plateNumber, v.vehicleType, v.slotNumber, v.checkInTime, v.checkOutTime, v.charge);
    }

    fclose(fp);     // Closing text file
    fclose(temp);     // Closing temporary text file

    remove("parking.txt");
    rename("temp.txt", "parking.txt");     // Renaming temp.txt to parking.txt

    if (found)
        printf("Check-out successful\n");
    else
        printf("Vehicle not found or already checked out.\n");
}


// Function to view all parking records
void viewHistory() {
    FILE *fp = fopen("parking.txt", "r");     // Opening text file

    struct Vehicle v;

    if (!fp) {
        printf("No parking records found\n");
        return;
    }

    printf("\n--- Parking History ---\n\n");

    // Printing the entire file content
    while (fscanf(fp, "%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF) {
        printf("Plate number : %s\n", v.plateNumber);
        printf("Vehicle type : %s\n", v.vehicleType);
        printf("Slot number  : %d\n", v.slotNumber);
        printf("Check-in     : %s\n", v.checkInTime);
        printf("Check-out    : %s\n", v.checkOutTime);
        printf("Charge       : Rs. %.2f\n", v.charge);
        printf("-----------------------------------\n");
    }

    fclose(fp);     // Closing text file
}