#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum SeatStatus {
    EMPTY,
    BOOKED
};

enum SeatPreference {
    WINDOW,
    AISLE
};

// Union for passenger information
union PassengerInfo {
    char passengerName[50];
    struct {
        char firstName[25];
        char lastName[25];
    };
    struct {
        char email[50];
        char phone[15];
    };
};

struct Seat {
    char seatIdentifier[10]; // Unique seat identifier
    enum SeatStatus status;
    enum SeatPreference preference; // Window or aisle
    union PassengerInfo passenger;
    float fare;
};

struct Bus {
    int busNumber;
    struct Seat economy[20];
    struct Seat business[10];
    struct Seat firstClass[10];
    float totalRevenue;
};

void generateSeatIdentifier(int busNumber, int classChoice, int seatNumber, char* seatIdentifier) {
    sprintf(seatIdentifier, "%d-%d-%d", busNumber, classChoice, seatNumber);
}

void displaySeats(struct Seat* seats, int numSeats) {
    printf("Seat Identifier\tStatus\tPassenger Name\tEmail\tPhone\tPreference\tFare\n");
    for (int i = 0; i < numSeats; i++) {
        printf("%s\t\t%s\t%s\t%s\t%s\t%s\t%.2f\n", seats[i].seatIdentifier,
               (seats[i].status == EMPTY) ? "Empty" : "Booked",
               seats[i].passenger.passengerName, seats[i].passenger.email,
               seats[i].passenger.phone,
               (seats[i].preference == WINDOW) ? "Window" : "Aisle",
               seats[i].fare);
    }
}

void bookSeat(struct Seat* seats, int numSeats, float fare) {
    int seatNumber;
    printf("Enter the seat number to book: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > numSeats) {
        printf("Invalid seat number.\n");
        return;
    }

    char seatIdentifier[10];
    generateSeatIdentifier(1, 1, seatNumber, seatIdentifier); // Assuming bus number 1 and class 1 (economy)

    if (seats[seatNumber - 1].status == EMPTY) {
        seats[seatNumber - 1].status = BOOKED;
        printf("Enter passenger name (or first name and last name): ");
        scanf("%s", seats[seatNumber - 1].passenger.firstName);

        // Assuming the input includes a space to separate first and last names
        if (strchr(seats[seatNumber - 1].passenger.firstName, ' ')) {
            sscanf(seats[seatNumber - 1].passenger.firstName, "%s %s", 
                   seats[seatNumber - 1].passenger.firstName, 
                   seats[seatNumber - 1].passenger.lastName);
        } else {
            strcpy(seats[seatNumber - 1].passenger.lastName, "");
        }

        printf("Enter passenger email: ");
        scanf("%s", seats[seatNumber - 1].passenger.email);
        
        printf("Enter passenger phone: ");
        scanf("%s", seats[seatNumber - 1].passenger.phone);

        printf("Enter seat preference (1: Window, 2: Aisle): ");
        int preferenceChoice;
        scanf("%d", &preferenceChoice);
        seats[seatNumber - 1].preference = (preferenceChoice == 1) ? WINDOW : AISLE;

        seats[seatNumber - 1].fare = fare;
        printf("Seat %s booked for %s %s.\n", seatIdentifier, 
               seats[seatNumber - 1].passenger.firstName, 
               seats[seatNumber - 1].passenger.lastName);
    } else {
        printf("Seat %s is already booked.\n", seatIdentifier);
    }
}

void cancelBooking(struct Seat* seats, int numSeats) {
    int seatNumber;
    printf("Enter the seat number to cancel booking: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > numSeats) {
        printf("Invalid seat number.\n");
        return;
    }

    char seatIdentifier[10];
    generateSeatIdentifier(1, 1, seatNumber, seatIdentifier); // Assuming bus number 1 and class 1 (economy)

    if (seats[seatNumber - 1].status == BOOKED) {
        float refund = seats[seatNumber - 1].fare * 0.8; // 80% refund
        seats[seatNumber - 1].status = EMPTY;
        strcpy(seats[seatNumber - 1].passenger.passengerName, "");
        strcpy(seats[seatNumber - 1].passenger.email, "");
        strcpy(seats[seatNumber - 1].passenger.phone, "");
        seats[seatNumber - 1].preference = WINDOW; // Reset preference to window
        seats[seatNumber - 1].fare = 0;
        printf("Booking for seat %s canceled. Refund amount: $%.2f\n", seatIdentifier, refund);
    } else {
        printf("Seat %s is not booked.\n", seatIdentifier);
    }
}

void saveSeatsToFile(struct Seat* seats, int numSeats, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(seats, sizeof(struct Seat), numSeats, file);
    fclose(file);
}

void loadSeatsFromFile(struct Seat* seats, int numSeats, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    fread(seats, sizeof(struct Seat), numSeats, file);
    fclose(file);
}

// Function to search for a passenger by name or seat number
void searchPassenger(struct Bus* bus, const char* query) {
    for (int i = 0; i < 20; i++) {
        if (strcmp(bus->economy[i].passenger.passengerName, query) == 0 ||
            strcmp(bus->economy[i].seatIdentifier, query) == 0) {
            printf("Found in Economy Class:\n");
            printf("Seat: %s\n", bus->economy[i].seatIdentifier);
            printf("Passenger: %s %s\n", bus->economy[i].passenger.firstName, bus->economy[i].passenger.lastName);
            printf("Email: %s\n", bus->economy[i].passenger.email);
            printf("Phone: %s\n", bus->economy[i].passenger.phone);
            printf("Preference: %s\n", (bus->economy[i].preference == WINDOW) ? "Window" : "Aisle");
            printf("Fare: %.2f\n", bus->economy[i].fare);
            return;
        }
    }

    // Search in Business Class and First Class in a similar manner
    // ...

    printf("Passenger not found.\n");
}

// Function to display bus statistics
void displayBusStatistics(struct Bus* bus) {
    int totalBookedEconomy = 0;
    int totalAvailableEconomy = 0;
    int totalBookedBusiness = 0;
    int totalAvailableBusiness = 0;
    int totalBookedFirstClass = 0;
    int totalAvailableFirstClass = 0;

    for (int i = 0; i < 20; i++) {
        if (bus->economy[i].status == BOOKED) {
            totalBookedEconomy++;
        } else {
            totalAvailableEconomy++;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (bus->business[i].status == BOOKED) {
            totalBookedBusiness++;
        } else {
            totalAvailableBusiness++;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (bus->firstClass[i].status == BOOKED) {
            totalBookedFirstClass++;
        } else {
            totalAvailableFirstClass++;
        }
    }

    printf("Bus %d Statistics:\n", bus->busNumber);
    printf("Economy Class: %d seats booked, %d seats available\n", totalBookedEconomy, totalAvailableEconomy);
    printf("Business Class: %d seats booked, %d seats available\n", totalBookedBusiness, totalAvailableBusiness);
    printf("First Class: %d seats booked, %d seats available\n", totalBookedFirstClass, totalAvailableFirstClass);

    int totalBooked = totalBookedEconomy + totalBookedBusiness + totalBookedFirstClass;
    int totalAvailable = totalAvailableEconomy + totalAvailableBusiness + totalAvailableFirstClass;

    float occupancy = (float)totalBooked / (totalBooked + totalAvailable) * 100;
    printf("Total Occupancy: %.2f%%\n", occupancy);
}

int main() {
    int numBuses = 5;
    struct Bus buses[numBuses];

    for (int i = 0; i < numBuses; i++) {
        buses[i].busNumber = i + 1;

        for (int j = 0; j < 20; j++) {
            generateSeatIdentifier(i + 1, 1, j + 1, buses[i].economy[j].seatIdentifier);
            buses[i].economy[j].status = EMPTY;
            buses[i].economy[j].preference = WINDOW; // Default preference to window
            strcpy(buses[i].economy[j].passenger.passengerName, "");
            strcpy(buses[i].economy[j].passenger.email, "");
            strcpy(buses[i].economy[j].passenger.phone, "");
            buses[i].economy[j].fare = 0.0;
        }

        for (int j = 0; j < 10; j++) {
            generateSeatIdentifier(i + 1, 2, j + 1, buses[i].business[j].seatIdentifier);
            buses[i].business[j].status = EMPTY;
            buses[i].business[j].preference = WINDOW; // Default preference to window
            strcpy(buses[i].business[j].passenger.passengerName, "");
            strcpy(buses[i].business[j].passenger.email, "");
            strcpy(buses[i].business[j].passenger.phone, "");
            buses[i].business[j].fare = 0.0;
        }

        for (int j = 0; j < 10; j++) {
            generateSeatIdentifier(i + 1, 3, j + 1, buses[i].firstClass[j].seatIdentifier);
            buses[i].firstClass[j].status = EMPTY;
            buses[i].firstClass[j].preference = WINDOW; // Default preference to window
            strcpy(buses[i].firstClass[j].passenger.passengerName, "");
            strcpy(buses[i].firstClass[j].passenger.email, "");
            strcpy(buses[i].firstClass[j].passenger.phone, "");
            buses[i].firstClass[j].fare = 0.0;
        }
    }

    int choice;
    const char* filename = "bus_seats.dat";

    do {
        printf("\nBus Reservation System\n");
        printf("1. Display Available Seats\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel Booking\n");
        printf("4. Save Data to File\n");
        printf("5. Load Data from File\n");
        printf("6. Search for Passenger\n");
        printf("7. Display Bus Statistics\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the bus number (1 to %d): ", numBuses);
                int busChoice;
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    printf("\nEconomy Class:\n");
                    displaySeats(buses[busChoice - 1].economy, 20);
                    printf("\nBusiness Class:\n");
                    displaySeats(buses[busChoice - 1].business, 10);
                    printf("\nFirst Class:\n");
                    displaySeats(buses[busChoice - 1].firstClass, 10);
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 2:
                printf("Enter the bus number (1 to %d): ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    int classChoice;
                    printf("Select class (1: Economy, 2: Business, 3: First Class): ");
                    scanf("%d", &classChoice);
                    if (classChoice >= 1 && classChoice <= 3) {
                        struct Seat* seats;
                        int numSeats;
                        float fare;
                        if (classChoice == 1) {
                            seats = buses[busChoice - 1].economy;
                            numSeats = 20;
                            fare = 50.0; // Set fare for economy class
                        } else if (classChoice == 2) {
                            seats = buses[busChoice - 1].business;
                            numSeats = 10;
                            fare = 75.0; // Set fare for business class
                        } else {
                            seats = buses[busChoice - 1].firstClass;
                            numSeats = 10;
                            fare = 100.0; // Set fare for first class
                        }
                        bookSeat(seats, numSeats, fare);
                        buses[busChoice - 1].totalRevenue += fare;
                    } else {
                        printf("Invalid class choice.\n");
                    }
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 3:
                printf("Enter the bus number (1 to %d) to cancel booking: ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    int classChoice;
                    printf("Select class (1: Economy, 2: Business, 3: First Class): ");
                    scanf("%d", &classChoice);
                    if (classChoice >= 1 && classChoice <= 3) {
                        struct Seat* seats;
                        int numSeats;
                        if (classChoice == 1) {
                            seats = buses[busChoice - 1].economy;
                            numSeats = 20;
                        } else if (classChoice == 2) {
                            seats = buses[busChoice - 1].business;
                            numSeats = 10;
                        } else {
                            seats = buses[busChoice - 1].firstClass;
                            numSeats = 10;
                        }
                        cancelBooking(seats, numSeats);
                    } else {
                        printf("Invalid class choice.\n");
                    }
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 4:
                printf("Enter the bus number (1 to %d) to save data: ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    printf("Select class (1: Economy, 2: Business, 3: First Class): ");
                    int classChoice;
                    scanf("%d", &classChoice);
                    if (classChoice >= 1 && classChoice <= 3) {
                        struct Seat* seats;
                        int numSeats;
                        if (classChoice == 1) {
                            seats = buses[busChoice - 1].economy;
                            numSeats = 20;
                        } else if (classChoice == 2) {
                            seats = buses[busChoice - 1].business;
                            numSeats = 10;
                        } else {
                            seats = buses[busChoice - 1].firstClass;
                            numSeats = 10;
                        }
                        saveSeatsToFile(seats, numSeats, filename);
                        printf("Data saved to file for Bus %d, Class %d.\n", busChoice, classChoice);
                    } else {
                        printf("Invalid class choice.\n");
                    }
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 5:
                printf("Enter the bus number (1 to %d) to load data: ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    printf("Select class (1: Economy, 2: Business, 3: First Class): ");
                    int classChoice;
                    scanf("%d", &classChoice);
                    if (classChoice >= 1 && classChoice <= 3) {
                        struct Seat* seats;
                        int numSeats;
                        if (classChoice == 1) {
                            seats = buses[busChoice - 1].economy;
                            numSeats = 20;
                        } else if (classChoice == 2) {
                            seats = buses[busChoice - 1].business;
                            numSeats = 10;
                        } else {
                            seats = buses[busChoice - 1].firstClass;
                            numSeats = 10;
                        }
                        loadSeatsFromFile(seats, numSeats, filename);
                        printf("Data loaded from file for Bus %d, Class %d.\n", busChoice, classChoice);
                    } else {
                        printf("Invalid class choice.\n");
                    }
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 6:
                printf("Enter the bus number (1 to %d) to search for a passenger: ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    char query[50];
                    printf("Enter passenger name or seat number to search: ");
                    scanf("%s", query);
                    searchPassenger(&buses[busChoice - 1], query);
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 7:
                printf("Enter the bus number (1 to %d) to display statistics: ", numBuses);
                scanf("%d", &busChoice);
                if (busChoice >= 1 && busChoice <= numBuses) {
                    displayBusStatistics(&buses[busChoice - 1]);
                } else {
                    printf("Invalid bus number.\n");
                }
                break;

            case 8:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    } while (choice != 8);

    // Display total revenue for each bus
    printf("\nTotal Revenue for Each Bus:\n");
    for (int i = 0; i < numBuses; i++) {
        printf("Bus %d: $%.2f\n", i + 1, buses[i].totalRevenue);
    }

    return 0;
}
