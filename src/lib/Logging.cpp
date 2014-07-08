#include "UniversumLib.h"

namespace UniLib {
    namespace lib {
  

        EngineLogger::~EngineLogger()
        {

        }

        EngineLogger::EngineLogger()
            : mMutex(NULL)
        {

        }

        DRReturn EngineLogger::init(const char* pcFilename, bool printToConsole)
        {
            mMutex = SDL_CreateMutex(); 
            if(!mMutex) LOG_ERROR_SDL(DR_ERROR);
            DRReturn ret = DRLogger::init(pcFilename, printToConsole);    
            return ret;
        }

        void EngineLogger::exit()
        {
            DRLogger::exit();
            if(mMutex)
                SDL_DestroyMutex(mMutex); 

        }

        DRReturn EngineLogger::writeToLogDirect(DRString text)
        {
            DRReturn ret = DR_OK;
            if(mMutex) 
                if(SDL_LockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);
            DRLogger::writeToLogDirect(text);
            if(mMutex)
                if(SDL_UnlockMutex(mMutex)) LOG_ERROR_SDL(DR_ERROR);
            return ret;
        }

        DRReturn EngineLogger::writeToLogDirect(const char* pcText, ...)
        {
            //Textbuffer zum schreiben
            char acBuffer[1024];

            va_list   Argumente;

            //Buffer fuellen
            va_start(Argumente, pcText);
            vsprintf(acBuffer, pcText, Argumente);
            va_end(Argumente);
            DRString final = getTimeSinceStart();
            final += DRString(acBuffer);

            return writeToLogDirect(final);
        }

        DRReturn EngineLogger::writeToLog(const char* pcText, ...)
        {
            //Textbuffer zum schreiben
            char acBuffer[1024];

            va_list   Argumente;

            //Buffer fuellen
            va_start(Argumente, pcText);
            vsprintf(acBuffer, pcText, Argumente);
            va_end(Argumente);
            DRString final = getTimeSinceStart();
            final += DRString(acBuffer);

            return writeToLog(final);
        }
        DRReturn EngineLogger::writeToLog(DRString text)
        {
            DRString final = DRString("<tr><td><font size=\"2\" color=\"#000080\">");
            final += text;
            final += DRString("</font></td></tr>");

            return writeToLogDirect(final);
        }

    }
}