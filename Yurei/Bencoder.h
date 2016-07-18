#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "yurei_utils.h"
#include "Element.h"


class Bencoder
{
private:

	enum val_id {INT, STR, LIST, DICT} val_id;
	
	class List : public Element
	{
	private:
		std::vector<Element> list;
	public:
		List();
		~List();

		static List* read(std::string, int&);
		void add(Element);
	};
	
	class Dict : public Element
	{
	private:
		std::map<std::string, Element> dict;
	public:
		Dict();
		~Dict();

		static Dict* read(std::string, int&);
		void add(std::string, Element);
	};



//functions
	static long long	decode_int(std::string, int&);
	static std::string	decode_string(std::string, int&);
	

public:
	Bencoder();
	~Bencoder();

	static Element decode(std::string, int&);

	/*return decoded object*/
	int bdecode(std::string);

};
