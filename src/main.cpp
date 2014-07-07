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

    DRReturn init()
    {
        EngineLog.init("EngineLogger.html", true);
        Core2_init("Logger.html");

        return DR_OK;
    }

    void exit() 
    {
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
}