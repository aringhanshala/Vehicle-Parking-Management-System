// Vehicle Parking Management System: Handles check-in, check-out, vehicle search and parking history with time stamping and vehicle type based charging.


// Preprocessors
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define max_slots 20


// Slot tracking array: 0= available, 1= occupied
int slots[max_slots] = {0};     // Global variable


// Declaration of the structure
struct Vehicle {
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
void loadSlotStatus();
int findFreeSlot();
void searchVehicle();
int isUpperLetter(char ch);
int isDigit(char ch);
int isValidPlate(char plate[]);
int isValidVehicleType(char type[]);


// Main function
void main() {
    int choice;     // Variable declaration
    loadSlotStatus();  // Load slot status from file

    // Heading
    printf("\n");
    printf("**************************************\n");
    printf("* Vehicle Parking Management System  *\n");
    printf("**************************************\n");

    // Main menu
    do {
        printf("\n");
        printf("What do you want to do?:\n");
        printf("1. Vehicle Check-in\n");
        printf("2. Vehicle Check-out\n");
        printf("3. View Parking History\n");
        printf("4. Search Vehicle\n");
        printf("5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        // User's choice will call the respective function
        switch (choice) {
            case 1:
                checkIn();     // Calling checkIn function
                break;
            case 2:
                checkOut();     // Calling checkOut function
                break;
            case 3:
                viewHistory();     // Calling viewHistory funciton
                break;
            case 4:
                searchVehicle();     // Calling searchVehicle function
                break;
            case 5:
                printf("\nExited from the System\n");     // Exit from the system
                break;
            default:
                printf("Invalid choice. Please try again.\n");     // If the user input invalid choice
        }

        // Pause after each action
        if (choice != 5) {
            printf("\nPress Enter to continue...");
            getchar(); // consume newline from scanf
            getchar(); // wait for user to press Enter
        }

    } while (choice != 5);
}


// Function to get current time
void getCurrentTime(char *buffer) {
    time_t t;
    struct tm *tm_info;
    t= time(NULL);
    tm_info= localtime(&t);
    strftime(buffer, 30,"%Y-%m-%d_%H:%M:%S", tm_info);
}


// Load slot availability based on parking.txt
void loadSlotStatus() {
    FILE *fp= fopen("parking.txt", "r");     // Opening text file
    struct Vehicle v;

    if(!fp) return;

    // Reset slots to 0 before loading
    for(int i= 0; i< max_slots; i++){
        slots[i]= 0;
    }

    while(fscanf(fp,"%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF){
        if(strcmp(v.checkOutTime, "N/A") == 0){     // Checking weather the person checkout or not
            if(v.slotNumber>= 1 && v.slotNumber<= max_slots){
                slots[v.slotNumber - 1]= 1;     // Changing the status of slot to occupied
            }
        }
    }

    fclose(fp);     // Closing text file
}


// Find first available slot
int findFreeSlot(){
    for(int i= 0; i< max_slots; i++){
        if(slots[i] == 0)
            return i + 1;     // returning the first available slot
    }
    return -1;     // if no parking slot available it will return a index that not belongs to the array
}


// Helper function to check if a character is uppercase letter
int isUpperLetter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}


// Helper function to check if a character is digit
int isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}


// Plate number validation
int isValidPlate(char plate[]) {
    int len = strlen(plate);

    if (len == 9) {
        return isUpperLetter(plate[0]) &&
               isUpperLetter(plate[1]) &&
               isDigit(plate[2]) &&
               isDigit(plate[3]) &&
               isUpperLetter(plate[4]) &&
               isDigit(plate[5]) &&
               isDigit(plate[6]) &&
               isDigit(plate[7]) &&
               isDigit(plate[8]);
    } else if (len == 10) {
        return isUpperLetter(plate[0]) &&
               isUpperLetter(plate[1]) &&
               isDigit(plate[2]) &&
               isDigit(plate[3]) &&
               isUpperLetter(plate[4]) &&
               isUpperLetter(plate[5]) &&
               isDigit(plate[6]) &&
               isDigit(plate[7]) &&
               isDigit(plate[8]) &&
               isDigit(plate[9]);
    }

    return 0;
}


// Vehicle type validation
int isValidVehicleType(char type[]) {
    return strcmp(type, "car") == 0 ||
           strcmp(type, "bike") == 0 ||
           strcmp(type, "scooty") == 0;
}


// Vehicle check-in function
void checkIn() {
    FILE *fp = fopen("parking.txt", "a");     // Opening text file

    if (!fp) {
        printf("Error opening file for check-in.\n");
        return;
    }

    struct Vehicle v;
    int freeSlot = findFreeSlot();

    if (freeSlot == -1) {
        printf("\nNo parking slots available right now.\n");
        fclose(fp);
        return;
    }

    // Validate plate number
    while (1) {
        printf("\nEnter Plate Number: ");
        scanf("%s", v.plateNumber);

        if (isValidPlate(v.plateNumber)) {
            break;
        } else {
            printf("\nInvalid plate number format. Please re-enter\n");
        }
    }

    // Validate vehicle type
    while (1) {
        printf("Enter Vehicle Type (car/bike/scooty): ");
        scanf("%s", v.vehicleType);

        if (isValidVehicleType(v.vehicleType)) {
            break;
        } else {
            printf("Invalid vehicle type. Please re-enter.\n");
        }
    }

    v.slotNumber = freeSlot;
    slots[freeSlot - 1] = 1;
    getCurrentTime(v.checkInTime);
    strcpy(v.checkOutTime, "N/A");
    v.charge = 0.0;

    fprintf(fp, "%s %s %d %s %s %.2f\n",
            v.plateNumber, v.vehicleType, v.slotNumber,
            v.checkInTime, v.checkOutTime, v.charge);

    fclose(fp);     // Closing text file

    printf("\nCheck-in successful. Assigned Slot Number: %d\n", v.slotNumber);
}


// Vehicle check-out function
void checkOut() {
    FILE *fp= fopen("parking.txt","r");     // Opening text file
    FILE *temp= fopen("temp.txt","w");     // Opening temperory file

    if(!fp || !temp){
        printf("Error! on opening file\n");
        return;
    }

    struct Vehicle v;
    char plate[20];
    int found = 0;

    printf("Enter Plate Number for Check-out: ");
    scanf("%s", plate);

    // Searching in the entire file and making required changes
    while(fscanf(fp,"%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF){
        if (strcmp(v.plateNumber, plate) == 0 && strcmp(v.checkOutTime, "N/A") == 0){
            found= 1;
            getCurrentTime(v.checkOutTime);

            // Charge according to stored vehicle type
            if(strcmp(v.vehicleType, "car") == 0){
                v.charge = 30.0;
            } else if(strcmp(v.vehicleType, "bike") == 0 || strcmp(v.vehicleType, "scooty") == 0){
                v.charge = 20.0;
            } else{
                v.charge = 40.0;
            }

            slots[v.slotNumber - 1] = 0;    // Changing the status of slot to free

            printf("Parking charge: Rs. %.2f\n", v.charge);
        }

        fprintf(temp,"%s %s %d %s %s %.2f\n", v.plateNumber, v.vehicleType, v.slotNumber, v.checkInTime, v.checkOutTime, v.charge);
    }

    fclose(fp);     // Closing text file
    fclose(temp);     // Closing temporary file

    remove("parking.txt");     // Removing the parking.txt file
    rename("temp.txt","parking.txt");     // Renaming temp.txt to parking.txt

    if(found) printf("Check-out successful\n");
    else printf("Vehicle not found or already checked out.\n");
}


// Function to view all parking records
void viewHistory(){
    FILE *fp= fopen("parking.txt", "r");     // Opening text file
    struct Vehicle v;

    if(!fp){
        printf("No parking records found\n");
        return;
    }

    printf("\n--- Parking History ---\n\n");

    // Printing the entire information of vehicles from the text file
    while(fscanf(fp,"%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF){
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


// Function to search by plate number
void searchVehicle() {
    FILE *fp= fopen("parking.txt","r");     // Opening text file

    struct Vehicle v;
    char plate[20];
    int found = 0;

    if(!fp){
        printf("Error opening parking records.\n");
        return;
    }

    printf("Enter Plate Number to search: ");
    scanf("%s", plate);

    // Printing the entire information of the vehicle from the text file
    while(fscanf(fp,"%s %s %d %s %s %f", v.plateNumber, v.vehicleType, &v.slotNumber, v.checkInTime, v.checkOutTime, &v.charge) != EOF){
        if(strcmp(v.plateNumber, plate) == 0){
            found = 1;
            printf("\n--- Vehicle Details ---\n\n");
            printf("Plate number : %s\n", v.plateNumber);
            printf("Vehicle type : %s\n", v.vehicleType);
            printf("Slot number  : %d\n", v.slotNumber);
            printf("Check-in     : %s\n", v.checkInTime);
            printf("Check-out    : %s\n", v.checkOutTime);
            printf("Charge       : Rs. %.2f\n", v.charge);
            printf("-----------------------------------\n");
        }
    }

    fclose(fp);     // Closing text file

    if(!found) printf("No record found for plate number: %s\n", plate);
}
