#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50
#define MAX_LINE 1024
#define MAX_POINTS 15
#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct _student {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;
} Student;

int readNoRowsInFile()
{
	int counter = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* filePointer = NULL;
	filePointer = fopen("students.txt", "r");
	if (!filePointer) {
		printf("File not opened!\n");
		return FILE_ERROR_OPEN;
	}

	while (!feof(filePointer)) {
		fgets(buffer, MAX_LINE, filePointer);
		counter++;
	}

	fclose(filePointer);

	return counter;
}

int main()
{
	int i = 0, noRows = 0;
	noRows = readNoRowsInFile();

	if (noRows > 0)
	{
		FILE* filePointer = NULL;
		filePointer = fopen("students.txt", "r");
		if (!filePointer) {
			printf("File not opened!\n");
			return FILE_ERROR_OPEN;
		}

		Student* stud = NULL;
		stud = (Student*)malloc(noRows * sizeof(Student));
		if (stud == NULL) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}

		for (i = 0; i < noRows; i++) {
			if (fscanf(filePointer, " %s %s %lf ", stud[i].name, stud[i].surname, &stud[i].points) != 3)
				return SCANF_ERROR;
		}

		for (i = 0; i < noRows; i++) {
			printf("%s %s %.2lf %.2lf\%\n", stud[i].name, stud[i].surname, stud[i].points, stud[i].points / MAX_POINTS * 100);
		}

		fclose(filePointer);
		free(stud);
	}

	return EXIT_SUCCESS;
}
