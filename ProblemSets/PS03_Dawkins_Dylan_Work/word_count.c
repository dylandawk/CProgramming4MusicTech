#include <stdio.h> 
#include <string.h> 
#define LINE_LEN 80

int main(int argc, char *argv[])
{
    int count;
    char *ifile, line[LINE_LEN],  *word;
    FILE *fp;

    //Usage error
    if(argc < 2){
        printf("ERROR %s inputfile.txt \n", argv[0]);
        return -1;
    }
    ifile = argv[1];
    fp = fopen(ifile, "r");
    if (fp == NULL)
    {
            printf("ERROR: cannot open %s\n", ifile);
            return -1;
    }
    count = 0;

    while (fgets(line, LINE_LEN, fp) != NULL) {
        char *word = strtok(line, " \t\n");
        while(word != NULL){
            word = strtok(NULL, " \t\n");
            count++;
        }
    }
    printf("Word count is: %d\n", count);
}