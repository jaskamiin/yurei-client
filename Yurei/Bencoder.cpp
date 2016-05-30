#include "Bencoder.h"

Bencoder::Bencoder()
{
	/*load decode function map
	decode_functions['i'] = &decode_int;
//	decode_functions['l'] = &decode_list;
//	decode_functions['d'] = &decode_dict;
	decode_functions['0'] = &decode_string;
	decode_functions['1'] = &decode_string;
	decode_functions['2'] = &decode_string;
	decode_functions['3'] = &decode_string;
	decode_functions['4'] = &decode_string;
	decode_functions['5'] = &decode_string;
	decode_functions['6'] = &decode_string;
	decode_functions['7'] = &decode_string;
	decode_functions['8'] = &decode_string;
	decode_functions['9'] = &decode_string;

	*///load encode function map
}


Bencoder::~Bencoder()
{
}



int Bencoder::bdecode(std::string data)
{
	try{
		
		std::string::iterator begin = data.begin();
		size_t data_length = data.length();	

		long long	found_int;
		std::string found_str;

		for (int i = 0; i < data_length; ++i)
		{

			char current_token = data[i];
			
			//current token `i` will decode integer
			if (current_token == 'i')
			{
				//if there's no integer found, throw error
				if (data[i + 1] == 'e') throw BENCODE_INT_BAD_FORMAT;
				if (data[i + 1] != '-' && !isdigit(data[i + 1])) throw BENCODE_INT_NONE_FOUND;
				
				//decode the integer
				found_int = decode_int(data, &(++i));

				printf("%lld\n", found_int);

				//make sure the index didn't go out of range
				if (i > data_length) throw BOUNDS_ERROR;
				if (data[i] != 'e') throw BENCODE_FAILURE;

				//parse to whatever format needed

			}
			else if (isdigit(current_token) && data[i + 1] == ':')
			{

			}
			else if (current_token == 'l' || current_token == 'd')
			{

			}
		}

	} catch (EXCEPTION ERROR) {
		// Open error log file and print error message to it
		std::fstream err_log;
		err_log.open("ErrorLog.txt", std::fstream::out | std::fstream::app);

		time_t t = time(0);

		err_log << asctime(localtime(&t)) << "\t--";

		switch (ERROR)
		{
		case BOUNDS_ERROR:
			err_log << BOUNDS_ERROR_MSG("bdecode()");
			break;
		case BENCODE_INT_BAD_FORMAT:
			err_log << BENCODE_INT_BAD_FORMAT_MSG;
			break;
		case BENCODE_INT_NONE_FOUND:
			err_log << BENCODE_INT_NONE_FOUND_MSG;
			break;
		default:
			err_log << BENCODE_FAILURE_MSG;
			break;
		}

		err_log << std::endl;
		err_log.close();

		return ERROR;

	}

	return BENCODE_SUCCESS;
}


long long Bencoder::decode_int(std::string data, int* index)
{
	char* end;
	//get integer from string
	long long retval = strtoll(&data[*index], &end, 10);

	//index now points at `e`
	*index += count_digits(retval);

	return retval;
}