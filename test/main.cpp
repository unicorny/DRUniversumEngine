#include "include/main.h"


using namespace UniversumLibTest;

std::list<Test*> gTests;

void fillTests()
{
	gTests.push_back(new UnitTest());
    gTests.push_back(new LoggerTest());
	gTests.push_back(new TimerTest());
	gTests.push_back(new CPUTaskTest());
	gTests.push_back(new HashTest());
//	gTests.push_back(new LoginTest());
}

DRReturn load() {
	UniLib::init();
	fillTests();
	for(std::list<Test*>::iterator it = gTests.begin(); it != gTests.end(); it++)
	{
		if((*it)->init()) LOG_WARNING("Fehler bei Init test");
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
		UniLib::EngineLog.writeToLog("<hr><h3>running test: %s</h3>", (*it)->getName());
		if(!(*it)->test()) UniLib::EngineLog.writeToLog("<h4><font color='green'>success</font></h4>");
		else UniLib::EngineLog.writeToLog("<h4><font color='red'>failed</font></h4>");
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

#ifdef main
#undef main
#endif 

int main(int argc, char argv[])
{
	if(!load()) run();
	ende();	
	return 42;
}