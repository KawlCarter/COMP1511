// COMP1511 cs_flowers.c
//
// This program was written by [your name] (z5555555)
// on [23.03.2024]
//

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
    int prev_action;
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
int perform_w_command(struct tile map[][MAX_COL], int game_row, int game_col);
int perform_a_command(struct tile map[][MAX_COL], int game_row, int game_col);
void perform_s_command(struct tile map[][MAX_COL], int game_row, int game_col);
void perform_d_command(struct tile map[][MAX_COL], int game_row, int game_col);
void updateflower_states(struct tile map[MAX_ROW][MAX_COL],
    int row_idx, int col_idx);
int valid_indexs(int row, int col);
void alert_flowers(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col
);
void print_map_mod(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col
);
void scan_branches(struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col);
void print_map_mod1(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col,
    int org_row,
    int org_col
);
void weak_points(struct tile map[MAX_ROW][MAX_COL], int row,
    int col, int * prev_placed_val);
int check_map(struct tile map[][MAX_COL]);
void print_tile(struct tile tile);
void print_flower(struct flower flower);
void add_branch(struct tile map[][MAX_COL], int row, int col);
void add_bush(struct tile map[][MAX_COL], int start_row, int start_col,
    char orientation, int length);
//------------------------------------------------------------------------------
// Main Function
//------------------------------------------------------------------------------
/**
 * protect_weak_point
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
int protect_weak_point(struct tile map[MAX_ROW][MAX_COL],
    int player_row, int player_col) {
    for (int i = 1; i < MAX_ROW - 1; i += 2) {
        for (int j = 1; j < MAX_COL - 1; j += 2) {
            if (map[i][j].type == FLOWER && map[i][j].flower.state != DORMANT) {
                if (map[i][j].flower.prev_action == 0) {
                    map[i][j].flower.prev_action = 1;
                    if (map[i + 1][j].type == EMPTY && (i + 1 != player_row
                        || j != player_col)) {
                        map[i + 1][j].type = BUSH;
                        continue;
                    }
                    if (map[i + 1][j].type == EMPTY && map[i][j + 1].type == EMPTY) {
                        map[i][j + 1].type = BUSH;
                        continue;
                    }
                    if (map[i + 1][j].type == BRANCH) {
                        map[i + 1][j].type = BUSH;
                    }
                    if (map[i + 1][j].type == BUSH && map[i][j + 1].type == EMPTY) {
                        map[i][j + 1].type = BUSH;
                        continue;
                    }
                    if (map[i][j + 1].type == BUSH && map[i - 1][j].type == EMPTY) {
                        map[i - 1][j].type = BUSH;
                        continue;
                    }
                    if (map[i][j + 1].type == BUSH && map[i - 1][j].type == BRANCH) {
                        map[i - 1][j].type = BUSH;
                    }
                    if (map[i - 1][j].type == BUSH && map[i][j - 1].type == EMPTY) {
                        map[i][j - 1].type = BUSH;
                        continue;
                    }
                }
                else if (map[i][j].flower.prev_action == 1)
                    map[i][j].flower.prev_action = 0;
                else if (map[i][j].type == FLOWER && map[i][j].flower.state == DISTURBED)
                    map[i][j].flower.prev_action = 0;
            }
        }
    }
    return 0;
}
/**
 * add_flowers
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param row: The row coordinate of the player.
 * @param col: The column coordinate of the player.
 *
 * @returns None
**/
void add_flowers(struct tile map[][MAX_COL], int row, int col) {
    if (valid_indexs(row, col)) {
        if (row % 2 != 0 && col % 2 != 0 &&
            (map[row][col].type != BUSH && map[row][col].type != BRANCH))
        {
            map[row][col].type = FLOWER;
            map[row][col].flower.state = DORMANT;
        }
        else
        {
            printf("Invalid flower position!\n");
        }
    }
    else {
        printf("Invalid flower position!\n");
    }
}
/**
 * flow_check_rows
 *
 * @param game_row: The row coordinate of the player.
 * @returns None
**/
int flow_check_rows(int row)
{
    int flag = 0;
    if (row >= 0 && row < MAX_ROW - 1)
    {
        flag = 1;
    }
    return flag;
}
/**
 * flow_check_cols
 *
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
int flow_check_cols(int col)
{
    int flag = 0;
    if (col >= 0 && col < MAX_COL - 1)
    {
        flag = 1;
    }
    return flag;
}
/**
 * odd_idxs
 *
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
int odd_idxs(int row, int col)
{
    int flag = 0;
    if (row % 2 != 0 && col % 2 != 0)
    {
        flag = 1;
    }
    return flag;
}
/**
 * fill_max_flowers
 *
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
int flow_valid_indexs(int row, int col)
{
    int flag_row = flow_check_rows(row);
    int flag_col = flow_check_cols(col);
    int flag_odd_idxs = odd_idxs(row, col);
    int final_flag = 0;
    if (flag_row && flag_col)
    {
        final_flag = 1;
    }
    else
    {
        final_flag = 0;
    }
    return final_flag && flag_odd_idxs;
}
/**
 * fill_max_flowers
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 *
 * @returns None
**/
void fill_max_flowers(struct tile map[][MAX_COL])
{
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (flow_valid_indexs(i, j)) {
                if (map[i][j].type != BUSH) {
                    map[i][j].type = FLOWER;
                    map[i][j].flower.state = DORMANT;
                }
            }
        }
    }
}
int main(void) {
    struct tile map[MAX_ROW][MAX_COL];
    initialise_map(map);
    int player_row, player_col;
    printf("Welcome to CS Flowers!\n");
    printf("Player's starting position: ");
    scanf("%d %d", &player_row, &player_col);
    while (player_row != 0 && player_row != MAX_ROW - 1 &&
        player_col != 0 && player_col != MAX_COL - 1) {
        printf("Invalid starting position!\n");
        printf("Re-enter starting position: ");
        scanf("%d %d", &player_row, &player_col);
    }
    print_map(map, player_row, player_col);
    int num_foliage;
    int num_flowers;
    printf("How many tiles of foliage: ");
    scanf("%d", &num_foliage);
    char type;
    int row, col, length;
    char orientation;
    if (num_foliage != 0)
    {
        for (int i = 0; i < num_foliage; i++) {

            scanf(" %c", &type);
            if (type == 'b')
            {
                scanf("%d  %d", &row, &col);
                add_branch(map, row, col);
            }
            else if (type == 'u')
            {
                scanf("%d %d %c %d", &row, &col, &orientation, &length);
                if (orientation == 'v' || orientation == 'h') {
                    add_bush(map, row, col, orientation, length);
                }
            }
        }
        print_map(map, player_row, player_col);
    }
    else {
        print_map(map, player_row, player_col);
    }
    printf("How many flowers: ");
    scanf("%d", &num_flowers);
    if (num_flowers != 0 && num_flowers < 25)
    {
        for (int i = 0; i < num_flowers; i++) {

            scanf("%d  %d", &row, &col);
            add_flowers(map, row, col);
        }
    }
    else if (num_flowers >= 25)
    {
        fill_max_flowers(map);
    }
    print_map(map, player_row, player_col);

    char command;
    char cut_c;
    int game_row = player_row;
    int game_col = player_col;
    int org_r = game_row;
    int org_c = game_col;
    printf("Game Started! \n");
    while (1)
    {
        printf("Enter command: ");
        if (scanf(" %c", &command) != 1) {
            break;
        }
        int tmp = 1;
        if (command == 'c') {
            scanf(" %c", &cut_c);
            tmp++;
        }

        if (tmp == 1)
        {
            if (command == 'w')
            {
                if (valid_indexs(game_row - 1, game_col))
                {
                    if (map[game_row - 1][game_col].flower.state != DORMANT)
                    {
                        protect_weak_point(map, player_row, player_col);
                    }
                    if (map[game_row - 1][game_col].type != BUSH &&
                        map[game_row - 1][game_col].type != FLOWER)
                    {
                        game_row = game_row - 1;
                        if (map[game_row][game_col].type == BRANCH)
                            alert_flowers(map, game_row, game_col);
                        print_map_mod(map, game_row, game_col);

                    }
                    else {
                        print_map_mod(map, game_row, game_col);
                    }
                }
                else {
                    print_map_mod(map, game_row, game_col);
                }
            }
            else if (command == 'a')
            {
                if (valid_indexs(game_row, game_col - 1))
                {
                    if (map[game_row][game_col - 1].type != BUSH &&
                        map[game_row][game_col - 1].type != FLOWER)
                    {
                        game_col = game_col - 1;
                        if (map[game_row][game_col].type == BRANCH)
                            alert_flowers(map, game_row, game_col);
                        print_map_mod(map, game_row, game_col);
                    }
                    else {
                        print_map_mod(map, game_row, game_col);
                    }

                }
                else {
                    print_map_mod(map, game_row, game_col);
                }
            }
            else if (command == 's')
            {
                if (valid_indexs(game_row + 1, game_col))
                {
                    if (map[game_row + 1][game_col].flower.state != DORMANT)
                    {
                        protect_weak_point(map, player_row, player_col);
                    }
                    if (map[game_row + 1][game_col].type != BUSH &&
                        map[game_row + 1][game_col].type != FLOWER)
                    {
                        game_row = game_row + 1;
                        if (map[game_row][game_col].type == BRANCH)
                            alert_flowers(map, game_row, game_col);
                        print_map_mod(map, game_row, game_col);
                    }
                    else {
                        print_map_mod(map, game_row, game_col);
                    }
                }
                else {
                    print_map_mod(map, game_row, game_col);
                }
            }
            else if (command == 'd')
            {
                if (valid_indexs(game_row, game_col + 1))
                {
                    if (map[game_row][game_col + 1].flower.state != DORMANT)
                    {
                        protect_weak_point(map, player_row, player_col);
                    }
                    if (map[game_row][game_col + 1].type != BUSH &&
                        map[game_row][game_col + 1].type != FLOWER)
                    {
                        game_col = game_col + 1;
                        if (map[game_row][game_col].type == BRANCH)
                            alert_flowers(map, game_row, game_col);
                        print_map_mod(map, game_row, game_col);
                    }
                    else {
                        print_map_mod(map, game_row, game_col);
                    }
                }
                else {
                    print_map_mod(map, game_row, game_col);
                }
            }
            else if (command == 'i')
            {
                protect_weak_point(map, player_row, player_col);
                print_map(map, game_row, game_col);
            }
        }
        else if (tmp == 2)
        {
            command = cut_c;
            if (command == 'w')
            {
                if (valid_indexs(game_row - 1, game_col))
                {
                    if (perform_w_command(map, game_row, game_col))
                        break;
                }
                else {
                    org_r = game_row;
                    org_c = game_col;
                    print_map(map, org_r, org_c);
                }
            }
            else if (command == 'a')
            {
                if (valid_indexs(game_row, game_col - 1))
                {
                    if (perform_a_command(map, game_row, game_col))
                        break;
                }
                else {
                    org_r = game_row;
                    org_c = game_col;
                    print_map(map, org_r, org_c);
                }
            }
            else if (command == 's')
            {
                if (valid_indexs(game_row + 1, game_col))
                {
                    perform_s_command(map, game_row, game_col);
                }
                else {
                    org_r = game_row;
                    org_c = game_col;
                    print_map(map, org_r, org_c);
                }
            }
            else if (command == 'd')
            {
                if (valid_indexs(game_row, game_col + 1))
                {
                    perform_d_command(map, game_row, game_col);
                }
                else {
                    org_r = game_row;
                    org_c = game_col;
                    print_map(map, org_r, org_c);
                }
            }
        }
    }
    return 0;
}
/**
 * perform_d_command
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
void perform_d_command(struct tile map[][MAX_COL], int game_row, int game_col)
{
    int org_r = game_row;
    int org_c = game_col;
    if (map[game_row][game_col + 1].type == FLOWER ||
        map[game_row][game_col + 1].type == BUSH)
    {
        org_r = game_row;
        org_c = game_col;
        game_col = game_col + 1;
        if (map[game_row][game_col].type == FLOWER)
            alert_flowers(map, game_row, game_col);
        map[game_row][game_col].type = EMPTY;

        print_map(map, org_r, org_c);
        game_row = org_r;
        game_col = org_c;
    }
    else {
        org_r = game_row;
        org_c = game_col;
        print_map(map, org_r, org_c);
    }
}
/**
 * perform_s_command
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
void perform_s_command(struct tile map[][MAX_COL], int game_row, int game_col)
{
    int org_r = game_row;
    int org_c = game_col;
    if (map[game_row + 1][game_col].type == BUSH ||
        map[game_row + 1][game_col].type == FLOWER)
    {
        org_r = game_row;
        org_c = game_col;
        game_row = game_row + 1;
        map[game_row][game_col].type = EMPTY;
        print_map(map, org_r, org_c);
        game_row = org_r;
        game_col = org_c;
    }
    else {
        org_r = game_row;
        org_c = game_col;
        print_map(map, org_r, org_c);
    }
}
/**
 * perform_w_command
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
int perform_w_command(struct tile map[][MAX_COL], int game_row, int game_col)
{
    int org_r = game_row;
    int org_c = game_col;
    if (map[game_row - 1][game_col].type == BUSH ||
        map[game_row - 1][game_col].type == FLOWER)
    {
        org_r = game_row;
        org_c = game_col;
        game_row = game_row - 1;
        if (map[game_row][game_col].type == FLOWER)
            alert_flowers(map, game_row, game_col);
        map[game_row][game_col].type = EMPTY;
        print_map(map, org_r, org_c);
        if (check_map(map))
        {
            printf("All flowers are eradicated and UNSW has been saved!\n");
            return 1;
        }
        game_row = org_r;
        game_col = org_c;
    }
    else {
        org_r = game_row;
        org_c = game_col;
        print_map(map, org_r, org_c);
    }
    return 0;
}
/**
 * perform_a_command
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param game_row: The row coordinate of the player.
 * @param game_col: The column coordinate of the player.
 *
 * @returns None
**/
int perform_a_command(struct tile map[][MAX_COL], int game_row, int game_col)
{
    int org_r = game_row;
    int org_c = game_col;
    if (map[game_row][game_col - 1].type == BUSH ||
        map[game_row][game_col - 1].type == FLOWER)
    {

        org_r = game_row;
        org_c = game_col;
        game_col = game_col - 1;
        if (map[game_row][game_col].type == FLOWER)
            alert_flowers(map, game_row, game_col);
        map[game_row][game_col].type = EMPTY;
        print_map(map, org_r, org_c);
        if (check_map(map))
        {
            printf("All flowers are eradicated and UNSW has been saved!\n");
            return 1;
        }
        game_row = org_r;
        game_col = org_c;
    }
    else {
        org_r = game_row;
        org_c = game_col;
        print_map(map, org_r, org_c);
    }
    return 0;
}
/**
 * check_rows
 *
 * @param row: The row coordinate of the player.
 *
 * @returns None
**/
int check_rows(int row)
{
    int flag = 0;
    if (row >= 0 && row < MAX_ROW)
    {
        flag = 1;
    }
    return flag;
}
/**
 * check_cols
 *
 * @param col: The column coordinate of the player.
 *
 * @returns None
**/
int check_cols(int col)
{
    int flag = 0;
    if (col >= 0 && col < MAX_COL)
    {
        flag = 1;
    }
    return flag;
}
/**
 * valid_indexs
 *
 * @param row: The row coordinate of the player.
 * @param col: The column coordinate of the player.
 *
 * @returns None
**/
int valid_indexs(int row, int col)
{
    int flag_row = check_rows(row);
    int flag_col = check_cols(col);
    int final_flag = 0;
    if (flag_row && flag_col)
    {
        final_flag = 1;
    }
    else
    {
        final_flag = 0;
    }
    return final_flag;
}
/**
 * check_map
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 *
 * @returns None
**/
int check_map(struct tile map[][MAX_COL])
{
    int flag = 0;
    for (int i = 0; i < MAX_ROW; i++)
    {
        for (int j = 0; j < MAX_COL; j++)
        {
            if (map[i][j].type != FLOWER)
            {
                flag = 1;
            }
            else {
                flag = 0;
                break;
            }
        }
        if (!flag)
        {
            break;
        }
    }
    return flag;
}

/**
 * add_branch
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param row: The row coordinate of the player.
 * @param col: The column coordinate of the player.
 *
 * @returns None
**/
void add_branch(struct tile map[][MAX_COL], int row, int col)
{
    if (row > 0 && row < MAX_ROW - 1 &&
        col > 0 && col < MAX_COL - 1)
    {
        map[row][col].type = BRANCH;
        printf("Branch added!\n");
    }
    else {
        printf("Invalid foliage position!\n");
    }
}

/**
 * add_bush
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param start_row: The row coordinate of the player.
 * @param start_col: The column coordinate of the player.
 *
 * @returns None
**/
void add_bush(struct tile map[][MAX_COL], int start_row, int start_col,
    char orientation, int length) {
    if ((start_row > 0 && start_row < MAX_ROW - 1 &&
        start_col > 0 && start_col < MAX_COL - 1) &&
        (map[start_row][start_col].type != BRANCH))
    {
        for (int i = 0; i < length; i++) {
            if (orientation == 'v') {
                map[start_row + i][start_col].type = BUSH;
            }
            else if (orientation == 'h') {
                map[start_row][start_col + i].type = BUSH;
            }
        }
        printf("Bush added!\n");
    }
    else
    {
        printf("Invalid foliage position!\n");
    }
}

/**
 * alert_flowers
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/

void alert_flowers(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col
)
{
    // we need to process 5x5 grid
    // we need to scan 24 positions
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int row_idx = (player_row - 5 / 2 + i);
            int col_idx = (player_col - 5 / 2 + j);
            if (valid_indexs(row_idx, col_idx))
            {
                updateflower_states(map, row_idx, col_idx);
            }
        }
    }

}

/**
 * updateflower_states
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
void updateflower_states(struct tile map[MAX_ROW][MAX_COL],
    int row_idx, int col_idx) {

    if (map[row_idx][col_idx].flower.state == DORMANT)
    {
        map[row_idx][col_idx].flower.state = DISTURBED;
    }
    else if (map[row_idx][col_idx].flower.state == DISTURBED)
    {
        map[row_idx][col_idx].flower.state = AWAKENED;
    }
    else if (map[row_idx][col_idx].flower.state == AWAKENED)
    {
        map[row_idx][col_idx].flower.state = AWAKENED;
    }
}

//------------------------------------------------------------------------------

// TODO: Put your functions here
/**
 * Check to see if valid starting position is given
 *
 * @param row:  starting row
 * @param col:  starting col
 *
 * @returns an integer 0 or 1
 *
**/

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
            map[row][col].flower.prev_action = 0;
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
        for (int j = 0; j < MAX_COL; j++) {
            printf("|");
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            }
            else if (map[i][j].type == EMPTY) {
                printf(" P ");
            }
            else if (map[i][j].type == DANGER) {
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
 * Prints the game map
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
void print_map_mod(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col
) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < MAX_COL; j++) {
            printf("|");
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            }
            else if (map[i][j].type == EMPTY || map[i][j].type == BRANCH) {
                map[i][j].type = EMPTY;
                printf(" P ");
            }
            else if (map[i][j].type == DANGER) {
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
 * Prints the game map
 *
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player.
 * @param player_col: The column coordinate of the player.
 *
 * @returns None
**/
void print_map_mod1(
    struct tile map[MAX_ROW][MAX_COL],
    int player_row,
    int player_col,
    int org_row,
    int org_col
) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("+---");
        }
        printf("+\n");
        for (int j = 0; j < MAX_COL; j++) {
            printf("|");
            if (i == org_row && j == org_col)
            {
                map[i][j].type = EMPTY;
                printf(" P ");
            }
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            }
            else if (map[i][j].type == BUSH || map[i][j].type == FLOWER) {
                map[i][j].type = EMPTY;
                //printf("P");
            }
            else if (map[i][j].type == DANGER) {
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
    }
    else if (tile.type == BUSH) {
        printf("###");
    }
    else if (tile.type == BRANCH) {
        printf("_/-");
    }
    else if (tile.type == DANGER) {
        printf("< >");
    }
    else if (tile.type == ROOT) {
        printf("<^>");
    }
    else if (tile.type == POLLEN) {
        printf(":::");
    }
    else if (tile.type == FLOWER) {
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
    }
    else if (flower.state == DISTURBED) {
        printf("uwo");
    }
    else if (flower.state == AWAKENED) {
        printf("owo");
    }
    else if (flower.state == ANXIOUS) {
        printf("o~o");
    }
    else if (flower.state == ENERGETIC) {
        printf("*w*");
    }
}
