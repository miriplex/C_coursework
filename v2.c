//Patricia Bertran de Lis
//In this program, several random walks will be taken each with a maxumum number of 10 steps
//For each cell, we will be calculating the probability of escape, the mean path length
//and the standard deviation of the path length
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define NUMROWS	 9    // number of rows
#define NUMCOLS  9    // number of columns
#define NUMWALKS 1000 // number of random walks
#define NUMSTEP  10   //  max number of steps in each walk


// The struct is used to group all the necesary information about a coordinate
struct coordinate {
    char letter;  // These can be either B, L, V, D or W
    double ProbabilityOfEscape;  //variable to store the probability of escape
    double meanPathLength;   //variable to store the mean path length
    double std_deviation;   //variable to store the standard deviation
};


struct InformationOfPath {  // Way of grouping all the necessary data of a path
    bool success;   // is it succesful or is it not
    int PathLength;  // place to store the length of the path
    bool error;
};



int CurrentState(struct coordinate island[][NUMCOLS], int x, int y) {  //function to find the state of a coordinate
    char currentObject = island[y][x].letter;   
        // 4 options
    if (currentObject == 'D' || currentObject == 'V' || currentObject == 'W')
        return 0;    //option 1
    else if (currentObject == 'B')
        return 1;    //option 2
    else if (currentObject == 'L')
        return 2;    //option 3
    else {
        printf ("Error!");
	return 3;
    }

}


// Function that takes a random walk and returns the information of the path
struct InformationOfPath CreateRandomPath(struct coordinate island[][NUMCOLS], int starting_x, int starting_y){
    struct InformationOfPath path = {0}; // Initialize path data
    int x = starting_x;                  // Current x coordinate
    int y = starting_y;                  // Current y coordinate
    int rand_num;                        // Variable to store a random number
    int randomDirection;                 // Index for random direction
    int status = CurrentState(island, x, y); // Check the initial cell

    // Iterate through all walks
    for (int step = 0; step < NUMSTEP; step++) {
        if (status != 2) { // Walk needs to terminate
            path.success = (status == 1);
            path.PathLength = step;
            path.error = (status == 3);
            return path;
        }

        // Generate a random direction
        rand_num = rand();
        randomDirection = rand_num % 8;

        // Perform a movement
        switch (randomDirection) {
            case 0: 
            y--; 
            break;           // N
            case 1: 
            y--; 
            x++; 
            break;      // NE
            case 2: 
            x++; 
            break;           // E
            case 3: 
            x++; 
            y++; 
            break;      // SE
            case 4: 
            y++; 
            break;           // S
            case 5: 
            x--; 
            y++; 
            break;      // SW
            case 6: 
            x--; 
            break;           // W
            case 7: 
            x--; 
            y--; 
            break;      // NW
        }

        // keep x and y within map's boundaries
        x = (x < 0) ? 0 : (x >= NUMCOLS) ? NUMCOLS - 1 : x;
        y = (y < 0) ? 0 : (y >= NUMROWS) ? NUMROWS - 1 : y;

        // Update the status
        status = CurrentState(island, x, y);
    }

    // Get final position values
    path.PathLength = NUMSTEP;
    path.success = (status == 1);
    path.error = (status == 3);
    
    return path;
}


int main(void) {

    srand(123456); 

    FILE *fp;
    struct coordinate island[NUMROWS][NUMCOLS];  //calls the struct
    char StoreLetter;    //place to store the letter read from the file
    fp = fopen("island_map.txt", "r"); // opens the file where the island map is stored
    if(fp == NULL) { // If the file doean't open or exist
        printf("Error!"); //display an error on the screen 
		return 1;
        }
    for (int y = 0; y < NUMROWS; y++) // goes through all the rows to read the characters of the island
    {
        for (int x = 0; x < NUMCOLS; x++)      // goes through all the columns to read the characters of the island
        {
            StoreLetter = fscanf(fp, "%c ", &island[y][x].letter); // Read a single character for each cell   //does it sav it in letter
        }
    }
    fclose(fp);  //closes the file


     // Perform random walks on each cell
    for (int y = 0; y < NUMROWS; y++) {
        for (int x = 0; x < NUMCOLS; x++) {
            int wins = 0;
            int totalPathLength = 0;
            int walksPerformed = 0;
            struct InformationOfPath InfoPath[NUMWALKS];    // An Array to store path information for every walk for a selected cell


            for (int walk = 0; walk < NUMWALKS; walk++) {
                struct InformationOfPath path = CreateRandomPath(island, x, y);

                if (path.error) {
                    return 1;
                }

                if (path.success) {
                    wins++;
                    totalPathLength += path.PathLength;
                }

                if (path.PathLength == 0) // Path ended instantly
                    break;
                
                InfoPath[walk] = path;
                walksPerformed++;
            }

            // Calculate probabilities and statistics only if any walks were performed
            if (walksPerformed > 0) {
                double successRate = (double)wins / walksPerformed * 100.0;
                double meanPathLength = (wins > 0) ? ((double)totalPathLength / wins) : 0.0;
                double stdDeviation = 0.0;

                if (wins > 0) {
                    double sumOfSquares = 0.0;
                    for (int walk = 0; walk < NUMWALKS; walk++) {
                        if (InfoPath[walk].success)
                            sumOfSquares += pow((double)InfoPath[walk].PathLength - meanPathLength, 2);
                    }
                    stdDeviation = sqrt(sumOfSquares / wins);
                }

                island[y][x].ProbabilityOfEscape = successRate;
                island[y][x].meanPathLength = meanPathLength;
                island[y][x].std_deviation = stdDeviation;
            }
        }
    }
        
    //Display of all maps
    printf("Island:\n");
    for (int y = 0; y < NUMROWS; y++)   //goes through every row
    {
        for (int x = 0; x < NUMCOLS; x++)  //goes through every column
        {
            if (x != NUMCOLS - 1){   
                printf("%c ", island[y][x].letter); // prints the character and space
            }
            else {
            printf("%c", island[y][x].letter); // prints the character
            }
        }
        printf("\n"); // skip a line between rows
    }
    printf("\n");
    printf("Probability of escape:\n");
    for (int y = 0; y < NUMROWS; y++) //goes through every row
    {
        for (int x = 0; x < NUMCOLS; x++)  //goes through every column
        {
            if (x != NUMCOLS - 1){
                printf("%.2f ", island[y][x].ProbabilityOfEscape); // Prints probability to 2 decimal places and space
            }
            else {
                 printf("%.2f", island[y][x].ProbabilityOfEscape); // Prints probability to 2 decimal places
            }
        }
        printf("\n"); //skip a line between rows
    }
    printf("\n");
    printf("Mean path length:\n");
    for (int y = 0; y < NUMROWS; y++)  //goes through every row
    {
        for (int x = 0; x < NUMCOLS; x++) //goes through every column
        {
            if (x != NUMCOLS - 1){
                printf("%.2f ", island[y][x].meanPathLength); // Prints mean path length to 2 decimal places and space
            }
            else {
                 printf("%.2f", island[y][x].meanPathLength); // Prints mean path length to 2 decimal places
            }
        }
        printf("\n"); //skip a line between rows
    }
    printf("\n");
    printf("Standard deviation of path length:\n");
    for (int y = 0; y < NUMROWS; y++)  //goes through every row
    {
        for (int x = 0; x < NUMCOLS; x++)  //goes through every column
        {
            if (x != NUMCOLS - 1){
                printf("%.2f ", island[y][x].std_deviation); // Prints standard deviation to 2 decimal places and space
            }
            else {
                printf("%.2f", island[y][x].std_deviation); // Prints standard deviation to 2 decimal places
            }
        }
        printf("\n"); //skip a line between rows
    }

    return 0;
}
