// CS Airline
// cs_airline.c
//
// This program was written on 10 April 2022
//
// Version 1.0.0 2022-04-02: Initial Release.
// Version 1.0.1 2022-04-06: Fixed style issue with help function.
//
// Program to manage a booking system for flights.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// The buffer length is used for reading a single line
#define MAX_STRING_LEN 100

#define COMMAND_PRINT_HELP "help"
#define COMMAND_PRINT_ROUTE "print"
#define COMMAND_SUBROUTE "subroute"
#define COMMAND_CHANGE_ORIGIN "change_origin"
#define COMMAND_BYPASS "bypass"
#define COMMAND_EMERGENCY "emergency"
#define COMMAND_CANCEL "cancel"
#define COMMAND_REVERSE "reverse"
#define COMMAND_ADD_PERSON "add_person"
#define COMMAND_PRINT_MANIFEST "print_manifest"
#define COMMAND_FLIGHT_STATS "stats"

#define HOUR 60
#define TRUE 1
#define FALSE 0

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Airports represent 'stops' along the flight path
// These are the 'nodes' of the linked list
struct airport {
    char code[MAX_STRING_LEN];
    int arrival_time;
    int departure_time;
    struct airport *next_airport;
};

// Root flight structure
// This stores the state of the current flight
// (i.e. the head and tail of the linked list)
struct flight {
    struct airport *origin_airport;
    struct airport *dest_airport;
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Functions
void remove_newline(char input[MAX_STRING_LEN]);
void do_print_help(void);

void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
);

// Stage 1 Functions
struct flight *setup_flight(void);
void do_print_airports(struct flight *flight);
void print_one_airport(struct airport *ap);
struct airport *create_airport(
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure
);

// Stage 2 Functions
void do_subroute(struct flight *flight);
void do_change_origin(struct flight *flight);

struct airport *insert_end(struct airport *head, struct airport *tail, 
char code[MAX_STRING_LEN], int arrival_time, int departure_time);

void do_bypass(struct flight *flight);

void do_emergency(struct flight *flight);

struct flight *do_cancel(struct flight *flight);

void do_reverse(struct flight *flight);

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  //////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {

    struct flight *flight = setup_flight();

    printf("Enter Command: ");
    char command_line[MAX_STRING_LEN];
    while (fgets(command_line, MAX_STRING_LEN, stdin) != NULL) {

        remove_newline(command_line);

        if (strcmp(COMMAND_PRINT_HELP, command_line) == 0) {
            do_print_help();
        } else if (strcmp(COMMAND_PRINT_ROUTE, command_line) == 0) {
            do_print_airports(flight);
        } else if (strcmp(COMMAND_SUBROUTE, command_line) == 0) {
            do_subroute(flight);
        } else if (strcmp(COMMAND_CHANGE_ORIGIN, command_line) == 0) {
            do_change_origin(flight);
        } else if (strcmp(COMMAND_BYPASS, command_line) == 0) {
            do_bypass(flight);
        } else if (strcmp(COMMAND_EMERGENCY, command_line) == 0) {
            do_emergency(flight);
        } else if (strcmp(COMMAND_CANCEL, command_line) == 0) {
            flight = do_cancel(flight);
        } else if (strcmp(COMMAND_REVERSE, command_line) == 0) {
            do_reverse(flight);
        }
        
            
        printf("Enter Command: ");
    }

    printf("Goodbye!\n");

    return 0;
}

// Function for reversing the flight airport order
void do_reverse(struct flight *flight) {
    
    struct airport *current = flight->origin_airport;
    current = flight->dest_airport;
    
    while (current->next_airport != flight->dest_airport) {
        current = current->next_airport;
    }
    flight->origin_airport->next_airport = current;
    flight->dest_airport = flight->origin_airport;
}

// Function for cancelling a flight and inputting a new flight
struct flight *do_cancel(struct flight *flight) {
    
    free(flight);
    printf("Flight cancelled. Now accepting a new flight:\n");
    struct flight *new_flight = setup_flight();
    
    return new_flight;
}

// Function for emergency plane landing command
void do_emergency(struct flight *flight) {
    
    printf("How long until emergency: ");
    
    char input_line[MAX_STRING_LEN] = {0};
    fgets(input_line, MAX_STRING_LEN, stdin);
    int num_hours = atoi(input_line);
    num_hours = num_hours * 100;
    
    int arrival_time = flight->origin_airport->departure_time + num_hours;
    int departure_time = 0000;
    char emergency_code[MAX_STRING_LEN];
    strcpy(emergency_code, "ATL0");
    struct airport *emergency_airport = create_airport(emergency_code, 
    arrival_time, departure_time);
    
    struct airport *current = flight->origin_airport;
    
    if (num_hours == 0) {
        flight->origin_airport->departure_time = 0000;
        while (current->next_airport != NULL) {
            struct airport *delete = current;
            delete = delete->next_airport;
            free(delete);
            current = current->next_airport->next_airport;
        }
    } else if (flight->dest_airport->arrival_time <= arrival_time) {
        printf("Flight was safe!\n");
    } else {
        
        
        emergency_airport->next_airport = NULL;
        
        flight->dest_airport = emergency_airport;
        
        while (current->next_airport != NULL) {
            if (current->next_airport->arrival_time > arrival_time) {
                struct airport *new = current->next_airport->next_airport;
                free(current->next_airport);
                current = new;
            } else {
                current = current->next_airport;
            }
        }
    }
}
// Function for bypass command to skip an airport on the flight route
void do_bypass(struct flight *flight) {
    
    char code [MAX_STRING_LEN];
    int exists = FALSE;
    
    printf("Enter airport code: ");
    fgets(code, MAX_STRING_LEN, stdin);
    remove_newline(code);
    
    struct airport *current = flight->origin_airport;
    
    if (strcmp(code, current->code) == 0) {
        free(current->next_airport);
    } else {
        while (current->next_airport != NULL) {
            if (strcmp(code, current->next_airport->code) == 0) {
                struct airport *new_next = current->next_airport->next_airport;
                free(current->next_airport);
                current->next_airport = new_next;
                exists = TRUE;
            }
            current = current->next_airport;
        }
        if (exists == FALSE) {
            printf("No airport of that code exists!\n");
        }
    }
}

// Stage 1.1
// Creates and initialises a flight by asking the user for input.
// Parameters:
//     None
// Returns:
//     Pointer to the malloc'd flight
struct flight *setup_flight(void) {

    // Create a new, empty flight
    struct flight *new_flight = malloc(sizeof(struct flight));
    new_flight->origin_airport = NULL;
    new_flight->dest_airport = NULL;

    // Read number of stops using fgets
    printf("How many stops will be enroute? ");
    char input_line[MAX_STRING_LEN] = {0};
    fgets(input_line, MAX_STRING_LEN, stdin);

    // Convert the string to a number
    int num_stops = 0;
    num_stops = atoi(input_line);

    // Use fgets to read a line and then use the provided interpret_line() 
    // function to extract information for each airport
    char buffer [MAX_STRING_LEN];
    int arrival_time = 0;
    int departure_time = 0;
    char airport_code [MAX_STRING_LEN];
    
    int count = 1;
    
    fgets(buffer, MAX_STRING_LEN, stdin);
    interpret_line(buffer, &arrival_time, &departure_time, airport_code);
    if (departure_time < arrival_time) {
        printf("Departure time cannot be before the arrival time!");
    } 
    new_flight->origin_airport = create_airport(airport_code, arrival_time, 
    departure_time);
    
    struct airport *current = new_flight->origin_airport;
    
    // For each airport, check conditions and add it to 
    // the end of the flight route
    while (count < num_stops) {
        count++;
        fgets(buffer, MAX_STRING_LEN, stdin);
        interpret_line(buffer, &arrival_time, &departure_time, airport_code);
        if (departure_time < arrival_time) {
            printf("Departure time cannot be before the arrival time!\n");
        } else if (new_flight->origin_airport->departure_time > arrival_time) {
            printf("New arrival time cannot be before ");
            printf("the previous departure time\n");
        } else {
            current = insert_end(new_flight->origin_airport, 
            new_flight->dest_airport, airport_code, arrival_time, departure_time);
            while (current != NULL) {
                if (current->next_airport == NULL) {
                    new_flight->dest_airport = current;
                }
                current = current->next_airport;
            }
        }
    }

    return new_flight;
}

// Function to append an airport to the end of a flight
struct airport *insert_end(
    struct airport *head, 
    struct airport *tail, 
    char code[MAX_STRING_LEN], 
    int arrival_time, 
    int departure_time) {
    
    struct airport *current = head;
    
    while (current->next_airport != NULL) {
        current = current->next_airport;
    }

    struct airport *new = create_airport(code, arrival_time, departure_time);
    current->next_airport = new;
    tail = current->next_airport;
    return head;
}
// Stage 1.2
// Given a pointer to a flight struct, prints all the info about the route
// Parameters:
//     flight = the flight which contains the flight route to print
// Returns:
//     None
void do_print_airports(struct flight *flight) {

    // Loop through the flight route and call the 
    // provided print_one_airport function
    printf("ROUTE:\n");
    struct airport *current = flight->origin_airport;
    while (current != NULL) {
        print_one_airport(current);
        current = current->next_airport;
    }
}

// Stage 1.2
// PROVIDED FUNCTION - DO NOT CHANGE
// Given a pointer to an airport struct,
// prints all the info about the airport
// Parameters:
//     ap = the airport to print
// Returns:
//     None
void print_one_airport(struct airport *ap) {
    printf(
        "%8s:    %04d -> %04d\n",
       ap->code,
       ap->arrival_time,
       ap->departure_time
    );
}

// Function to determine the subroute time from the origin to a given airport
void do_subroute(struct flight *flight) {
    
    char code [MAX_STRING_LEN];
    int start = flight->origin_airport->departure_time;
    int end = 0;
    
    printf("Enter airport code: ");
    fgets(code, MAX_STRING_LEN, stdin);
    remove_newline(code);
    
    struct airport *current = flight->origin_airport;
    
    while (current != NULL) {
        if (strcmp(current->code, code) == 0) {
            end = current->arrival_time;  
        }
        current = current->next_airport;
    }
    
    if (end == 0) {
        printf("Desired subroute destination does not exist!\n");
    } else {
        int hours = (end / 100) - (start / 100);
        int mins = (end % 100) - (start % 100);
        int minute_diff = hours * HOUR + mins;
        if (minute_diff < 0) {
            minute_diff *= -1;
        }
        hours = minute_diff / HOUR;
        mins = minute_diff % HOUR;
        printf("Time for subroute: %d hrs & %d mins\n", hours, mins);
    }
}

// Function to change the origin airport location
void do_change_origin(struct flight *flight) {
    
    char buffer [MAX_STRING_LEN];
    int arrival_time = 0;
    int departure_time = 0;
    char airport_code [MAX_STRING_LEN];
    
    int unique = TRUE;
    
    printf("Enter new origin info: ");
    fgets(buffer, MAX_STRING_LEN, stdin);
    interpret_line(buffer, &arrival_time, &departure_time, airport_code);
    
    struct airport *current = flight->origin_airport;
    
    if (arrival_time > departure_time) {
        printf("Departure time cannot be before the arrival time!\n");
    } else if (flight->origin_airport->arrival_time < departure_time) {
        printf("Departure of new origin cannot be after the arrival time ");
        printf("of the next airport!\n");
    } else {
        while (current != NULL) {
            if (strcmp(current->code, airport_code) == 0) {
                printf("New airport code is not unique!\n");
                unique = FALSE;
            }
            current = current->next_airport;
        }
        if (unique == TRUE) {
            struct airport *new_origin = create_airport(airport_code, arrival_time, 
            departure_time); 
            new_origin->next_airport = flight->origin_airport;
            flight->origin_airport = new_origin;
            printf("Added: %s\n", airport_code);
        }
    } 
}

// Stage 1.1
// Given the information about a new airport,
// Uses `malloc` to create memory for it and returns a pointer to
// that memory.
// Parameters:
//     code            = the airport ICAO code
//     arrival_time    = the arrival time of the new airport
//     departure_time  = the departure time of the new airport
// Returns:
//     A pointer to the malloc'd struct airport
struct airport *create_airport(
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure_time)
{
    // Malloc new struct
    struct airport *new_airport = malloc(sizeof(struct airport));

    // initialise airport fields
    strcpy(new_airport->code, code);
    new_airport->arrival_time = arrival_time;
    new_airport->departure_time = departure_time;
    new_airport->next_airport = NULL;

    return new_airport;
} 
    

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  HELPER FUNCTIONS  //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will remove and first newline it sees.
// The newline character wil be replaced with a null terminator ('\0')
// Parameters:
//     flight  = the flight to move people along of
// Returns:
//     None
void remove_newline(char input[MAX_STRING_LEN]) {

    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}


// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the following foramt: [integer0] [integer1] [string]
// This function will extract the relevant values into the given pointer variables.
// The function will also remove any newline characters.
//
// For example, if given: "0135 0545 YSSY"
// The function will put the integer values
//     135 into the val1 pointer
//     545 into the val2 pointer
// And will copy a null terminated string
//     "YSSY" into the 'word' array
//
// If you are interested, `strtok` is a function which takes a string,
// and splits it up into before and after a "token" (the second argument)
//
// Parameters:
//     buffer  = A null terminated string in the following format
//               [integer0] [integer1] [string]
//     val0    = A pointer to where [integer0] should be stored
//     val1    = A pointer to where [integer1] should be stored
//     word    = An array for the [string] to be copied into
// Returns:
//     None
void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
) {

    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as int
    char *val0_str = strtok(buffer, " ");
    if (val0_str != NULL) {
        *val0 = atoi(val0_str);
    } else {
        *val0 = -1;
    }

    // Extract value 2 as int
    char *val1_str = strtok(NULL, " ");
    if (val1_str != NULL) {
        *val1 = atoi(val1_str);
    } else {
        *val1 = -1;
    }

    char *word_str = strtok(NULL, " ");
    if (word_str != NULL) {
        // Extract word
        strcpy(word, word_str);
    }

    if (val0_str == NULL || val1_str == NULL || word_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }

}

void do_print_help(void) {
    printf("+-----------------+-------------------------------------------+\n");
    printf("|Command Name     | How to Use                                |\n");
    printf("+=================+===========================================+\n");
    printf("|PRINT ROUTE      | Enter command: print                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|SUBROUTE         | Enter command: subroute                   |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CHANGE ORIGIN    | Enter command: change_origin              |\n");
    printf("|                 | Enter new origin info: [ARRIVAL TIME]...  |\n");
    printf("|                 |     ... [DEPARTURE TIME] [AIRPORT CODE]   |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|BYPASS           | Enter command: bypass                     |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|EMERGENCY        | Enter command: emergency                  |\n");
    printf("|                 | How long until emergency: [TIME IN HOURS] |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CANCEL           | Enter command: cancel                     |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|REVERSE          | Enter command: reverse                    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|                 | Enter command: add_person                 |\n");
    printf("|ADD PERSON       | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("|                 | Enter the passenger name: [NAME]          |\n");
    printf("|                 | Enter the passenger weight: [WEIGHT]      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|PRINT MANIFEST   | Enter command: print_manifest             |\n");
    printf("|                 | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|FLIGHT STATISTICS| Enter command: stats                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
}