#include <iostream>
#include <string.h>


void preKMP(char* pattern, int f[])
{
	int m = strlen(pattern), k;
	f[0] = -1;
	for (int i = 1; i < m; i++)
	{
		k = f[i - 1];
		while (k >= 0)
		{
			if (pattern[k] == pattern[i - 1])
				break;
			else
				k = f[k];
		}
		f[i] = k + 1;
	}
}

//check whether target string contains pattern 
bool KMP(char* pattern, char* target)
{
	int m = strlen(pattern);
	int n = strlen(target);
	int* f = new int[m];
	preKMP(pattern, f);
	int i = 0;
	int k = 0;
	while (i < n)
	{
		if (k == -1)
		{
			i++;
			k = 0;
		}
		else if (target[i] == pattern[k])
		{
			i++;
			k++;
			if (k == m)
				return 1;
		}
		else
			k = f[k];
	}
	return 0;
}

int main()
{
	char tar[] = "abbaba";
	char *tar2 = new char[strlen(tar) * 2 - 1];
		int j = 0;
	for (int i = 0; i < strlen(tar); i++)
	{
		tar2[j] = tar[i];
		j++;
	}
	for (int i = 0; i < strlen(tar); i++)
	{
		tar2[j] = tar[i];
		j++;
	}
	for (; j < strlen(tar2) - 1; j++)
		tar[j] = ' ';
	char pat[] = "abaabb";
	if (KMP(pat, tar2))
	{
		printf("%s", pat);
		printf("%s", "' found in string '");
		printf("%s", tar);
	}
	else
	{
		printf("%s", pat);
		printf("%s", "' not found in string '");
		printf("%s", tar);
	}
	
	system("pause");
	return 0;
}