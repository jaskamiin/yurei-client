#include "Bencoder.h"


/*INTEGER class definitions*/

Bencoder::Integer* Bencoder::Integer::read(std::string content, int& idx)
{
	std::size_t e = content.find("e", ++idx);
	if (e == std::string::npos) throw BENCODE_INT_BAD_FORMAT;

	Integer i (strtoll(&content[idx], NULL, 10));

	idx += count_digits(i.integer);

	return &i;
}


/*STRING class definitions*/

Bencoder::String* Bencoder::String::read(std::string content, int& idx)
{
	int64_t len = (Integer::read(content, idx))->get();
	
	if (content.at(idx) != ':')
		throw BENCODE_STR_BAD_FORMAT;
	
	String data (content.substr(++idx, len));
	idx += len;
	return &data;
}


/*LIST class definitions*/

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

std::vector<Element> Bencoder::decode(std::string content)
{
	int idx = 0;
	std::vector<Element> elements;
	std::size_t len = content.length();
	
	while (idx != len)
		elements.push_back(decode(content, idx));

	return elements;
}


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