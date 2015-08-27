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
       
        DRReturn EngineLogger::writeAsBinary(DRString name, u8 zahl)
        {
            return writeAsBinary_intern(name, getValueAsBinaryString(zahl));
        }
        DRReturn EngineLogger::writeAsBinary(DRString name, u16 zahl)
        {
            u8 zahl1 = (u8)zahl;
            u8 zahl2 = (u8)(zahl >> 8);
            DRString zahlBuffer = getValueAsBinaryString(zahl1) + " " + getValueAsBinaryString(zahl2);
            return writeAsBinary_intern(name, zahlBuffer);
        }
        DRReturn EngineLogger::writeAsBinary(DRString name, u32 zahl)
        {
            u8 zahlen[4];
            DRString zahlBuffer;
            for(int i = 0; i < 4; i++) {
                zahlBuffer += getValueAsBinaryString((u8)(zahl >> 8*i));
                if(i < 3) zahlBuffer += DRString(" ");
            }
            return writeAsBinary_intern(name, zahlBuffer);
        }
        DRReturn EngineLogger::writeAsBinary(DRString name, u64 zahl)
        {
            u8 zahlen[8];
            DRString zahlBuffer;
            for(int i = 0; i < 8; i++) {
                zahlBuffer += getValueAsBinaryString((u8)(zahl >> 8*i));
                if(i < 7) zahlBuffer += DRString(" ");
            }
            return writeAsBinary_intern(name, zahlBuffer);
        }

        DRReturn EngineLogger::writeAsBinary_intern(DRString name, DRString zahlBuffer)
        {
            DRString final = DRString("<tr><td><font size=\"2\" color=\"#0000f0\">");
            final += name;
            final += DRString("</font>");
            final += zahlBuffer;
            final += DRString("</td></tr>");
            return writeToLogDirect(final.data());
        }
    }
}