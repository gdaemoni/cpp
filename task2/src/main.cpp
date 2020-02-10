#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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

template<typename N>
N pow(N n, int power)
{
	if (power == 0)
		return (1);
	for (;power > 1; --power)
		n *= n;
	return (n);
}

struct coord
{
	int x;
	int y;
	int z;
};

struct	t_triangle
{
	coord top[3];
};

class 	pair_tr
{
public:
	t_triangle triangle[2];
	bool	is_similar()
	{
		double k = length(triangle[0].top[2], triangle[0].top[0]) / 
				length(triangle[1].top[2], triangle[1].top[0]);
		for(int i = 0; i < 2; ++i)
		{
			if (length(triangle[0].top[i], triangle[0].top[i + 1]) / 
				length(triangle[1].top[i], triangle[1].top[i + 1]) 
				!= k)
				return (false);
		}
		return (true);
	}
private:
	double		length(coord a, coord b)
	{
		return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2)));
	}
};

int 	number_of_digits(int n)
{
	int res = 1;
	if (n < 0)
	{
		res += 1;
		n *= -1;
	}
	while (n >= 10)
	{
		n /= 10;
		res++;
	}
	return(res);
}

void 	fill_coord(coord& top, const char *line, int& i)
{
	top.x = atoi(line + i++);
	i += number_of_digits(top.x);
	top.y = atoi(line + i++);
	i += number_of_digits(top.y);
	top.z = atoi(line + ++i);
	i += number_of_digits(top.z);
}

void	fill_pair_tr(pair_tr& pair, const char *line, size_t size)
{
	int n = 0, count = 0;
	for (int i = 0; i < size; ++i)
	{
		if (line[i] == 'A')
			fill_coord(pair.triangle[n].top[0], line, i += 4);
		if (line[i] == 'B')
			fill_coord(pair.triangle[n].top[1], line, i += 4);
		if (line[i] == 'C')
			fill_coord(pair.triangle[n].top[2], line, i += 4);
		if (line[i] == ']')
		{
			count++;
			if (count == 3)
				++n;
		}
	}
}

void	ReadFile(const char *path)
{
	ifstream	f(path);
	pair_tr		trngls;

	if(!f.is_open())
		error(FOPEN, path);
	while (!f.eof())
	{
		string 	line;
		getline(f, line, '\n');
		fill_pair_tr(trngls, line.c_str(), line.size());
		if (trngls.is_similar())
			cout << "These triangles are similar" << endl;
		else
			cout << "These triangles are not similar" << endl;
	}
	f.close();
}

int 	main(int argc, char**argv)
{
	if (argc != 2)
		error(ARGC, argc);
	ReadFile(argv[1]);
}
