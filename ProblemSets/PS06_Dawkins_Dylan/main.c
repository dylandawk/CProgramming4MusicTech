#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

/* useful to get program running, but may want to set to 0 after bugs are out */
#define db_flag     1

enum {
    MD_LINEAR = 1,
    MD_BINARY,
    MD_HASH
};

int main(int argc, char *argv[])
{
    int mode, i, j, k, n, num_entries, status;
    float avg_chain_len;
    char line[LINE_LEN], *s;
    char *ifile;
    FILE *ifp;
    struct PhoneBook phone_book[PB_LEN];

    /* parse command line
     *  Usage printout
     *  Determine mode
     *  Open phone book file
     */
    //Usage
    if(argc != 3){
        printf("ERROR %s 'linear' | 'binary' | 'hash' inputFile.csv \n", argv[0]);
        return -1;
    } 
    //Set Mode
    if(strcmp("linear", argv[1]) == 0){
        mode = MD_LINEAR;
    } else if (strcmp("binary", argv[1]) == 0){
        mode = MD_BINARY;
    } else if (strcmp("hash", argv[1]) == 0){
        mode = MD_HASH;
    } else {
        printf("ERROR: search string does not match 'linear', 'binary', or 'hash'");
        return - 1;
    }
    //Set and Open File
    ifile = argv[2];
    ifp = fopen(ifile, "r");
	if (ifp == NULL)
    {
            printf("ERROR: cannot open %s\n", ifile);
            return -1;
    }


    /* initialize phone book structure:
     * all strings have first character as zero (NULL)
     * chain value is set to -1
     */
    for (i=0; i<PB_LEN; i++) { 
        phone_book[i].first[0] = 0; 
        phone_book[i].last[0] = 0; 
        phone_book[i].phone[0] = 0; 
        phone_book[i].chain = -1;
    }

    /* read in phone book
     * use fgets(), strtok() and strncpy()
     */
    num_entries  = 0;
    struct PhoneBook *p = &phone_book[0];
    for(int i = 0; i < PB_LEN; i++){
        if(fgets(line, LINE_LEN, ifp) == NULL){
            break; //end of line
        }else {
            strcpy(p->last, strtok(line, ",\n"));
            strcpy(p->first, strtok(NULL, ",\n"));
            strcpy(p->phone, strtok(NULL, ",\n"));
            //printf("%s,%s,%s \n", p->last, p->first, p->phone);
            num_entries ++;
            p++;
        }
	}


    /* processing prior to search */
    switch (mode) {
        case MD_BINARY:
            /* binary search assumes list is sorted in alpha-numeric order
             * so sort phone book by last name
             */
            printf("Sorting phone book\n");
            qsort(phone_book, num_entries, sizeof(phone_book[0]), comp_last );
            break;
        case MD_HASH:
            printf("Building hash map table\n");
            create_hash_map(phone_book, num_entries);
            break;
    }

    while (1) {
        /* now ask for any last name */
        printf("Input any last name in phone book (0 to exit):\n");
        scanf("%s", line);
        if (line[0] == '0') {
            break; /* quit */
        }
        if (db_flag) printf("You entered:  %s\n", line);

        switch (mode) {
            case MD_LINEAR:
                /* linear search */
                i = linear_search(line, phone_book, num_entries);
                break;
            case MD_BINARY:
                /* binary search */
                i = binary_search(line, phone_book, num_entries);
                break;
            case MD_HASH:
                /* hashed search */
                i = hash_search(line, phone_book);
                break;
        }
        if (i < 0) {
            fprintf(stderr, " ERROR: %s not found\n", line);
        }
        else {
            printf("%s, %s - %s\n",
                phone_book[i].last,
                phone_book[i].first,
                phone_book[i].phone);
        }
    }
    return 0;
}

/* ordering function for qsort() */
int comp_last(const void * a, const void * b)
{
    int sortVal;
	struct PhoneBook *p1 = (struct PhoneBook *)a;
	struct PhoneBook *p2 = (struct PhoneBook *)b;
	return strncmp(p1->last, p2->last, NAME_LEN-1);
};

/*
 * linear search
 */
int linear_search(char *target, struct PhoneBook *phone_book, int num_entries)
{
    //Your code here
    int index = -1;
    for (int i = 0; i < num_entries; i++){
        if(strcmp(target, phone_book->last) == 0){
            index = i;
        }
        phone_book++;
    }
    return index;

}

/*
 * binary search
 */
int binary_search(char *target, struct PhoneBook *phone_book, int num_entries)
{
     /* binary search
     * low is low index in search interval
     * high is high index in search interval
     */
    int low, mid, high, status, n, i;
    low = 0;
    high = num_entries;
    mid = (low+high)/2;
    n = 0; //count loop iterations
    while (low <= high) {
        if(strcmp(target, phone_book[mid].last) == 0){
            return mid;
        } else if (strcmp(target, phone_book[mid].last) > 0){
            low = mid;
        } else if (strcmp(target, phone_book[mid].last) < 0){
            high = mid;
        }
        mid = (low+high)/2;
    }
    if (low > high) {
        fprintf(stderr, "ERROR: %s not found\n", target);
        return -1;
    }
    return i;
}

/* hash search */
int hash_search(char *target, struct PhoneBook *phone_book)
{
    //Compute hash of target
    int j = hash_func(target);
    //Lookup in hash array to get index
    int k = get_hash_map_value(j);

    //Trace chain to find target name
    while(1) {
        if(!strcmp(target, phone_book[k].last)) {
            return k;
        } else
        {
            k = phone_book[k].chain;
        }
        
    }
    return k;
}
