#include <stdio.h>
#include <stdlib.h>


#define ALIG_1 30
#define ALIG_2 0
#define ALIG_3 0
#define ALIG_4 0
#define ALIG_5 0
#define ALIG_6 0
#define ALIG_7 40
#define stream_ WriteF
#define addout_ "_out"

long _stdcall ReadLine(FILE* ReadFile, char* str, long size)//返回实际读取字节数，如果读取失败返回-1
{
	long i = 0;
	while (--size && !feof(ReadFile) && (str[i] = fgetc(ReadFile)) != '\n')
		++i;

	if (str[i] == '\n')
		str[i] = 0;
	else
		return -1;

	return i + 1;
}

bool _stdcall JumpLine(FILE* ReadFile,long jumpcount)
{
	for (int i = 0; i < jumpcount; ++i)
	{
		while (!feof(ReadFile) && fgetc(ReadFile) != '\n')
			continue;
		if (feof(ReadFile))
			return false;
	}
	return true;
}

long _stdcall ReadUntilExChar(const char* str, char c, char* ostr, long size)
{
	int i = 0, j = 0;
	while (--size && str[i] && (ostr[i] = str[i]) != c)
		++i;
		
	if (ostr[i] == c)
		ostr[i] = 0;
	else
		return -1;

	return i + 1;
}

long _stdcall ReadExclusivSpace(const char* str, char* ostr, long size)
{
	int i = 0, j = 0;
	while (--size && str[i])
	{
		if (str[i] != ' ')
			ostr[j++] = str[i];
		++i;
	}
	if (size)
		ostr[j] = 0;
	else
		return -1;
	return j;
}

long _stdcall strlen(const char* str)
{
	long i = 0;
	while (str[i++]) continue;
	return i;
}

long _stdcall strcpy(char* str1, const char* str2)
{
	int i = 0;
	while (str1[i] = str2[i]) ++i;
	return i;
}

typedef struct level
{
	long large_chests_shulker_boxes;
	long small_chest_shulker_boxes;
	long shulker_boxes;
	long sets_of_items;
	long items;
}LEVEL, * PLEVEL;

LEVEL CalculateLevels(long long number)
{
	LEVEL level = { 0 };

	if (number >= 93312)
	{
		level.large_chests_shulker_boxes = (number / 93312);
	}

	if (number >= 46656)
	{
		level.small_chest_shulker_boxes = (number / 46656) % 2;
	}

	if (number >= 1728)
	{
		level.shulker_boxes = (number / 1728) % 27;
	}

	if (number >= 64)
	{
		level.sets_of_items = (number / 64) % 27;
	}

	level.items = number % 64;

end:
	return level;
}


int main(int argc, char* argv[])
{
	char line[128], temp[78], * temptr;
	long ret;
	long long ltmp;
	LEVEL level[2], * plevel;
	bool print = false;
	FILE* ReadF, * WriteF;

	if (argc != 2)
		return -1;

	
	{
		int len = strlen(argv[1]);
		char* p = (char*)malloc(len + sizeof(addout_) - 1);
		len = strcpy(p, argv[1]);
		strcpy(p + len - 4, addout_".txt");

		ReadF = fopen(argv[1], "r");
		WriteF = fopen(p, "w");
		free(p);
	}
	
	if (!ReadF || !WriteF)
	{
		fclose(ReadF);
		fclose(WriteF);
		return -2;
	}
		

	if(JumpLine(ReadF, 1))
		fprintf(stream_,"%s\n", "+-----------+-----------+-----------+-----------+");
	if (~ReadLine(ReadF, line, 128) && *line == '|')
		fprintf(stream_, "%s\n", line);
	if (JumpLine(ReadF, 1))
		fprintf(stream_, "%s\n", "+-----------+-----------+-----------+-----------+");
	JumpLine(ReadF, 2);
	fprintf(stream_, "| %-*s| %-*s| %-*s|\n\n", ALIG_1, "物品名", ALIG_7 - 1, "总物品个数", ALIG_7 - 1, "缺少的物品个数");
		
	
	while (~ReadLine(ReadF, line, 128) && *line != '+')
	{
		temptr = line + 1;

		for (int i = 0; i < 4; ++i)
		{
			if (~(ret = ReadUntilExChar(temptr, '|', temp, 78)))
				temptr += ret;
			else
				return -2;
			if (~ReadExclusivSpace(temp, temp, 78))
			{
				if (i == 1)
					level[0] = CalculateLevels(atoll(temp));
				else if (i == 2)
					ltmp = atoll(temp);
				else if (i == 3)
					level[1] = CalculateLevels(ltmp - atoll(temp));
				else
					fprintf(stream_, "| %-*s|", ALIG_1, temp);
			}
		}

		for (int i = 0; i < 2; ++i)
		{
			plevel = &level[i];
			temptr = line;

			if (plevel->large_chests_shulker_boxes)
			{
				//if (print)
					//*temptr++ = '+';
				temptr += sprintf(temptr, " %*d大箱盒 ", ALIG_2, plevel->large_chests_shulker_boxes);
				print = true;
			}
			if (plevel->small_chest_shulker_boxes)
			{
				if (print)
					*temptr++ = '+';
				temptr += sprintf(temptr, " %*d小箱盒 ", ALIG_3, plevel->small_chest_shulker_boxes);
				print = true;
			}
			if (plevel->shulker_boxes)
			{
				if (print)
					*temptr++ = '+';
				temptr += sprintf(temptr, " %*d潜影盒 ", ALIG_4, plevel->shulker_boxes);
				print = true;
			}
			if (plevel->sets_of_items)
			{
				if (print)
					*temptr++ = '+';
				temptr += sprintf(temptr, " %*d组 ", ALIG_5, plevel->sets_of_items);
				print = true;
			}
			if (plevel->items)
			{
				if (print)
					*temptr++ = '+';
				temptr += sprintf(temptr, " %*d个 ", ALIG_6, plevel->items);
				print = true;
			}

			if (!print)
				sprintf(temptr, " 0个 ");

			print = false;

			fprintf(stream_, "%-*s|", ALIG_7, line);
		}
		fprintf(stream_,"\n");
	}
		
	fprintf(stream_, "%s\n", "+-----------+-----------+-----------+-----------+");

	fclose(ReadF);
	fclose(WriteF);

	return 0;
}

