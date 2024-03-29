#ifndef DEF_DISKSTATS
#define DEF_DISKSTATS

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <ctime>

static struct s_stats{
	long major;
	long minor;
	
	std::string device;

	long reads;
	long reads_merged;
	long sectors_read;
	long ms_reading;
	
	long writes;
	long writes_merged;
	long sectors_written;
	long ms_writing;

	long io_progress;
	long ms_io;
	long ms_weighted_io;

	//TODO: add fields of kernel 4.18+
} s_stats; 

class Diskstats{

	public:
		// --- Constructors / Destructors ---
		Diskstats(const std::string &file = "/proc/diskstats",
				const bool read = true);

		~Diskstats();
		
		// --- Methods ---
		void readFile();
		void readStat();
		void display(std::ostream& out) const;
		void substract(	const Diskstats &latest,
				const Diskstats & earliest);

		void add( const Diskstats &a, const Diskstats &b);
		std::string format(const std::string &toFormat) const;
					
		// --- Getters & setters ---
		const std::string& getDiskstatsFile() const;

	private:
		std::string diskstatsFile;
		time_t lastUpdate;
		std::unordered_map<std::string, long *> mapStats;
		
		// --- private Methods ---

		void readLine(const std::string &line);
		
		void writeDeviceInfo(	std::ostream &out,
					const std::string &device, 
					const long * const& stats) 
					const;
		
		int getValueIndex(const char formatCode) const;
		
		std::string formatDeviceInfo(
				const std::string &toFormat,
				const std::vector<int> &valueIndex,
				std::unordered_map<std::string, long*>
					::const_iterator it
				) const;
};

// --- Operators overriden --

Diskstats operator-(const Diskstats &latest, const Diskstats &earliest);
Diskstats operator+(const Diskstats &a, const Diskstats &b);

std::ostream& operator<<(std::ostream &out, const Diskstats &diskstats);
#endif
