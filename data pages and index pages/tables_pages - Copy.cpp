#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//page type 1- datapage 2-index page 3- table page
typedef struct students
{
	int id;
	char name[32];
};

typedef struct marks
{
	int student_id;
	int mark[4];
};

typedef struct Dp1
{
	
	students data[3];
	char page_type;
	char table_id;
	char unused[18];
};

typedef struct Dp2
{
	marks data[6];
	char page_type;
	char table_id;
	char unused[6];
};

typedef struct indexpage
{
	int pageid[16];
	int keys[15];
	char table_id;
	char unused[3];
};

typedef struct tablepage
{
	int rootpageid[10];
	char tablename[10][8];
	char page_type;
	char table_id;
	char unused[6];
};


students *student;
marks *m;
Dp1 dp1;
Dp2 dp2;
indexpage indexpg_students,indexpg_marks;
tablepage tablepg;

int offset1, offset2;

void dump_students_dp_to_file()
{
	FILE *fpr, *fpw;

	int i, j;
	char buffer[100];
	char *token;
	int no_of_records_per_datapage = 3;
	int no_of_datapages_per_indexpage = 16;
	fpr = fopen("students_data_48.txt", "r");
	student = (students *)malloc(sizeof(students) * (no_of_records_per_datapage));
	
	fpw = fopen("binary_48.bin", "wb");
	//fpw = fopen("binary_48.txt", "wb");
	
	j = 0;
	while (j < no_of_datapages_per_indexpage)
	{
		i = 0;
		while (i < no_of_records_per_datapage)
		{
			fscanf(fpr, "%s", buffer);
			token = strtok(buffer, ",");
			while (token != NULL)
			{
				student[i].id = atoi(token);
				token = strtok(NULL, ",");
				strcpy(student[i].name, token);
				token = strtok(NULL, ",");
			}
			dp1.data[i] = student[i];
			dp1.page_type = '1';
			dp1.table_id = '1';
			i++;
		}
		indexpg_students.pageid[j] = ftell(fpw);
		fwrite(&dp1, sizeof(dp1), 1, fpw);
		j++;
	}
	offset1 = ftell(fpw);
	fclose(fpr);
	fclose(fpw);
}

void dump_students_indexpage_to_file()
{
	FILE *fpr, *fpa;
	int i = 0, offset = 0, j;
	fpr = fopen("binary_48.bin", "rb");
	fpa = fopen("binary_48.bin", "ab");

//	fseek(fpr, -0, SEEK_END);
	//offset = ftell(fpr);
	//fpr = fopen("binary_48.txt", "rb");
	//fpa = fopen("binary_48.txt", "ab");

	int skip_first_datapage = 128;
	fseek(fpr, skip_first_datapage, SEEK_SET);

	//fread(&dp1, sizeof(dp1), 1, fpr);

	while (i < 16)
	{
		fread(&dp1, sizeof(dp1), 1, fpr);
		indexpg_students.keys[i] = dp1.data[0].id;
		i++;
	}
	indexpg_students.table_id = 1;

	fwrite(&indexpg_students, sizeof(indexpg_students), 1, fpa);
	fclose(fpr);
	fclose(fpa);
}

void dump_marks_dp_to_file()
{
	FILE *fpr, *fpw;

	int i, j;
	char buffer[100];
	char *token;
	int no_of_records_per_datapage = 6;
	int no_of_datapages_per_indexpage = 8;
	fpr = fopen("marks_data_48.csv", "r");
	marks *m = (marks *)malloc(sizeof(marks)*(no_of_records_per_datapage));
	
	fpw = fopen("binary_48.bin", "ab");
	//fpw = fopen("binary_48.txt", "ab");

	j = 0;
	fscanf(fpr, "%s", buffer);
	while (j < no_of_datapages_per_indexpage)
	{
		i = 0;
		
		while (i < no_of_records_per_datapage)
		{
			fscanf(fpr, "%s", buffer);
			token = strtok(buffer, ",");
			while (token != NULL)
			{
				m[i].student_id = atoi(token);
				token = strtok(NULL, ",");
				m[i].mark[0] = atoi(token);
				token = strtok(NULL, ",");
				m[i].mark[1] = atoi(token);
				token = strtok(NULL, ",");
				m[i].mark[2] = atoi(token);
				token = strtok(NULL, ",");
				m[i].mark[3] = atoi(token);
				token = strtok(NULL, ",");
			}
			dp2.data[i] = m[i];
			dp2.page_type = '1';
			dp2.table_id = '2';
			i++;
		}
		
		fwrite(&dp2, sizeof(dp2), 1, fpw);
		indexpg_marks.pageid[j] = ftell(fpw) - 128;
		j++;
	}
	offset2 = ftell(fpw);
	fclose(fpr);
	fclose(fpw);
}

void dump_marks_indexpage_to_file()
{
	
	FILE *fpr, *fpa;
	int i = 0, offset = 0, j;
	
	fpr = fopen("binary_48.bin", "rb");
	fpa = fopen("binary_48.bin", "ab");
	//fpr = fopen("binary_48.txt", "rb");
	//fpa = fopen("binary_48.txt", "ab");


	//int skip_first_datapage = 128;
	//fseek(fpr, skip_first_datapage, SEEK_SET);

	fread(&dp2, sizeof(dp2), 1, fpr);

	while (i < 16)
	{
		fread(&dp2, sizeof(dp2), 1, fpr);
		indexpg_marks.keys[i] = dp2.data[0].student_id;
		i++;
	}
	indexpg_marks.table_id = '2';

	fwrite(&indexpg_marks, sizeof(indexpg_marks), 1, fpa);
	fclose(fpr);
	fclose(fpa);
}
void dump_table_indexpage_to_file()
{
	tablepg.rootpageid[0] = offset1;
	tablepg.rootpageid[1] = offset2;

	strcpy(tablepg.tablename[0], "Students");
	strcpy(tablepg.tablename[1], "Marks");
	tablepg.page_type = '0';
	tablepg.table_id = '0';

	FILE *fpw = fopen("binary_48.bin", "ab");
	//FILE *fpw = fopen("binary_48.txt", "ab");
	fwrite(&tablepg, sizeof(tablepg), 1, fpw);
	fclose(fpw);
}

void sample()
{
	FILE *fpr;
	fpr = fopen("binary_48.bin", "rb");
	fseek(fpr, -128, SEEK_END);

	fread(&tablepg, sizeof(tablepg), 1, fpr);

	fseek(fpr, 3200, SEEK_END);

	fread(&indexpg_marks, sizeof(indexpg_marks), 1, fpr);
	
	fclose(fpr);
}

void join()
{
	FILE *fpr;
	fpr = fopen("binary_48.bin", "rb");

	int i = 0, j = 0, i1, j1, total;

	fseek(fpr, -128, SEEK_END);
	fread(&tablepg, sizeof(tablepg), 1, fpr);

	int students_index_start = tablepg.rootpageid[0];
	int marks_index_start = tablepg.rootpageid[1];

	fseek(fpr, students_index_start, SEEK_SET);
	fread(&indexpg_students, sizeof(indexpg_students), 1, fpr);

	fseek(fpr, marks_index_start, SEEK_SET);
	fread(&indexpg_marks, sizeof(indexpg_marks), 1, fpr);

	i1 = 0; j1 = 0;

	fseek(fpr, indexpg_students.pageid[i], SEEK_SET);
	fread(&dp1, sizeof(dp1), 1, fpr);

	fseek(fpr, indexpg_marks.pageid[j], SEEK_SET);
	fread(&dp2, sizeof(dp2), 1, fpr);

	while (i < 16 && j < 16)
	{
		while (i1 < 3 && j1 < 6 && i < 16 && j < 16)
		{
			if ((dp1.data[i1].id) < (dp2.data[j1].student_id))
				i1++;
			else if ((dp1.data[i1].id) > (dp2.data[j1].student_id))
				j1++;
			else
			{
				total = 0;
				for (int k = 0; k < 4; k++)
				{
					total = total + dp2.data[j1].mark[k];
				}

				printf("%d %d\n", dp1.data[i1].id, total);
				i1++; j1++;
			}

			if (i1 == 3)
			{
				i1 = 0;
				i++;
				fseek(fpr, indexpg_students.pageid[i], SEEK_SET);
				fread(&dp1, sizeof(dp1), 1, fpr);
			}
			if (j1 == 6)
			{
				j1 = 0;
				j++;
				fseek(fpr, indexpg_marks.pageid[j], SEEK_SET);
				fread(&dp2, sizeof(dp2), 1, fpr);
			}
		}
	}
}

int main()
{
	dump_students_dp_to_file();
	dump_students_indexpage_to_file();
	dump_marks_dp_to_file();
	dump_marks_indexpage_to_file();
	dump_table_indexpage_to_file();

	//sample();
	join();
	
	system("pause");
	return 0;
}