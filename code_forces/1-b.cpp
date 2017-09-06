// 1-b.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <stdio.h>
#include <string>

using namespace std;

char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char NUMBER[] = "1234567890";

bool is_alpha(char ch)
{
	int len = strlen(ALPHABET);
	for (int cnt = 0; cnt < len; cnt++)
		if (ALPHABET[cnt] == ch)
			return true;
	return false;
}

bool is_num(char ch)
{
	int len = strlen(NUMBER);
	for (int cnt = 0; cnt < len; cnt++)
		if (NUMBER[cnt] == ch)
			return true;
	return false;
}

void conver_alpha_to_num(char* alpha_str, char* num_str)
{
	int alpha_len = strlen(alpha_str);
	int num_digit = 0;
	long long num = 0;
	for (int cnt = alpha_len - 1; cnt >= 0; cnt--)
	{
		num += pow(26, num_digit) * (alpha_str[cnt] - 'A' + 1);
		num_digit++;
	}

	sprintf(num_str, "%lld", num);
}

void conver_num_to_alpha(char* num_str, char* alpha_str)
{
	long long num = atoi(num_str);
	int num_len = strlen(num_str);

	int num_q = num;
	int num_m = 0;

	char temp_alpha[32];
	int temp_alpha_cnt = 0;
	while (num_q)
	{
		num_m = num_q % 26 - 1 ? num_q % 26 - 1 : 26;
		num_q = num_q / 26;
		temp_alpha[temp_alpha_cnt++] = ALPHABET[num_m];
	}

	for (int cnt = 0; cnt < temp_alpha_cnt; cnt++)
	{
		alpha_str[cnt] = temp_alpha[temp_alpha_cnt - cnt - 1];
	}
}

void convert(char* cell_str)
{
	int cell_len = strlen(cell_str);

	// get type
	char cell_token[4][32];
	int cell_token_len[4];
	int cell_token_cnt = 0;
	memset(cell_token, 0, sizeof(cell_token));
	memset(cell_token_len, 0, sizeof(cell_token_len));

	int cell_token_prev = 0;
	for (int cnt = 0; cnt < cell_len; cnt++)
	{
		if (is_alpha(cell_str[cnt]))
		{
			if (cell_token_prev == 0)
				cell_token_prev = 'a';

			if (cell_token_prev != 'a')
			{
				cell_token_prev = 'a';
				cell_token_cnt++;
			}
		}
		else
		{
			if (cell_token_prev == 0)
				cell_token_prev = 'b';

			if (cell_token_prev != 'b')
			{
				cell_token_prev = 'b';
				cell_token_cnt++;
			}
		}

		memcpy(&cell_token[cell_token_cnt][cell_token_len[cell_token_cnt]++], &cell_str[cnt], 1);
	}
	cell_token_cnt++;

	if (cell_token_cnt == 4)
	{
		char alpha_str[32];
		memset(alpha_str, 0, sizeof(alpha_str));
		conver_num_to_alpha(cell_token[3], alpha_str);
		printf("%s%s", alpha_str, cell_token[1]);
	}
	else
	{
		char num_str[32];
		conver_alpha_to_num(cell_token[0], num_str);
		printf("R%sC%s", cell_token[1], num_str);
	}
}

int main()
{
	int lines;
	scanf("%lld", &lines);

	for (int cnt = 0; cnt < lines; cnt++)
	{
		char cell_str[32];
		scanf("%s", cell_str);
		convert(cell_str);
	}

    return 0;
}
