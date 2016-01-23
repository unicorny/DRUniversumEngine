#include "UniversumLib.h"

// some dll stuff for windows
int         g_iProzess = 0;
int			g_iProzessFunk = 0;
UniLib::controller::BindToRenderer* UniLib::g_RenderBinder = NULL;


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

    DRReturn init()
    {
		SDL_Init(SDL_INIT_TIMER);
        Core2_init("Logger.html");
        EngineLog.init("EngineLogger.html", true);        

        return DR_OK;
    }

    void exit() 
    {
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

	Json::Value convertStringToJson(std::string jsonString)
	{
		Json::Reader read;
		Json::Value result;
		if(!read.parse(jsonString, result)) {
			LOG_ERROR(read.getFormattedErrorMessages().data(), Json::Value(Json::objectValue));
		}
		return result;
	}

    DRString getValueAsBinaryString(u8 zahl)
    {
        char buffer[9];memset(buffer, 0, 9);
        for(int i = 0; i < 8; i++) {
            u8 compareZahl = (u8)pow(2.0, i);
            if((compareZahl & zahl) == compareZahl) {
                buffer[i] = '1';
            } else {
                buffer[i] = '0';
            }
        }
        return DRString(buffer);
    }

	DRString getValueAsBinaryString(int zahl)
	{
		u8 z1 = zahl & 0x000000ff;
		u8 z2 = (zahl & 0x0000ff00) >> 8;
		u8 z3 = (zahl & 0x00ff0000) >> 16;
		u8 z4 = (zahl & 0xff000000) >> 24;
		//printf("%d %d %d %d\n", z1, z2, z3, z4);
		return getValueAsBinaryString(z1) + getValueAsBinaryString(z2) + getValueAsBinaryString(z3) + getValueAsBinaryString(z4);

	}
	void setBindToRenderer(controller::BindToRenderer* bindToRender)
	{
		g_RenderBinder = bindToRender;
	}
}