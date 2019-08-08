#include "Diskstats.h"

#include <cstring>
#include <iostream>

using namespace std;
// ----- Constructor / Destructor -----

Diskstats::Diskstats(const string &file, const bool read) 
	: diskstatsFile(file), mapStats()
{
	if(read)
		readFile();
}

Diskstats::~Diskstats()
{
	unordered_map<string, long *>::iterator it;
	for(it = mapStats.begin(); it != mapStats.end(); ++it)
	{
		delete [] it->second;
	}
}

// --- public methods ---

void Diskstats::readFile()
{
	ifstream file(diskstatsFile);
	if(file)
	{
		string tmp;
		getline(file, tmp);
		do
		{
			//cout << tmp << endl;
			readLine(tmp);	
			getline(file, tmp);
		}while(!file.eof());
	}
	file.close();
}

void Diskstats::display(ostream& out) const
{
	unordered_map<string, long *>::const_iterator it;
	for(it = mapStats.cbegin(); it != mapStats.cend(); ++it)
	{
		writeDeviceInfo(out, it->first, it->second);
		out << endl << endl;
	}	
}

void Diskstats::substract(	const Diskstats &latest, 
				const Diskstats &earliest)
{
	unordered_map<string, long *>::const_iterator it_last;
	unordered_map<string, long *>::const_iterator it_early;

	unordered_map<string, long *> latest_map = latest.mapStats;
	unordered_map<string, long *> early_map = earliest.mapStats;
	
	for(	it_last = latest_map.cbegin(); 
		it_last != latest_map.cend(); 
		++it_last)
	{
		it_early = early_map.find(it_last->first);
		long * values = new long[13];

		long * last_values = it_last->second;
		long * early_values = it_early->second;
		
		for(int i = 0; i < 13; ++i)
		{
			values[i] = last_values[i] - early_values[i];
		}

		mapStats.insert(make_pair(it_last->first, values));
	}
}

void Diskstats::add(const Diskstats &a, const Diskstats &b)
{
	unordered_map<string, long *>::const_iterator it_a;
	unordered_map<string, long *>::const_iterator it_b;

	unordered_map<string, long *> a_map = a.mapStats;
	unordered_map<string, long *> b_map = b.mapStats;
	
	for(	it_a = a_map.cbegin(); 
		it_a != a_map.cend(); 
		++it_a)
	{
		it_b = b_map.find(it_a->first);
		long * values = new long[13];

		long * a_values = it_a->second;
		long * b_values = it_b->second;
		
		for(int i = 0; i < 13; ++i)
		{
			values[i] = a_values[i] + b_values[i];
		}

		mapStats.insert(make_pair(it_a->first, values));
	}
}

const string& Diskstats::getDiskstatsFile() const
{
	return diskstatsFile;
}
// --- private methods ---

void Diskstats::readLine(const string &line)
{
	string device;
	//TODO: remove this hardcoded value.
	long * values = new long[13];

	string words [14];
	//i is the index for storing words. 
	int i = 0;

	//j is the position in the string line.
	int j = 0;

	while(j < line.size() && i < 14)
	{
		string tmp;
		//searching for a word/number in the line.
		while(line[j] == ' ' || line[j] == '\t')
		{ ++j; }
		
		//reading all the word/number.
		while(j < line.size() && line[j] != ' ' && line[j] != '\t')
		{ tmp += line[j++]; }

		words[i++] = tmp;

	}
	
	for(i = 0; i < 14; ++i)
	{
		if(i < 2)
			values[i] = stol(words[i]);
		else if (i > 2)
			values[i-1] = stol(words[i]);
		// words[2] is not converted, because it is device name !
		// It will be kept like it is.
	}	
	
	mapStats.insert(make_pair(words[2], values));
}


void Diskstats::writeDeviceInfo(
			ostream &out, 
			const string &device, 
			const long * const &stats) 
const
{
	static const string statLabel[] = {
		"major number : ", 
		"minor number : ",
		"reads completed successfully : ",
		"reads merged : ",
		"sectors read : ",
		"time spent reading (ms) : ",
		"writes completed : ",
		"writes merged : ",
		"sectors written : ",
		"time spent writing (ms) : ",
		"I/Os currently in progress : ",
		"time spent doing I/Os (ms) : ",
		"weighted time spent doing I/Os (ms) : "
		};
	out << "Device : " << device << endl;
	for(int i = 0; i < 13; ++i)
	{
		cout << statLabel[i] << stats[i] << endl;
	}
		
}
// --- operators overriden ---

Diskstats operator-(const Diskstats &latest, const Diskstats &earliest)
{
	Diskstats d(latest.getDiskstatsFile(), false);
	d.substract(latest, earliest);
	return d;
}

Diskstats operator+(const Diskstats &a, const Diskstats &b)
{
	Diskstats d(a.getDiskstatsFile(), false);
	d.add(a, b);
	return d;
} 

ostream& operator<<(ostream &out, const Diskstats & diskstats)
{
	diskstats.display(out);
	return out;
}
