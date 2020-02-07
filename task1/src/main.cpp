#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum typeerror {
	FOPEN,
	ARGC
};

template<typename Message>
void	error(typeerror er, Message mess)
{
	switch (er)
	{
	case FOPEN:
		cout << "File open error " << mess << endl;
		break;
	case ARGC:
		cout << "Invalid number of arguments " << mess << endl;
		break;
	}
	exit(FOPEN);
}


template<typename Key>
void	qsort(vector<Key>& arr, const size_t low, const size_t high)
{
	size_t	plow = low;
	size_t	phigh = high;
	Key		elem = arr[low];

	while (plow < phigh)
	{
		while (phigh > plow && arr[phigh] >= elem)
			--phigh;
		if (phigh > plow)
		{
			arr[plow] = arr[phigh];
			while (plow <  phigh && arr[plow] <= elem)
				++plow;
			arr[phigh] = arr[plow];
		}
	}
	arr[plow] = elem;
	if ((plow - low) > 1)
		qsort<Key>(arr, low, plow - 1);
	if ((high - phigh) > 1)
		qsort<Key>(arr, phigh + 1, high);
}

int64_t	ReadFile(vector<int64_t>& numbers, const char *path)
{
	ifstream 	f;
	uint64_t	count;

	f.open(path);
	if (!f.is_open())
		error(FOPEN, path);
	while (!f.eof())
	{
		int64_t num = 0;
		f >> num;
		count += num;
		numbers.push_back(num);
	}
	f.close();
	return (count / numbers.size());
}


double	get_percentil(const vector<int64_t>& numbers, const float& val_perc)
{
	double		n = (numbers.size() - 1) * val_perc + 1;
	double		k = n - (uint64_t)n;
	int64_t		z = numbers[(size_t)n - 1];
	int64_t		z_next = numbers[(size_t)n];

	return (z + (k * (z_next - z)));
}

int		sum_digit(int64_t n)
{
	int sum = 0;

	while (n > 0)
	{
		sum += n % 10;
		n /= 10;
	}
	return (sum);
}

#define percentil 0.9

int		main(int argc, char**argv)
{
	vector<int64_t> numbers;
	int64_t			average;
	double			val_perc;
	size_t			res = 0;

	if (argc == 1 || argc > 2)
		error(ARGC, argc);
	average = ReadFile(numbers, argv[1]);
	qsort(numbers, 0, numbers.size() - 1);
	val_perc = get_percentil(numbers, percentil);
	for(size_t i = 0; i < numbers.size(); ++i)
		if (numbers[i] > average && numbers[i] < val_perc)
			res += sum_digit(numbers[i]);
	cout << res << endl;
}
