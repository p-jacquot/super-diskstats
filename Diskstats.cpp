#include "Diskstats.h"

#include <cstring>
#include <iostream>

using namespace std;
// ----- Constructor / Destructor -----

Diskstats::Diskstats(const string &file) : diskstatsFile(file), mapStats()
{
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

void Diskstats::display(ostream& out) const
{
	unordered_map<string, long *>::const_iterator it;
	for(it = mapStats.cbegin(); it != mapStats.cend(); ++it)
	{
		writeDeviceInfo(out, it->first, it->second);
		out << endl << endl;
	}	
}

// --- private methods ---

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

ostream& operator<<(ostream &out, const Diskstats & diskstats)
{
	diskstats.display(out);
	return out;
}
