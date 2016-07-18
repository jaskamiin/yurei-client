#include "Bencoder.h"


/*INTEGER class definitions*/
Bencoder::Integer::Integer() {}
Bencoder::Integer::~Integer() {}

Bencoder::Integer* Bencoder::Integer::read(std::string content, int& idx)
{
	std::size_t e = content.find("e", ++idx);
	if (e == std::string::npos) throw BENCODE_INT_BAD_FORMAT;

	Integer i (strtoll(&content[idx], NULL, 10));

	idx += count_digits(i.integer);

	return &i;
}


/*STRING class definitions*/
Bencoder::String::String() {}
Bencoder::String::~String() {}

Bencoder::String* Bencoder::String::read(std::string content, int& idx)
{
	long long len = (Integer::read(content, idx))->get();
	
	if (content.at(idx) == ':')
	{
		String data (content.substr(++idx, len));
		idx += len;
		return &data;
	}
	
	throw BENCODE_STR_BAD_FORMAT;
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


/*DICT class definitions*/

Bencoder::Dict::Dict(){}
Bencoder::Dict::~Dict(){}

Bencoder::Dict* Bencoder::Dict::read(std::string content, int& idx)
{
	if (content.at(idx) == 'd') idx++;
	Dict dict;

	while (content.at(idx) != 'e')
	{
		std::string key = String::read(content, idx)->get();
		Element value	= decode(content, idx);
		dict.add(key, value);
	}

	return &dict;
}
	
/*BENCODER class definitions*/

Bencoder::Bencoder(){}
Bencoder::~Bencoder(){}

Element Bencoder::decode(std::string content, int& idx)
{
	switch (content[idx])
	{
	case 'i':
		return *(Integer::read(content, idx));
	case 'l':
		return *(List::read(content, idx));
	case 'd':
		return *(Dict::read(content, idx));
	default:
		if (!isdigit(content[idx])) break;
		return *(String::read(content, idx));
	}
		
	throw BENCODE_FAILURE;
}


//Note: BENCODE_FAILURE exception is a placeholder for more specific error messages

int Bencoder::bdecode(std::string data)
{
	try{
		

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