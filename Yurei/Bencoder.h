#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#include "yurei_utils.h"


class Bencoder
{
private:


//functions
	long long decode_int(std::string, int*);
	std::string decode_string(std::string, int*);
	

public:
	Bencoder();
	~Bencoder();

	/*return decoded object*/
	int bdecode(std::string);

};
