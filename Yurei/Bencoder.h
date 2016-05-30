#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "yurei_utils.h"


class Bencoder
{
private:
	
//function maps -- key is string, value is function pointer to one of the corresponding functions
//	std::map<std::string, rData (*)()> encode_functions;
//	std::map<std::string, rData (*)()> decode_functions;


//functions
	long long decode_int(std::string, int*);
	std::string decode_string(std::string, int*);
	//List decode_list();
	//Dict decode_dict();
	

public:
	Bencoder();
	~Bencoder();

	/*Return encoded string*/

	/*return decoded object*/
	int bdecode(std::string);

};
