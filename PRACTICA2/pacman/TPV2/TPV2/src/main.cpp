// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include "game/Game.h"

int main(int, char**) {

	try 
	{
		Game &g = *Game::instance();
		g.init();
		g.start();
	}

	// catch exceptions thrown as strings
	catch (const std::string& e) 
	{
		std::cerr << e << std::endl;
	}

	// catch exceptions thrown as char*
	catch (const char* e) 
	{ 
		std::cerr << e << std::endl;
	}

	// catch exceptions thrown as a sub-type of std::exception
	catch (const std::exception& e) 
	{ 
		std::cerr << e.what();
	}
	catch (...) 
	{
		std::cerr << "Caught an exception of unknown type ...";
	}

	return 0;
}