#pragma once

#include "Server.h"

class Tracker 
{
private:

	unsigned 	cachetime;
	Server 		server;


	void cachetime_update();

public:
	Tracker();
	~Tracker();
};