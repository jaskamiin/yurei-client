#include "Bencoder.h"


/*INTEGER class definitions*/
Bencoder::Integer::Integer() {}
Bencoder::Integer::~Integer() {}

Bencoder::Integer* Bencoder::Integer::read(std::string content, int& idx)
{
	//get integer from string
	Integer i;
	i.integer = strtoll(&content[idx], NULL, 10);

	//index now points to position after number
	idx += count_digits(i.integer);

	return &i;
}


/*STRING class definitions*/
Bencoder::String::String() {}
Bencoder::String::~String() {}

Bencoder::String* Bencoder::String::read(std::string content, int& idx)
{
	long long len = (Bencoder::Integer::read(content, idx))->get();
	
	if (content.at(idx) == ':')
	{
		String data (content.substr(++idx, len));
		idx += len;
		return &data;
	}
	else throw BENCODE_STR_BAD_FORMAT;

}


/*LIST class definitions*/

Bencoder::List::List()
{
	list.clear();
}

Bencoder::List::~List(){}

Bencoder::List* Bencoder::List::read(std::string content, int& idx)
{
	if (content.at(idx) == 'l') idx++;
	List list;

	while (content.at(idx) != 'e')
		list.add(decode(content, idx));

	idx++;
	return &list;
}

void Bencoder::List::add(Element obj)
{
	this->list.push_back(obj);
}


/*DICT class definitions*/

Bencoder::Dict::Dict(){}
Bencoder::Dict::~Dict(){}

Bencoder::Dict* Bencoder::Dict::read(std::string content, int& idx)
{
	if (content.at(idx) == 'd') idx++;
	Dict dict;

	while (content.at(idx) != 'e')
	{
		std::string key = decode_string(content, idx);
		Element value	= decode(content, idx);
		dict.add(key, value);
	}

	return &dict;
}

void Bencoder::Dict::add(std::string key, Element val)
{
	dict.insert({ key, val });
}

/*BENCODER class definitions*/

Bencoder::Bencoder()
{

}


Bencoder::~Bencoder()
{
}


Element Bencoder::decode(std::string content, int& idx)
{
	char curr = content[idx];

	if (curr == 'l')
	{
		return *(List::read(content, idx));
	}
	else if (curr == 'd')
	{

	}
	else if (curr == 'i')
	{
		std::size_t e = content.find("e", ++idx);
		if (e == std::string::npos) throw BENCODE_INT_BAD_FORMAT;
				
		long long retInt = decode_int(content, idx);
		if 

	}
	else if (isdigit(curr))
	{

	}
		
}


//Note: BENCODE_FAILURE exception is a placeholder for more specific error messages

int Bencoder::bdecode(std::string data)
{
	try{
		
		std::string::iterator begin = data.begin();
		size_t data_length = data.length();	

		long long	found_int;
		std::string found_str;

		for (int i = 0; i < data_length; ++i)
		{

			char current_token = data[i]; //use this where possible for readability
			
			//current token `i` will decode integer
			if (current_token == 'i')
			{
				//if there's no integer found, throw error
				if (data[i + 1] == 'e') throw BENCODE_INT_BAD_FORMAT;
				if (data[i + 1] != '-' && !isdigit(data[i + 1])) throw BENCODE_INT_NONE_FOUND;
				
				//decode the integer
				found_int = decode_int(data, (++i));


				//make sure the index didn't go out of range
				if (i > data_length) throw BOUNDS_ERROR;
				if (data[i] != 'e') throw BENCODE_FAILURE;

				//parse to whatever format needed.. 
				printf("%lld\n", found_int);

			}
			else if (isdigit(current_token))
			{
				//get length of bencoded string
				long long length = decode_int(data, i);

				//if the number isn't followed by `:`, error 
				if (data[i++] != ':') throw BENCODE_FAILURE;
				
				found_str = data.substr(i, length);

				i += (length-1);

				//parse to whatever format needed
				printf("%s\n", found_str.c_str());


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