// A C program simulates random walks across a 9x9 grid to calculate escape probabilities 
// and path statistics for each cell on a hazardous island. 
// Outputs include escape probability, mean path length, and path length standard deviation.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


#define NUMROWS	 9    // map size
#define NUMCOLS  9    // map size
#define NUMWALKS 1000 // number of random walks
#define NUMSTEP  10   // number of steps in each walk

// Struct that will contain the essential info for every taken path - 
// If the path was successfull and the number of steps that were taken.
struct path_info { 
    bool success;
    int path_length;
};

// A struct that defines the data that is stored in every cell of a map
struct cell {
    char letter;  // B, L, V, D or W
    double mean_path_length;
    double probability;
    double std_deviation;
};

// Function to determine the status of a cell based on its contents
// Returns an integer code representing the status:
// 0 for danger (Dinosaur, Volcano, Water),
// 1 for escape (Bridge),
// 2 for safe (Land),
// 3 for undefined types.
int calculate_status(struct cell map[][NUMCOLS], int x, int y) {
    char current_object = map[y][x].letter;
    if (current_object == 'D' || current_object == 'V' || current_object == 'W') {
        return 0;
    }
    else if (current_object == 'B') {
        return 1;
    }
    else if (current_object == 'L') {
        return 2;
    }
    else {
        return 3;
    }

}

// Function to show the the map of letters
void show_map(struct cell map[][NUMCOLS])
{
    printf("Map:\n");
    // Itterate through every element in the map (row & column)
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" "); // Add a space before every letter in a row unless its the very first one.
            }
            printf("%c", map[y][x].letter); // Show the letter
        }
        printf("\n"); // Move on to the next line
    }
}

// Function to show the probability of escape
void show_probability_of_escape(struct cell map[][NUMCOLS])
{
    printf("Probability of escape:\n");
    // Itterate through every element in the map (row & column)
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" "); // Add a space before every number in a row unless its the very first one.
            }
            printf("%.2f", map[y][x].probability); // Show probability to 2 decimal places
        }
        printf("\n"); // Move on to the next line
    }
}

// Function to show the mean path length
void show_mean_path_length(struct cell map[][NUMCOLS])
{
    printf("Mean path length:\n");
    // Itterate through every element in the map (row & column)
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" "); // Add a space before every number in a row unless its the very first one.
            }
            printf("%.2f", map[y][x].mean_path_length); // Show mean path length to 2 decimal places
        }
        printf("\n"); // Move on to the next line
    }
}

// Function to show the standard deviation of path length
void show_std_of_path_length(struct cell map[][NUMCOLS])
{
    printf("Standard deviation of path length:\n");
    // Itterate through every element in the map (row & column)
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" "); // Add a space before every number in a row unless its the very first one.
            }
            printf("%.2f", map[y][x].std_deviation); // Show standard deviation to 2 decimal places
        }
        printf("\n"); // Move on to the next line
    }
}

// Function to simulate a random path from a starting cell on a map
// Returns a path_info struct indicating if the escape was successful and the path length
struct path_info generate_a_random_path(struct cell map[][NUMCOLS], int starting_x, int starting_y){
    struct path_info path;          // Resulting path information
    int x = starting_x;             // Current x coordinate
    int y = starting_y;             // Current y coordinate
    int rand_num;                   // Variable to store random number
    int movement_direction_index;   // Index for random direction
    int status;                     // Status from current cell

    // Iterate through a maximum of NUMSTEP steps
    for (int step = 0; step < NUMSTEP; step++)
    {
        status = calculate_status(map, x, y); // Calculate the current cell's status

        // Handle different statuses
        if (status == 0){ // Dangerous cell
            path.success = 0;
            path.path_length = step;
            return path;
        }
        else if (status == 1){ // Bridge cell (successful escape)
            path.success = true;
            path.path_length = step;
            return path;
        }
        else if (status == 3) { // Undefined cell type
            path.path_length = NUMSTEP + 1; // path length for undefined behavior
            return path;
        }
        
        // Generate a random direction
        rand_num = rand();
        movement_direction_index = rand_num % 8; // Only 8 possible directions

        // Move in the chosen direction
        switch (movement_direction_index) {
            case 0: y--; break;         // North
            case 1: y--; x++; break;    // Northeast
            case 2: x++; break;         // East
            case 3: x++; y++; break;    // Southeast
            case 4: y++; break;         // South
            case 5: x--; y++; break;    // Southwest
            case 6: x--; break;         // West
            case 7: x--; y--; break;    // Northwest
            default: break;
        }

        // Ensure the coordinates stay within map bounds
        x = (x < NUMCOLS) ? x : (NUMCOLS - 1);
        x = (x > 0) ? x : 0;
        y = (y < NUMROWS) ? y : (NUMROWS - 1);
        y = (y > 0) ? y : 0;

    }

    // Final check if the number of steps reached the maximum number of steps (NUMSTEP)
    path.path_length = NUMSTEP;
    status = calculate_status(map, x, y); // Calculate status for the final position
    if (status == 1){ // Bridge cell (successful escape)
        path.success = true;
    }
    else { // Everything else
        path.success = false;
    }
    return path;
}


int main(void) {

    srand(123456); // Set the required seed 

    FILE *fptr;
    struct cell map[NUMROWS][NUMCOLS];

    char single_space;

    fptr = fopen("island_map.txt", "r"); // Select the file to import the map from

    if(fptr == NULL) { // display error if can't open file
        printf("Error!");
		return 1;
        }
    
    // Read the map from the file
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            single_space = fscanf(fptr, "%c ", &map[y][x].letter); // Read a single character for each cell
        }
    }

    fclose(fptr);

    // Iterate over each cell in the grid to to simulate random walks, collect data and calculate required info
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            double success_rate;                    // Probabilty of a successful escape for a selected cell
            double mean_path;                       // Mean path length for a selected cell
            double std;                             // Standard deviation for a selected cell
            int successful_walks = 0;               // The number of successful walks for a selected cell
            int total_path_length = 0;              // Sum of all path lengths
            struct path_info path_inf[NUMWALKS];    // An Array to store path information for every walk for a selected cell

            // Perform a series of random walks for selected cell
            for (int walk = 0; walk < NUMWALKS; walk++)
            {
                path_inf[walk] = generate_a_random_path(map, x, y); // Generate a random walk from current cell
                if (path_inf[walk].path_length == 0) // Died or Escaped instantly
                {
                    break; // No need to simulate all other walks, as they will produce the same result
                }
                else if (path_inf[walk].path_length > NUMSTEP) // If the path length exceeds the maximum number of steps an error will be returned
                {
                    printf("Error!"); 
                    return 1;
                }
                else if (path_inf[walk].success) // A walk was successful
                {
                    successful_walks++;                                 // Increment successful walks
                    total_path_length += path_inf[walk].path_length;    // Accumulate total path length
                }
            }
            if (path_inf[0].path_length == 0) // Died or Escaped instantly
            {
                if (path_inf[0].success) { // Escaped instantly
                    map[y][x].probability = 100.00;
                    map[y][x].mean_path_length = 0.00;
                    map[y][x].std_deviation = 0.00;
                }
                else { // Died instantly
                    map[y][x].probability = 0.00;
                    map[y][x].mean_path_length = 0.00;
                    map[y][x].std_deviation = 0.00; 
                }
            }
            else // Started on a Land (L)
            {
                success_rate = successful_walks * 100.0 / (double)NUMWALKS; // Calculate success rate as a percentage
                mean_path = total_path_length / (double)successful_walks;   // Calculate average path length

                double sq_diff_sum = 0; // Sum of squared differences from the mean
                
                // Itterate through every walk for a current cell and calculate the sum of squared distances
                for (int walk = 0; walk < NUMWALKS; walk++)
                {
                    if (path_inf[walk].success)
                    {
                        sq_diff_sum += pow((double)path_inf[walk].path_length - mean_path, 2);
                    }
                }

                // Calculate standard deviation
                std = sqrt(sq_diff_sum / (double)successful_walks);

                // Record all the data
                map[y][x].probability = success_rate;
                map[y][x].mean_path_length = mean_path;
                map[y][x].std_deviation = std;
            }
        }
    }


    // Display the computed map and statistics
    show_map(map);
    printf("\n");
    show_probability_of_escape(map);
    printf("\n");
    show_mean_path_length(map);
    printf("\n");
    show_std_of_path_length(map);

    return 0;
}

