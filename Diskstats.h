#ifndef DEF_DISKSTATS
#define DEF_DISKSTATS

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>


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
		Diskstats(const std::string &file = "/proc/diskstats");
		~Diskstats();
		
		// --- Methods ---
		void display(std::ostream& out) const;

	private:
		std::string diskstatsFile;
		std::unordered_map<std::string, long *> mapStats;
		
		// --- private Methods ---

		void readFile();
		void readLine(const std::string &line);
		void writeDeviceInfo(	std::ostream &out,
					const std::string &device, 
					const long * const& stats) 
					const;
};

std::ostream& operator<<(std::ostream &out, const Diskstats &diskstats);
#endif
