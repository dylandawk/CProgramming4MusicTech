#include <stdbool.h>
#include "maze.h"

bool solveMaze(int i, int j)
{
    int Ni = i-1; int Si = i+1; int Ei = i; int Wi = i;
    int Nj = j; int Sj = j; int Ej = j-1; int Wj = j+1;
    //position reached endpoint
    if(grid[i][j] == END_MARKER)
    {
        return true;
    }
    //position has been visited
    else if(grid[i][j] == VISITED_MARKER)
    {
        return false;
    }
    //position is 'illegal'
    else if(grid[i][j] == WALL_MARKER || i < 0 || i > (DIM_I -1) || j <0 || i>(DIM_J-1))
    {
        return false;
    }
    //position is 'legal'   
    else 
    {
        grid[i][j] = VISITED_MARKER;
        display();

        //recursion if true
        if (solveMaze(Ni, Nj) || solveMaze(Si, Sj) || solveMaze(Ei, Ej) || solveMaze(Wi, Wj))
        {
            grid[i][j] = SOLUTION_MARKER;
            display();
            return true;
        }
        //if false
        else
        {
            return false;
        }
    }
}