#ifndef __UNIVERSUM_LIB_TEST_CPU_TASK_TEST__
#define __UNIVERSUM_LIB_TEST_CPU_TASK_TEST__

#include "Test.h"
#include "controller/CPUTask.h"
#include "controller/Command.h"

namespace UniLib {
	namespace controller {
		class CPUSheduler;
	}
}

namespace UniversumLibTest {

	class HeavyWorkTask: public UniLib::controller::CPUTask
	{
	public:
		HeavyWorkTask(UniLib::controller::CPUSheduler* scheduler, int i) :CPUTask(scheduler), mFinished(false), mNumber(i) {};
		virtual ~HeavyWorkTask() {};
		virtual DRReturn run() 
		{
			mStartTicks = SDL_GetTicks();
			int max =  DRRandom::rInt(10000000, 1000000);
			
			
			//max = 100;
			for(int i = 0; i < max; i++) {
				float number = 17.0f;
				float ergebnis = pow(number, 10);
				number = sqrt(ergebnis);
				
				//printf("%d, number: %.2f\n", i, number);
			}
			//UniLib::EngineLog.writeToLog("floor or ceil: %d, number: %d", floorOrCeil, mNumber);
			
			lock();
			mFinished = true;
			unlock();
			if(mFinishCommand) mFinishCommand->taskFinished(this);
			return DR_OK;
		};
		virtual bool isTaskFinished() {lock(); bool fin = mFinished; unlock(); return fin;}
		virtual const char* getResourceType() const {return "HeavyWorkTask";};
		int getNumber() {return mNumber;}
		Uint32 getStartTicks() {return mStartTicks;}
	protected:
		bool mFinished;
		int mNumber;
		Uint32 mStartTicks;
	};

	

	class CPUTaskTest: public Test
	{
	public:
		CPUTaskTest();
		~CPUTaskTest();

		virtual DRReturn init();
		virtual DRReturn test();
		void finishTask(int number, Uint32 startTicks);
		virtual const char* getName() {return "CPU Task Test";};
	private:
		UniLib::controller::CPUSheduler* mScheduler;
		SDL_mutex* mWorkMutex;
		int		mFinishedTasks;
		// set to true if at least own finished task hasn't the same number as the current value of mFinishedTasks
		bool    mFinishedCountNotNumber;
	};

	class CPUThreadFinishCommand: public UniLib::controller::Command
	{
	public:
		CPUThreadFinishCommand(CPUTaskTest* test): mTest(test) {};
		virtual DRReturn taskFinished(UniLib::controller::Task* task)
		{
			HeavyWorkTask* t = dynamic_cast<HeavyWorkTask*>(task);
			mTest->finishTask(t->getNumber(), t->getStartTicks());
			delete this;
			return DR_OK;
		}
	protected:
		CPUTaskTest* mTest;
	};


};

#endif //__UNIVERSUM_LIB_TEST_CPU_TASK_TEST__