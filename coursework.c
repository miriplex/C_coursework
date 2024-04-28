// Add brief description here

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
// Enter any other #includes that you deem necessary below


#define NUMROWS	 9    // map size
#define NUMCOLS  9    // map size
#define NUMWALKS 1000 // number of random walks
#define NUMSTEP  10   // number of steps in each walk
// Enter any other #defines as you deem necessary below

struct path_info {
    bool success;
    int path_length;
};

struct cell {
    char letter;
    double mean_path_length;
    double probability;
    double std_deviation;
};

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


void show_map(struct cell map[][NUMCOLS])
{
    printf("Map:\n");
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" ");
            }
            printf("%c", map[y][x].letter);
        }
        printf("\n");
    }
}

void show_probability_of_escape(struct cell map[][NUMCOLS])
{
    printf("Probability of escape:\n");
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" ");
            }
            printf("%.2f", map[y][x].probability);
        }
        printf("\n");
    }
}

void show_mean_path_length(struct cell map[][NUMCOLS])
{
    printf("Mean path length:\n");
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" ");
            }
            printf("%.2f", map[y][x].mean_path_length);
        }
        printf("\n");
    }
}

void show_std_of_path_length(struct cell map[][NUMCOLS])
{
    printf("Standard deviation of path length:\n");
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            if (x != 0){
                printf(" ");
            }
            printf("%.2f", map[y][x].std_deviation);
        }
        printf("\n");
    }
}

struct path_info generate_a_random_path(struct cell map[][NUMCOLS], int starting_x, int starting_y){
    struct path_info path;
    int x = starting_x;
    int y = starting_y;
    int rand_num;
    int movement_direction_index;
    int status;

    for (int step = 0; step < NUMSTEP; step++)
    {
        status = calculate_status(map, x, y);

        if (status == 0){
            path.success = 0;
            path.path_length = step;
            return path;
        }
        else if (status == 1){
            path.success = 1;
            path.path_length = step;
            return path;
        }
        
        rand_num = rand();
        movement_direction_index = rand_num % 8;
        switch (movement_direction_index)
        {
            case 0:
                y++;
                break;

            case 1:
                y++;
                x++;
                break;
            
            case 2:
                x++;
                break;
            
            case 3:
                x++;
                y--;
                break;
            
            case 4:
                y--;
                break;
            
            case 5:
                x--;
                y--;
                break;
            
            case 6:
                x--;
                break;
            
            case 7:
                x--;
                y++;
                break;
    
            default:
                break;
        }
    
        x = (x < NUMCOLS) ? x : NUMCOLS;
        x = (x > 0) ? x : 0;
        y = (y < NUMROWS) ? y : NUMROWS;
        y = (y > 0) ? y : 0;

    }
    path.success = 0;
    path.path_length = 10;
    return path;
}


int main(void) {

    srand(123456);

    FILE *fptr;
    struct cell map[NUMROWS][NUMCOLS];

    struct path_info all_paths[NUMROWS][NUMCOLS][NUMWALKS];

    char single_space;

    fptr = fopen("island_map.txt", "r");

    if(fptr == NULL) { // display error if can't open file
        printf("Error. Not able to open the file.");
		return 1;
        }
        
    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            single_space = fscanf(fptr, "%c ", &map[y][x].letter);
        }
    }

    fclose(fptr);

    for (int y = 0; y < NUMROWS; y++)
    {
        for (int x = 0; x < NUMCOLS; x++)
        {
            double success_rate;
            double mean_path;
            double std;
            int successful_walks = 0;
            int total_path_length = 0;
            struct path_info path_inf;
            for (int walk = 0; walk < NUMWALKS; walk++)
            {
                path_inf = generate_a_random_path(map, x, y);
                all_paths[y][x][walk] = path_inf;
                if (path_inf.path_length == 0)
                {
                    break;
                }
                else if (path_inf.success == true)
                {
                    successful_walks++;
                    total_path_length += path_inf.path_length;
                }
            }
            if (all_paths[y][x][0].path_length == 0)
            {
                if (all_paths[y][x][0].success == true) {
                    map[y][x].probability = 100.00;
                    map[y][x].mean_path_length = 0.00;
                    map[y][x].std_deviation = 0.00;
                }
                else {
                    map[y][x].probability = 0.00;
                    map[y][x].mean_path_length = 0.00;
                    map[y][x].std_deviation = 0.00; 
                }
            }
            else 
            {
                success_rate = successful_walks / 10.0;
                mean_path = total_path_length / (double)successful_walks;

                double sq_diff_sum = 0;
                
                for (int walk = 0; walk < NUMWALKS; walk++)
                {
                    if (all_paths[y][x][walk].success = true)
                    {
                        sq_diff_sum += pow(all_paths[y][x][walk].path_length - mean_path, 2);
                    }
                }

                std = sqrt(sq_diff_sum / successful_walks);

                map[y][x].probability = success_rate;
                map[y][x].mean_path_length = mean_path;
                map[y][x].std_deviation = std;
            }
        }
    }

    show_map(map);
    printf("\n");
    show_probability_of_escape(map);
    printf("\n");
    show_mean_path_length(map);
    printf("\n");
    show_std_of_path_length(map);

    return 0;
}

