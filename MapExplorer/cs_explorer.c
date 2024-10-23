// CS Explorer
// cs_explorer.c
//
// This program was written on 10 March 2022
//
// Version 1.0.0 (2022-03-08): Assignment Released.
//
// Program to run an map-based game

#include <stdio.h>

// Additional libraries here


// Provided constants 
#define SIZE 8
#define PLAYER_STARTING_ROW (SIZE - 1)
#define PLAYER_STARTING_COL 0
#define EMPTY_POINTS 0
#define EMPTY_TYPE 'E'
#define PLAYER_TYPE 'P'
#define MONSTER_TYPE 'M'
#define HEALING_TYPE 'H'
#define BOULDER_TYPE 'B'

// Your constants here
#define CHEAT_MAP 'c'
#define QUIT 'q'
#define MOVE 'm'
#define MOVE_UP 'u'
#define MOVE_DOWN 'd'
#define MOVE_LEFT 'l'
#define MOVE_RIGHT 'r'
#define SIZE_LOWEST 0
#define HEALTH 'h'
#define INT_HEALTH 10
#define SQUARE_POINTS 's'
#define WIN 1
#define LOSE 0
#define BOULDER_FALL 'b'
#define RECTANGLE_POINTS 'r'
#define VALID 1
#define INVALID 0
#define HIGH_POINT 9

// Provided struct
struct location {
    char occupier;
    int points;
};

// Your structs here
struct point {
    int row;
    int col;
};

// Provided functions use for game setup
// You do not need to use these functions yourself.
void init_map(struct location map[SIZE][SIZE]);
void place_player_on_starting_location(struct location map[SIZE][SIZE]);

// You will need to use these functions for stage 1.
void print_cheat_map(struct location map[SIZE][SIZE]);
void print_game_play_map(struct location map[SIZE][SIZE]);

// Your functions prototypes here
void movement(struct location map[SIZE][SIZE], char direction, int row, 
int col);
int player_col(char direction, int play_col);
int player_row(char direction, int play_row);
void square_print(struct location map[SIZE][SIZE]);
int win(struct location map[SIZE][SIZE]);
int lose_boulder(struct location map[SIZE][SIZE], int play_row, int play_col);
int lose_health(int play_health);
int boulder_falling(struct location map[SIZE][SIZE]);
void clamp_rectangle(struct point start, struct point end);
int valid_array(int row, int col);
int valid_point(struct point start);
void rectangle_print(struct location map[SIZE][SIZE]);


int main(void) {

    struct location map[SIZE][SIZE];
    init_map(map);

    printf("Welcome Explorer!!\n");
    printf("How many game pieces are on the map?\n");
    
    // TODO: Add code to scan in the number of game pieces here.
    int num_pieces;
    scanf("%d", &num_pieces);
    // TODO: Add code to scan in the details of each game piece and place them
    //       on the map
    
    printf("Enter the details of game pieces:\n");
    
    int num_points;
    int row;
    int col;
    int count = 0;
    
    while (count < num_pieces) {
        scanf("%d %d %d", &num_points, &row, &col);
        if (valid_array(row, col) == VALID) {
            if (num_points < EMPTY_POINTS && num_points >= -HIGH_POINT) {
                map[row][col].occupier = MONSTER_TYPE;
                map[row][col].points = num_points;
            } else if (num_points > EMPTY_POINTS && num_points <= HIGH_POINT) {
                map[row][col].occupier = HEALING_TYPE;
                map[row][col].points = num_points;
            } else if (num_points == EMPTY_POINTS) {
                map[row][col].occupier = BOULDER_TYPE;
                map[row][col].points = num_points;
            }
        }
        if (row == PLAYER_STARTING_ROW && col == PLAYER_STARTING_COL) {
            map[row][col].occupier = PLAYER_TYPE;
            map[row][col].points = EMPTY_POINTS;
        }
        count++; 
    }
    count = 0;
    
    // After the game pieces have been added to the map print out the map.
    print_game_play_map(map);
    printf("\nEXPLORE!\n");
    printf("Enter command: ");
    // TODO: keep scanning in commands from the user until the user presses
    // ctrl + d
    while (scanf("%d %d %d", &num_points, &row, &col) == 3) {
        if ((valid_array(row, col) == VALID) 
        && (row != PLAYER_STARTING_ROW && col != PLAYER_STARTING_COL)) {        
            if (num_points < EMPTY_POINTS && num_points >= -HIGH_POINT) {
                map[row][col].occupier = MONSTER_TYPE;
                map[row][col].points = num_points;
            } else if (num_points > EMPTY_POINTS && num_points <= HIGH_POINT) {
                map[row][col].occupier = HEALING_TYPE;
                map[row][col].points = num_points;
            } else if (num_points == EMPTY_POINTS) {
                map[row][col].occupier = BOULDER_TYPE;
                map[row][col].points = num_points;
            }
        }
    }
   
    char command;
    char direction;
    int play_row = PLAYER_STARTING_ROW;
    int play_col = PLAYER_STARTING_COL;
    int play_health = INT_HEALTH;
    
    while (scanf(" %c", &command) == VALID) {
        if (command == CHEAT_MAP) {
            print_cheat_map(map);
            print_game_play_map(map);
        }
        if (command == QUIT) {
            printf("Exiting Program!\n");
            return 0;
        }
        if (command == MOVE) {
            scanf(" %c", &direction);
            movement(map, direction, play_row, play_col);
            print_game_play_map(map);
            if (map[play_row][play_col].occupier == EMPTY_TYPE) {
                play_row = player_row(direction, play_row);
                play_col = player_col(direction, play_col);  
                play_health = play_health + map[play_row][play_col].points;
            }
        }
        if (command == HEALTH) {
            printf("Your player is at (%d, %d) with a health of %d.\n", 
            play_row, play_col, play_health);
            print_game_play_map(map);
        }
        if (command == SQUARE_POINTS) {
            square_print(map);
            print_game_play_map(map);
        }
        if (command == RECTANGLE_POINTS) {
            rectangle_print(map);
        }
        if (command == BOULDER_FALL) {
            if (boulder_falling(map) == LOSE) {
                print_game_play_map(map);
                printf("GAME LOST!\n");
                return 0;
            } else {
                print_game_play_map(map);
            }
        }
       
        if (lose_health(play_health) == LOSE 
        || lose_boulder(map, play_row, play_col) == LOSE) {
            printf("GAME LOST!\n");
            return 0;
        }
        if (win(map) == WIN) {
            printf("GAME WON!\n");
            return 0;
        }
        printf("Enter command: ");
    }
  
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Function for rectangle points
void rectangle_print(struct location map[SIZE][SIZE]) {
    
    struct point start;
    struct point end;
    char type;
    int points = 0;
    struct point placeholder;

    scanf("%d %d %d %d %c", &start.row, &start.col, &end.row, &end.col, &type);
    
    placeholder.row = start.row;
    placeholder.col = start.col;
    
    clamp_rectangle(start, end);
    
    int length = end.col - start.col;
    
    if (end.row < start.row) {
        start.row = end.row;
        end.row = placeholder.row;
    } 
    if (end.col < start.col) {
        start.col = end.col;
        end.col = placeholder.col;
    }
    
    if (type == MONSTER_TYPE) {
        while (start.row <= end.row) {
            while (start.col <= end.col) {
                if (valid_point(start) == VALID) {
                    if (map[start.row][start.col].occupier == MONSTER_TYPE) {
                        points = points + map[start.row][start.col].points;
                        start.col++;
                    }
                    start.col++;
                } else {
                    start.col++;
                }
            }
            if (valid_point(start) == VALID) {
                if (map[start.row][start.col].occupier == MONSTER_TYPE) {
                    points = points + map[start.row][start.col].points;
                }
            }
            start.row++;
            start.col = end.col - length;
        }
        printf("Monsters in this section could apply %d health points."
        , points);    
    }
    
    if (type == HEALING_TYPE) {
        while (start.row <= end.row) {
            while (start.col <= end.col) {
                if (valid_point(start) == VALID) {
                    if (map[start.row][start.col].occupier == HEALING_TYPE) {
                        points = points + map[start.row][start.col].points;
                    } 
                    start.col++;
                } else {
                    start.col++;
                }
            }
            if (valid_point(start) == VALID) {
                if (map[start.row][start.col].occupier == HEALING_TYPE) {
                    points = points + map[start.row][start.col].points;
                }
            }
            start.row++;
            start.col = end.col - length;
        }
        printf("Healing Potions in this section could apply %d health points."
        , points);    
    }
    printf("\n");
}

// Function for clamping rectangle
void clamp_rectangle(struct point start, struct point end) {
    if (start.row >= SIZE) {
        start.row = SIZE - 1;
    } else if (start.row < SIZE_LOWEST) {
        start.row = SIZE_LOWEST;
    }
    if (start.col >= SIZE) {
        start.col = SIZE - 1;
    } else if (start.col < SIZE_LOWEST) {
        start.col = SIZE_LOWEST;
    }
    
    if (end.row >= SIZE) {
        end.row = SIZE - 1;
    } else if (end.row < SIZE_LOWEST) {
        end.row = SIZE_LOWEST;
    }
    if (end.col >= SIZE) {
        end.col = SIZE - 1;
    } else if (end.col < SIZE_LOWEST) {
        end.col = SIZE_LOWEST;
    }
}

// Function for falling boulders
int boulder_falling(struct location map[SIZE][SIZE]) {
    struct point boulder;
    scanf("%d %d", &boulder.row, &boulder.col);
    
    if (boulder.row >= SIZE) {
        boulder.row = SIZE - 1;
    } else if (boulder.row < SIZE_LOWEST) {
        boulder.row = SIZE_LOWEST;
    }
    if (boulder.col >= SIZE) {
        boulder.col = SIZE - 1;
    } else if (boulder.col < SIZE_LOWEST) {
        boulder.col = SIZE_LOWEST;
    }
    
    if (map[boulder.row][boulder.col].occupier == PLAYER_TYPE) {
        return LOSE;
    } else {
        map[boulder.row][boulder.col].occupier = BOULDER_TYPE;
        map[boulder.row][boulder.col].points = EMPTY_POINTS;
        return WIN;
    }
}

// Function to lose from no health
int lose_health(int play_health) {
    
    if (play_health <= 0) {
        return LOSE;
    } else {
        return 1;
    }
}

// Function to lose from being surrounded by boulders
int lose_boulder(struct location map[SIZE][SIZE], int play_row, int play_col) {
 
    int ending;
 
    play_row++;
    if (play_row < SIZE && play_row >= SIZE_LOWEST) {
        if (map[play_row][play_col].occupier == BOULDER_TYPE) {
            ending = LOSE;
        } else {
            ending = WIN;
        }
        play_row--;
    } else {
        ending = LOSE;
        play_row--;
    }
    
    play_row--;
    if (play_row < SIZE && play_row >= SIZE_LOWEST) {
        if (map[play_row][play_col].occupier == BOULDER_TYPE) {
            ending = ending + LOSE;
        } else {
            ending = ending + WIN;
        }
        play_row++;
    } else {
        ending = ending + LOSE;
        play_row++;
    }
    
    play_col++;
    if (play_col < SIZE && play_col >= SIZE_LOWEST) {
        if (map[play_row][play_col].occupier == BOULDER_TYPE) {
            ending = ending + LOSE;
        } else {
            ending = ending + WIN;
        }
        play_col--;
    } else {
        ending = ending + LOSE;
        play_col--;
    }

    play_col--;
    if (play_col < SIZE && play_col >= SIZE_LOWEST) {
        if (map[play_row][play_col].occupier == BOULDER_TYPE) {
            ending = ending + LOSE;
        } else {
            ending = ending + WIN;
        }
        play_col++;
    } else {
        ending = ending + LOSE;
        play_col++;
    }
    
    if (ending == LOSE) {
        return LOSE;
    } else {
        return WIN;
    }
        
}

// Function to win
int win(struct location map[SIZE][SIZE]) {

    int i = 0;
    int j = 0;
    char end = EMPTY_TYPE;
    
    while (i < SIZE) {
        while (j < SIZE) {
            if (map[i][j].occupier != MONSTER_TYPE) {
                j++;
            } else {
                end = MONSTER_TYPE;
                i = SIZE;
                j = SIZE;
            }
        }
        i++;
        j = 0;
    }
    
    if (end != MONSTER_TYPE) {
        return WIN;
    } else {
        return LOSE;
    }
}

// Function to print square points
void square_print(struct location map[SIZE][SIZE]) {
    
    int row;
    int col;
    int size;
    char type;
    int points = 0;
    
    scanf("%d %d %d %c", &row, &col, &size, &type);
    
    if (row >= SIZE) {
        row = SIZE - 1;
    } else if (row < SIZE_LOWEST) {
        row = SIZE_LOWEST;
    }
    if (col >= SIZE) {
        col = SIZE - 1;
    } else if (col < SIZE_LOWEST) {
        col = SIZE_LOWEST;
    }
    
    int square_row = row + size - 1;
    int square_col = col + size - 1;
    
    if (type == MONSTER_TYPE) {
        while (row < square_row) {
            while (col < square_col) {
                if (valid_array(row, col) == VALID) {
                    if (map[row][col].occupier == MONSTER_TYPE) {
                        points = points + map[row][col].points;
                        col++;
                    } else {
                        col++;
                    }
                } else {
                    col++;
                }
            }
            if (valid_array(row, col) == VALID) {
                if (map[row][col].occupier == MONSTER_TYPE) {
                    points = points + map[row][col].points;
                }
            }
            row++;
            col = square_col - size + 1;
        }
        printf("Monsters in this section could apply %d health points."
        , points);    
    }
    
    if (type == HEALING_TYPE) {
        while (row < square_row) {
            while (col < square_col) {
                if (valid_array(row, col) == VALID) {
                    if (map[row][col].occupier == HEALING_TYPE) {
                        points = points + map[row][col].points;
                        col++;
                    } else {
                        col++;
                    }
                } else {
                    col++;
                }
            }
            if (valid_array(row, col) == VALID) {
                if (map[row][col].occupier == HEALING_TYPE) {
                    points = points + map[row][col].points;
                }
            }
            row++;
            col = square_col - size + 1;
        }
        printf("Healing Potions in this section could apply %d health points."
        , points);
    }
    printf("\n");
}


// Functions to change row and column values 
int player_col(char direction, int play_col) {
    
    if (direction == MOVE_LEFT) {
        play_col--;
    } else if (direction == MOVE_RIGHT) {
        play_col++;
    }
    
    return play_col;
}

int player_row(char direction, int play_row) {
   
    if (direction == MOVE_UP) {
        play_row--;
    } else if (direction == MOVE_DOWN) {
        play_row++;
    }
    
    return play_row;
}

// Function to move player 
void movement(struct location map[SIZE][SIZE], char direction, 
int play_row, int play_col) {
    
    int reset_row = play_row;
    int reset_col = play_col;
    
    if (direction == MOVE_UP) {
        play_row--;
    } else if (direction == MOVE_DOWN) {
        play_row++;
    } else if (direction == MOVE_LEFT) {
        play_col--;
    } else if (direction == MOVE_RIGHT) {
        play_col++;
    }
    
    if (play_row >= SIZE || play_row < SIZE_LOWEST || play_col >= SIZE 
    || play_col < SIZE_LOWEST) {
        play_row = reset_row;
        play_col = reset_col;
        map[play_row][play_col].occupier = PLAYER_TYPE;
    } else if (map[play_row][play_col].occupier == BOULDER_TYPE) {
        play_row = reset_row;
        play_col = reset_col;
    } else {
        map[play_row][play_col].occupier = PLAYER_TYPE;
        map[reset_row][reset_col].occupier = EMPTY_TYPE;
    }
}
// Functions for whether row and column values are valid
int valid_array(int row, int col) {
    
    int result;    
    
    if (row >= SIZE_LOWEST && row < SIZE && col >= SIZE_LOWEST 
    && col < SIZE) {
        result = VALID;
    } else {
        result = INVALID;
    }
    
    return result;
}
 
int valid_point(struct point start) {
    
    int result;
    
    if (start.row >= SIZE_LOWEST && start.row < SIZE && start.col >= SIZE_LOWEST 
    && start.col < SIZE) {
        result = VALID;
    } else {
        result = INVALID;
    }
    
    return result;
}   

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function
// Initalises all elements on the map to be empty to prevent access errors.
void init_map(struct location map[SIZE][SIZE]) {
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            struct location curr_loc;
            curr_loc.occupier = EMPTY_TYPE;
            curr_loc.points = EMPTY_POINTS;
            map[row][col] = curr_loc;
            col++;
        }
        row++;
    }

    place_player_on_starting_location(map);
}

// Provided Function
// Places the player in the bottom left most location.
void place_player_on_starting_location(struct location map[SIZE][SIZE]) {
    map[PLAYER_STARTING_ROW][PLAYER_STARTING_COL].occupier = PLAYER_TYPE;
}

// Provided Function
// Prints out map with alphabetic values. Monsters are represented with 'M',
// healing potions in 'H', boulders with 'B' and the player with 'P'.
void print_game_play_map(struct location map[SIZE][SIZE]) {
    printf(" -----------------\n");
    int row = 0;
    while (row < SIZE) {
        printf("| ");
        int col = 0;
        while (col < SIZE) {
            if (map[row][col].occupier == EMPTY_TYPE) {
                printf("- ");
            } else {
                printf("%c ", map[row][col].occupier);
            }
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(" -----------------\n");
}

// Provided Function
// Prints out map with numerical values. Monsters are represented in red,
// healing potions in blue, boulder in green and the player in yellow.
//
// We use some functionality you have not been taught to make sure that
// colours do not appear during testing.
void print_cheat_map(struct location map[SIZE][SIZE]) {

    printf(" -----------------\n");
    int row = 0;
    while (row < SIZE) {
        printf("| ");
        int col = 0;
        while (col < SIZE) {
            if (map[row][col].occupier == PLAYER_TYPE) {
                // print the player in purple
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;35m");
                #endif
                // ----------------------------------------
                printf("%c ", PLAYER_TYPE);
            } else if (map[row][col].occupier == HEALING_TYPE) {
                // print healing potion in green
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;32m");
                #endif
                // ----------------------------------------
                printf("%d ", map[row][col].points);
            } else if (map[row][col].occupier == MONSTER_TYPE) {
                // print monsters in red
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;31m");
                #endif
                // ----------------------------------------
                printf("%d ", -map[row][col].points);
            } else if (map[row][col].occupier == BOULDER_TYPE) {
                // print boulder in blue
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;34m");
                #endif
                // ----------------------------------------
                printf("%d ", map[row][col].points);
            } else {
                // print empty squares in the default colour
                printf("- ");
            }
            // ----------------------------------------
            // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
            // reset the colour back to default
            #ifndef NO_COLORS
            printf("\033[0m");
            #endif
            // ----------------------------------------
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(" -----------------\n");
}