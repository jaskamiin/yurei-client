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
	
	class Integer : public Element 
	{
	private:
		long long integer;
	public:
		Integer();
		~Integer();

		static Integer* read(std::string, int&);
		long long get() { return integer; };
	};

	class String : public Element
	{
	private:
		std::string string;
	public:
		String();
		String(std::string str) : string(str) {}
		~String();

		static String* read(std::string, int&);
		std::string get() { return string; };
	};

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

		
public:
	Bencoder();
	~Bencoder();

	static Element decode(std::string, int&);

	/*return decoded object*/
	int bdecode(std::string);
};
