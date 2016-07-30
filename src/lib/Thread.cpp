//#include "lib/Thread.h"
#include "UniversumLib.h"

namespace UniLib {
    namespace lib {

        Thread::Thread(const char* threadName/* = NULL*/, bool createInConstructor/* = true*/)
            : mutex(NULL), thread(NULL), condition(NULL), semaphore(NULL), exitCalled(false)
        {
			if (createInConstructor) init(threadName);
        } 

		DRReturn Thread::init(const char* threadName)
		{
			semaphore = SDL_CreateSemaphore(1);
			if (!semaphore) LOG_WARNING_SDL();
			condition = SDL_CreateCond();
			if (!condition) LOG_WARNING_SDL();
			if (SDL_SemWait(semaphore)) LOG_WARNING_SDL();
			mutex = SDL_CreateMutex();
			if (!mutex) LOG_WARNING_SDL();

#if SDL_VERSION_ATLEAST(1,3,0)
			thread = SDL_CreateThread(run, threadName, this);
#else
			thread = SDL_CreateThread(run, this);
#endif
			return DR_OK;
		}

        Thread::~Thread()
        {
            if(thread)
            {
                //Post Exit to Thread
                exitCalled = true;
                if(SDL_SemPost(semaphore)) LOG_WARNING_SDL();
                condSignal();
                //SDL_Delay(500);
                SDL_WaitThread(thread, NULL);
                //LOG_WARNING_SDL();

                thread = NULL;
                SDL_DestroySemaphore(semaphore);
                SDL_DestroyMutex(mutex);
                SDL_DestroyCond(condition);
            }
        }

        DRReturn Thread::condSignal()
        {
            if(SDL_CondSignal(condition)== -1) //LOG_ERROR_SDL(DR_ERROR);
            {
                LOG_WARNING("Fehler beim Aufruf von SDL_CondSignal"); 
                LOG_ERROR_SDL(DR_ERROR);
            }
            return DR_OK;
        }

        int Thread::run(void* data)
        {
            Thread* t = static_cast<Thread*>(data);
            while(SDL_SemTryWait(t->semaphore)==SDL_MUTEX_TIMEDOUT)
            {
                if(t->exitCalled) return 0;
                // Lock work mutex
                t->threadLock();
                int status = SDL_CondWait(t->condition, t->mutex); 
                if(t->exitCalled) return 0;
                if( status == 0)
                {
                    int ret = t->ThreadFunction();
                    t->threadUnlock();
                    if(ret)
                    {
                        EngineLog.writeToLog("error-code: %d", ret);
                        LOG_ERROR("error in user defined thread, exit thread", -2);
                    }
                }
                else
                {
                    //unlock mutex and exit
                    t->threadUnlock();
                    LOG_ERROR("Fehler in Thread, exit", -1);
                }
            }
            return 0;
        }

		// ---------------------------------------------------------------------------------
		// Timing thread
		TimingThread::TimingThread(std::string name, Uint32 rerunDelay, Timer* timerOnWhichToAttach, const char* threadName/* = NULL*/)
			: Thread(threadName), mName(name), mTimer(timerOnWhichToAttach), myself(this)
		{
			if(timerOnWhichToAttach) {
				timerOnWhichToAttach->addTimer(name, myself, rerunDelay);
			}
		}
		TimingThread::~TimingThread()
		{

		}

		TimerReturn TimingThread::callFromTimer()
		{
			if(condSignal()) LOG_ERROR("error by calling condSignal", REPORT_ERROR);
			return GO_ON;
		}
    }
}