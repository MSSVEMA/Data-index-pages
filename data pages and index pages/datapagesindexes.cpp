//Working 768 rows
/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct students//size 36
{
	int id;
	char name[32];
};

typedef struct datapages//size 128
{
	students student[3];
	char unused[20];
};

typedef struct indexpage//size 128
{
	int pageid[16];
	int keys[15];
	char unused[4];
};

typedef struct l2indexpage
{
	int indexid[16];
	int indexkey[15];
	char unused[4];

};

students *student;
datapages datapage;
indexpage index;
indexpage l2index;

void dump_datapages_to_file()
{
	FILE *fpr, *fpw;

	int i, j, k;
	char buffer[100];
	char *token;
	int no_of_records_per_datapage = 3;
	int no_of_datapages_per_indexpage = 16;
	int no_of_indexpages_per_L2indexpage = 16;
	fpr = fopen("input.txt", "r");
	student = (students *)malloc(sizeof(students) * (no_of_records_per_datapage));

	fpw = fopen("768bytes.bin", "wb");
	k = 0;
	while (k < no_of_indexpages_per_L2indexpage)
	{
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
				datapage.student[i] = student[i];
				i++;
			}
			fwrite(&datapage, sizeof(datapage), 1, fpw);
			j++;
		}
		k++;
	}

	fclose(fpr);
	fclose(fpw);
}

int dump_index_pages_to_file()
{
	FILE *fpr, *fpa;
	int i = 0, offset = 0, j;
	fpr = fopen("768bytes.bin", "rb");
	fpa = fopen("768bytes.bin", "ab");
	int start = 0, k = 0;
	int skip_first_datapage = 128;
	fseek(fpr, skip_first_datapage, SEEK_SET);
	//fread(&datapage, sizeof(datapage), 1, fpr);
	j = 0;
	while (j < 16)
	{
		i = 0;
		while (i < 15)//keys
		{
			fread(&datapage, sizeof(datapage), 1, fpr);
			index.keys[i] = datapage.student[0].id;
			index.pageid[i] = k * 128;
			k++;
			if (i==14)
			{
				index.pageid[i+1] = k * 128;
				k++;
			}
			i++;
		}
		fwrite(&index, sizeof(index), 1, fpa);
		j++;
	}



	offset = ftell(fpr);
	fclose(fpr);
	fclose(fpa);
	return offset;
}

void dump_L2_index_pages_to_file(int offset)
{
	FILE *fpr, *fpa;
	int i = 0;
	fpr = fopen("768bytes.bin", "rb");
	fpa = fopen("768bytes.bin", "ab");
	fseek(fpr, offset, SEEK_SET);
	char buffer[100];
	//int skip_datapage = 32769;
	//fseek(fpr, skip_datapage, SEEK_SET);
	//for (i = 0; i < 16; i++)
	//	fread(&datapage, sizeof(datapage), 1, fpr);

	//fscanf(fpr, "%s", buffer);
	i = 0;
	while (i < 15)
	{
		fread(&index, sizeof(index), 1, fpr);

		l2index.keys[i] = index.keys[0];
		i++;
	}
	for (i = 0; i < 16; i++)
	{
		l2index.pageid[i] = i;
	}
	fwrite(&l2index, sizeof(l2index), 1, fpa);
	fclose(fpa);
	fclose(fpr);
}

int main()
{
	int offset;
	dump_datapages_to_file();
	offset=dump_index_pages_to_file();
	dump_L2_index_pages_to_file(offset);

	system("pause");
	return 0;
}*/


//Working version of 16Dp and 1IP
/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"StringNumberUtilities.h"
typedef struct students//size 36
{
	int id;
	char name[32];
};

typedef struct datapages//size 128
{
	students student[3];
	char unused[20];
};

typedef struct indexpage//size 128
{
	int pageid[16];
	int keys[15];
	char unused[4];
};

students *student;
datapages datapage;
indexpage index;

void dump_datapages_to_file()
{
	FILE *fpr, *fpw;

	int i, j;
	char buffer[100];
	char *token;
	int no_of_records_per_datapage = 3;
	int no_of_datapages_per_indexpage = 16;
	fpr = fopen("dataset.txt", "r");
	student = (students *)malloc(sizeof(students) * (no_of_records_per_datapage));

	fpw = fopen("512bytes.bin", "wb");
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
			datapage.student[i] = student[i];
			i++;
		}
		fwrite(&datapage, sizeof(datapage), 1, fpw);
		j++;
	}
	fclose(fpr);
	fclose(fpw);
	
}

void dump_index_pages_to_file()
{
	FILE *fpr;
	int i = 0;
	fpr = fopen("512bytes.bin", "rb+");
	int skip_first_datapage = 128;
	fseek(fpr, skip_first_datapage, SEEK_SET);

	while (i < 16)
	{
		fread(&datapage, sizeof(datapage), 1, fpr);
		index.keys[i] = datapage.student[0].id;
		i++;
	}
	for (i = 0; i < 16; i++)
	{
		index.pageid[i] = i;
	}

	fwrite(&index, sizeof(index), 1, fpr);
	fclose(fpr);

}

void get_index()
{
	FILE *fpr;
	fpr = fopen("512bytes.bin", "rb");
	fseek(fpr, -128, SEEK_END);
	fread(&index, sizeof(index), 1, fpr);//get indexpage from last.
	fclose(fpr);
}

void get_range(int gt_value, int lt_value, int name_comparison_flag, char *name)
{
	int j, i = 0, offset = 0;
	FILE *fpr;
	int value;
	int start_i = 0, end_i = 0;
	fpr = fopen("512bytes.bin", "rb");

	i = 0;
	value = gt_value;
	while (i < 15)//since 15 keys
	{
		if (value >= index.keys[i])
			i++;
		else
		{
			start_i = i;
			break;
		}
	}
	value = lt_value;
	while (i < 15)//since 15 keys
	{
		if (value >= index.keys[i])
			i++;
		else
		{
			end_i = i;
			break;
		}
	}

	for (i = start_i; i <= end_i; i++)
	{
		offset = i * 128;
		fseek(fpr, offset, SEEK_SET);
		fread(&datapage, sizeof(datapage), 1, fpr);
		for (j = 0; j < 3; j++)
		{
			if (datapage.student[j].id > gt_value&&datapage.student[j].id < lt_value)
			{
				if (name_comparison_flag == 1)
				{
					if ((name != NULL) && (stringComparison(name, datapage.student[j].name) == 1))
						printf("%d %s\n", datapage.student[j].id, datapage.student[j].name);
				}
				else
				{
					printf("%d %s\n", datapage.student[j].id, datapage.student[j].name);
				}
			}
		}
	}

	fclose(fpr);
}

void query_processing(char *query)
{
	get_index();
	
	FILE *fpr;

	int i = 0, offset, j;
	char *keyword = (char *)malloc(sizeof(char) * 10);
	char *attribute = (char *)malloc(sizeof(char) * 10);
	char *value_str = (char *)malloc(sizeof(char) * 10);
	int value;

	getString(query, &i, ' ', keyword);
	getString(query, &i, ' ', attribute);
	getString(query, &i, ' ', value_str);
	value = getNumber(value_str, '\0');

	fpr = fopen("512bytes.bin", "rb");

	i = 0;
	while (i < 15)//since 15 keys
	{
		if (value >= index.keys[i])
			i++;
		else
		{
			offset = i * 128;
			fseek(fpr, offset, SEEK_SET);
			fread(&datapage, sizeof(datapage), 1, fpr);
			for(j = 0; j < 3; j++)
			{
				if (datapage.student[j].id == value)
				{
					printf("%s", datapage.student[j].name);
					break;
				}
			}
		}

	}


}

int main()
{
	dump_datapages_to_file();
	dump_index_pages_to_file();

	int gt_value = 0;
	int lt_value = 2;
	int name_comparison_flag = 0;
	char name[100] = "Lane\0";
	if (name_comparison_flag == 1)
		get_range(gt_value, lt_value, name_comparison_flag, name);
	else
		get_range(gt_value, lt_value, name_comparison_flag, NULL);
	char query[100];
	printf("Enter Query\n");
	scanf("%[^\n]s", query);
	query_processing(query);
	system("pause");
	return 0;
	}*/