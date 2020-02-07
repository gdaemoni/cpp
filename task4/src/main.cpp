#include <iostream>

using namespace std;

bool SimilarityStr(char *first, char *second)
{
	if (*first == '\0' && *second == '\0')
		return (true);
	if (*first != '\0' && *second == '*')
		return (SimilarityStr(first + 1, second) || SimilarityStr(first, second + 1));
	if (*first == '\0' && *second == '*')
		return (SimilarityStr(first, second + 1));
	if (*first == *second)
		return (SimilarityStr(first + 1, second + 1));
	return (false);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Invalid number of arguments" << endl;
		exit(0);
	}
	if (SimilarityStr(argv[1], argv[2]))
		cout << "OK" << endl;
	else
		cout << "KO" << endl;
	return (1);
}