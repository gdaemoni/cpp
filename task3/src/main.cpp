#include <iostream>
#include <fstream>
#include <string>
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
		cout << "Usage:" << endl;
		cout << "program_name file.log" << endl;
		cout << "program_name file.log start_date end_date" << endl;
		break;
	}
	exit(FOPEN);
}

struct t_data
{
	long	error_add;
	long	error_drain;
	long	try_to_add;
	long	try_to_drain;
	long	v_add_water;
	long	v_fail_add_water;
	long	v_fail_drain_water;
	long	v_drain_water;
	long	v_begin;
	long	v_end;
};

struct t_try
{
	long V;
	bool move;
};

class Barrel
{
	long		V_max;
	long		V_actual;
public:
	long	get_actual()
	{
		return (V_actual);
	}
	bool	add_water(long n)
	{
		if (V_max < V_actual + n)
			return false;
		V_actual += n;
		return true;
	}
	bool	drain_water(long n)
	{
		if (V_actual - n < 0)
			return false;
		V_actual -= n;
		return true;
	}
	void	set_actual(long n)
	{
		V_actual = n;
	}
	void 	set_max(long n)
	{
		V_max = n;
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

double 	process_time(string& time)
{
	int c = 0;
	double res = 0;
	for (int i = 0; i < time.size(); ++i)
	{
		if (time[i] >= '0' && time[i] <= '9')
		{
			long n = atoi(time.c_str() + i);
			i += number_of_digits(n);
			if (c == 0)
				res += n * 31536000;
			else if (c == 1)
				res += n * 2592000;
			else if (c == 2)
				res += n * 86400;
			else if (c == 3)
				res += n * 3600;
			else if (c == 4)
				res += n * 60;
			else if (c == 5)
				res += n;
			else 
				res += double(n / pow(10, number_of_digits(n)));
			c++;
		}
	}
	return (res);
}

class Manager
{
	Barrel	b;
	t_data 	data;
	double	start;
	double	end;

public:

	Manager()
	{
		start = 0;
		end = 0;
		data = {0,0,0,0,0,0,0,0,-1,0};
	}
	void set_total()
	{
		if (data.try_to_add)
			data.error_add = (100 * data.error_add) / data.try_to_add;
		if (data.try_to_drain)
			data.error_drain = (100 * data.error_drain) / data.try_to_drain;
		if (data.v_begin == -1)
			data.v_begin = 0;
	}
	void set_data(double time, t_try move)
	{
		bool fl = move.move ? b.add_water(move.V) : b.drain_water(move.V);
		if ((!start && !end) || (time >= start && time <= end))
		{
			if (data.v_begin < 0)
				data.v_begin = b.get_actual() - (fl ? move.V : 0);
			move.move ? data.try_to_add++ : data.try_to_drain++;
			if (fl)
				(move.move) ? (data.v_add_water += move.V) : (data.v_drain_water += move.V);
			else if (move.move)
			{
				data.v_fail_add_water += move.V;
				data.error_add++;
			}
			else if (!move.move)
			{
				data.v_fail_drain_water += move.V; 
				data.error_drain++;
			}
			data.v_end = b.get_actual();
		}
	}
	
	void set_max(long n)
	{
		b.set_max(n);
	}
	void set_actual(long n)
	{
		b.set_actual(n);
	}
	void set_satr_end(char *s, char *e)
	{
		string st(s);
		string en(e);
		start = process_time(st);
		end = process_time(en);
	}
	void print_answer()
	{
		cout << "For the indicated period:" << endl;
		cout << "There were " << data.try_to_add << " attempts to pour water into the barrel" << endl;
		cout << data.error_add << "% of mistakes" << endl;
		cout << data.v_add_water << "L were poured" << endl;
		cout << data.v_fail_add_water << "L were not poured" << endl;
		cout << "There were " << data.try_to_drain << " attempts to drain water into the barrel" << endl;
		cout << data.error_drain << "% of mistakes" << endl;
		cout << data.v_drain_water << "L were drained" << endl;
		cout << data.v_fail_drain_water << "L were not drained" << endl;
		cout << data.v_begin << "L begin" << endl;
		cout << data.v_end << "L end" << endl;
	}
};

t_try 	process_line(string line)
{
	t_try res;
	if (line[1] == 't')
		res.move = 1;
	else
		res.move = 0;
	res.V = atoi(line.c_str() + 7);
	return res;
}

void	ReadFile(Manager& m, const char *path)
{
	ifstream f(path);
	long	meta = 0, n_line = 0;
	string	line, time;

	if (!f.is_open())
		error(FOPEN, path);
	while (!f.eof())
	{
		if (n_line == 0)
			getline(f, line, '\n');
		if (n_line == 1) {
			f >> meta;
			m.set_max(meta);
		}
		if (n_line == 2) {
			f >> meta;
			m.set_actual(meta);
		}
		if (n_line > 2) {
			f >> time;
			getline(f, line, '\n');
			m.set_data(process_time(time), process_line(line));
		}
		n_line++;
	}
	m.set_total();
}

int 	main(int argc, char **argv)
{
	Manager m;
	if (argc < 2 || argc == 3 || argc > 4)
		error(ARGC, argc);
	if (argc == 4)
	{
		m.set_satr_end(argv[2], argv[3]);
		ReadFile(m, argv[1]);
	}
	else
		ReadFile(m, argv[1]);
	m.print_answer();
}
