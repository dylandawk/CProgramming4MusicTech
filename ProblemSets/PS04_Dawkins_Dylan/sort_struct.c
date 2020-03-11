#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STUDENTS	100
#define LINE_LEN		80
#define NAME_LEN		32

typedef struct {
	char last[NAME_LEN];
	char first[NAME_LEN];
	int id;
} Student;

int comp_first(const void * a, const void * b)
{
	Student *pa = (Student *)a; 
	Student *pb = (Student *)b;
	const char *l = pa->first;
	const char *r = pb->first;
	return strcmp(l,r);
};

int comp_last(const void * a, const void * b)
{
	Student *pa = (Student *)a; 
	Student *pb = (Student *)b;
	const char *l = pa->last;
	const char *r = pb->last;
	return strcmp(l,r);
};

int comp_id(const void * a, const void * b)
{

	Student *pa = (Student *)a; 
	Student *pb = (Student *)b;
	const int l = pa->id;
	const int r = pb->id;
	return (l-r);

};

int main(int argc, char *argv[])
{
	int num_students;
	char *sort_key;
	char *ifile, *ofile, line[LINE_LEN];
	FILE *ifp, *ofp;
	Student sdata[MAX_STUDENTS];

	//Usage Error
	if(argc < 4){
        printf("ERROR %s 'first' | 'last' | 'id' inputFile.csv outputFile.csv \n", argv[0]);
        return -1;
    }
	

	//Assign arguments
	sort_key = argv[1];
	ifile = argv[2];
	
	//Open files
	ifp = fopen(ifile, "r");
	if (ifp == NULL)
    {
            printf("ERROR: cannot open %s\n", ifile);
            return -1;
    }

	ofile = argv[3];
	ofp = fopen(ofile, "w");
	if (ofp == NULL)
    {
            printf("ERROR: cannot open %s\n", ofile);
            return -1;
    }	

	//Parse file into sdata array
	num_students = 0;
	Student *p = &sdata[0];
	for(int i = 0; i < MAX_STUDENTS; i++){
		if(fgets(line, LINE_LEN, ifp) == NULL){
			break; //end of line
		}else {
			strncpy(p->last, strtok(line, ",\n"), NAME_LEN);
			strncpy(p->first, strtok(NULL, ",\n"), NAME_LEN);
			p->id = atoi(strtok(NULL, ",\n"));
			num_students ++;
			p++;
		}
	}

	// //print structure contents (Disabled)
	// for (int i=0; i<num_students; i++) {
	// 	printf("%4d %s %s %6d\n", i, sdata[i].first, sdata[i].last, sdata[i].id);
	// }
	// printf("Read records for %d students\n", num_students);

	/* use quicksort to sort student array 
	 * use different calls to qsort depending on sort_key value
	 */
	if (strcmp(sort_key, "first") == 0) {
		/* sort on first name: comp_first is comparison function */
		qsort(&sdata[0], num_students, sizeof(sdata[0]), comp_first );
	}
	else if (strcmp(sort_key, "last") == 0) {
		/* sort on last name: comp_last is comparison function */
		qsort(&sdata[0], num_students, sizeof(sdata[0]), comp_last );
	}
	else if (strcmp(sort_key, "id") == 0) {
		/* sort on student id: comp_id is comparison function */
		qsort(&sdata[0], num_students, sizeof(sdata[0]), comp_id );
	}
	else {
		fprintf(stderr, "Unknown sort key: %s\n", sort_key);
		return -1;
	}

	/* write sorted student array to output file */
	for(int i = 0; i < num_students; i++){
		fprintf(ofp, "%s,%s,%d\n", sdata[i].last, sdata[i].first, sdata[i].id);
	}

	fclose(ifp);
	fclose(ofp);
	return (0);

}
