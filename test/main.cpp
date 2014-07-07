#include "include/main.h"

using namespace UniversumLibTest;

std::list<Test*> gTests;

void fillTests()
{
	gTests.push_back(new UnitTest());
}

DRReturn load() {
	UniLib::init();
	fillTests();
	for(std::list<Test*>::iterator it = gTests.begin(); it != gTests.end(); it++)
	{
		if((*it)->init()) LOG_ERROR("Fehler bei Init test", DR_ERROR);
	}
	return DR_OK;
}

DRReturn run()
{
	//printf("running tests\n");
	LOG_INFO("running tests");
	for(std::list<Test*>::iterator it = gTests.begin(); it != gTests.end(); it++)
	{
		//printf("running: %s\n", it->getName());
		DRLog.writeToLog("running test: %s", (*it)->getName());
		if(!(*it)->test()) DRLog.writeToLog("<font color='green'>success</font>");
		else DRLog.writeToLog("<font color='red'>failed</font>");
	}
	return DR_OK;
}

void ende()
{
	for(std::list<Test*>::iterator it = gTests.begin(); it != gTests.end(); it++)
	{
		DR_SAVE_DELETE(*it);
	}
	gTests.clear();
	UniLib::exit();
}


int main(int argc, char argv[])
{
	if(!load()) run();
	ende();	
	return 42;
}