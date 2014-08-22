#include "UniversumLib.h"

// some dll stuff for windows
int         g_iProzess = 0;
int			g_iProzessFunk = 0;


#ifdef _WIN32
//DLL Main Funktion
int WINAPI DllMain(HINSTANCE DllHandle, unsigned long ReasonForCall, void* Reserved)
{
    //Wenn ein weiterer Prozess die dll benutzt
    switch (ReasonForCall)
    {
    case DLL_PROCESS_ATTACH:
        {
            //Der Referenzzähler wird um eins erhöht
            //Start
            g_iProzess++;
        } break;

    case DLL_PROCESS_DETACH:
        {
            //Der Referenzzähler wird um eins reduziert
            //Ende
            g_iProzess--;

            //Sicherstellen, das Exit aufgerufen wird
            if(g_iProzessFunk > g_iProzess)
            {
                LOG_WARNING("Exit musste automatisch aufgerufen werden! SDL");
                UniLib::exit();
                // GameDLLExit();
            }
        } break;

    }


    return 1;
}
#endif //_WIN32

namespace UniLib {
    using namespace lib;
    EngineLogger EngineLog;
	Crypto* g_RSAModule = NULL;

    DRReturn init()
    {
		SDL_Init(SDL_INIT_TIMER);
        Core2_init("Logger.html");
        EngineLog.init("EngineLogger.html", true);        
		g_RSAModule = new Crypto();

        return DR_OK;
    }

    void exit() 
    {
		DR_SAVE_DELETE(g_RSAModule);
		SDL_Quit();
        EngineLog.exit();
        Core2_exit();
    }

    DRString getTimeSinceStart()
    {
        char timeBuffer[256];
        double seconds = static_cast<double>(SDL_GetTicks())/1000.0;
        double minutes = 0.0;
        seconds = modf(seconds/60.0, &minutes);
        seconds *= 60.0; 
        sprintf(timeBuffer, "[%.0f:%02.0f] ", minutes, seconds);

        return DRString(timeBuffer);
    }

	std::string readFileAsString(std::string filename)
	{
		//std::string completePath = 
		const char* path = DRFileManager::Instance().getWholePfad(filename.data());
		std::string complete;
		if(path) {
			complete = std::string(path) + std::string("/") + std::string(filename);
		}
		else {
			complete = std::string(filename);
		}

		DRFile file(complete.data(), "rb");
		if(!file.isOpen()) LOG_ERROR("Error by opening config", std::string());
		unsigned long size = file.getSize();
		void* data = malloc(size+1);
		memset(data, 0, size+1);
		if(file.read(data, size, 1)) 
			LOG_ERROR("Error by reading config", std::string());
		file.close();
		/// parsing

		std::string result((const char*)data);
		
		free(data);
		return result;
	}
}