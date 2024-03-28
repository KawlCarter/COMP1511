// Program name
//
// This program was written by [your name] (z5555555)
// on [date]
//
// TODO: Description of program

#include <stdio.h>

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define MAX_ROW 11
#define MAX_COL 11
#define TRUE 1
#define FALSE 0

// TODO: put your constants here 

//------------------------------------------------------------------------------
// Struct & Enum
//------------------------------------------------------------------------------

enum flower_state {
    NONE,
    DORMANT,
    DISTURBED,
    AWAKENED,
    ANXIOUS,
    ENERGETIC
};

struct flower {
    enum flower_state state;
    // TODO: You will need to add more to this struct in Stage 3
};

enum tile_type {
    EMPTY,
    BUSH,
    BRANCH,
    DANGER,
    ROOT,
    POLLEN,
    SEEDLING,
    FLOWER,
};

struct tile {
    enum tile_type type;
    struct flower flower;
};

// TODO: Put your structs here 

//------------------------------------------------------------------------------
// Your Function Prototypes
//------------------------------------------------------------------------------

// TODO: Put your function prototypes here

//------------------------------------------------------------------------------
// Provided Function Prototypes
//------------------------------------------------------------------------------

void initialise_map(struct tile map[MAX_ROW][MAX_COL]);
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
);

void print_tile(struct tile tile);
void print_flower(struct flower flower);

//------------------------------------------------------------------------------
// Main Function
//------------------------------------------------------------------------------

int main(void) {
    struct tile map[MAX_ROW][MAX_COL];
    initialise_map(map);

    printf("Welcome to CS Flowers!\n");

    // TODO: Start writing code here!

    // TODO: Stage 1.1 - Scan in the player position and print out the map

    // TODO: Stage 1.2 - Validate the player's spawn and re-scan if invalid

    // TODO: Stage 1.3 - Add foliage onto the map, starting with branches

    return 0;
}

//------------------------------------------------------------------------------
// Your Function
//------------------------------------------------------------------------------

// TODO: Put your functions here

//------------------------------------------------------------------------------
// Provided Functions
//------------------------------------------------------------------------------

/**
 * Initialises the game map with empty tiles, setting the type of each tile to 
 * `EMPTY` and initialising flower-related properties.
 *
 * @param map: The 2D array of `struct tile` representing the game map 
 *
 * @returns None
 *
**/
void initialise_map(struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row < MAX_ROW; row++) {
        for (int col = 0; col < MAX_COL; col++) {
            map[row][col].type = EMPTY;
            map[row][col].flower.state = NONE;
        }
    }
}

/**
 * Prints the game map 
 * 
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < MAX_COL;j++) {
            printf("|");
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            } else if (map[i][j].type == EMPTY) {
                printf(" P ");
            } else if (map[i][j].type == DANGER) {
                printf("<P>");
            }
        }

        printf("|\n");
    }

    for (int j = 0; j < MAX_COL; j++) {
        printf("+---");
    }
    printf("+\n");
}

/**
 * Helper function which prints the representation of a single tile 
 * based on its type. 
 *
 * @params tile: The `struct tile` to be printed.
 *
 * @returns None
**/
void print_tile(struct tile tile) {
    if (tile.type == EMPTY) {
        printf("   ");
    } else if (tile.type == BUSH) {
        printf("###");
    } else if (tile.type == BRANCH) {
        printf("_/-");
    } else if (tile.type == DANGER) {
        printf("< >");
    } else if (tile.type == ROOT) {
        printf("<^>");
    } else if (tile.type == POLLEN) {
        printf(":::");
    } else if (tile.type == FLOWER) {
        print_flower(tile.flower);
    }
}

/**
 * Helper function which prints a visual representation of the 
 * flower face based on its state.
 *
 * @params flower: The `struct flower` containing the state of the flower.
 *
 * @returns None
 *
**/
void print_flower(struct flower flower) {
    if (flower.state == DORMANT) {
        printf("uwu");
    } else if (flower.state == DISTURBED) {
        printf("uwo");
    } else if (flower.state == AWAKENED) {
        printf("owo");
    } else if (flower.state == ANXIOUS) {
        printf("o~o");
    } else if (flower.state == ENERGETIC) {
        printf("*w*");
    }
}

