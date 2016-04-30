#include "debug/CPUSchedulerTasksLog.h"
#include <sstream>

namespace UniLib {
	namespace debug {
		CPUShedulerTasksLog::CPUShedulerTasksLog()
		{

		}
		CPUShedulerTasksLog::~CPUShedulerTasksLog()
		{
			if(mInitialized)
				exit();
		}
		CPUShedulerTasksLog* const CPUShedulerTasksLog::getInstance()
		{
			static CPUShedulerTasksLog theOne;
			return &theOne;
		}

		DRReturn CPUShedulerTasksLog::init()
		{
			mInitialized = true;
			return DR_OK;
		}

		void CPUShedulerTasksLog::exit()
		{
			
			for (TaskLogEntryMap::iterator it = mTaskLogEntrys.begin(); it != mTaskLogEntrys.end(); it++) {
				DR_SAVE_DELETE(it->second);
			}
			mTaskLogEntrys.clear();
			mInitialized = false;
		}

		void CPUShedulerTasksLog::addTaskLogEntry(HASH id, const char* resourcesTypeName, const char* threadName, const char* name/* = NULL*/)
		{
			
			lock();
			mTaskLogEntrys.insert(TaskLogEntryPair(id, new TaskLogEntry(id, resourcesTypeName, threadName, name)));
			u8 count = mTaskLogEntrys.size();
			unlock();
			//if(id != 1)
				//printCurrentlyRunningTasks();
		}
		void CPUShedulerTasksLog::removeTaskLogEntry(HASH id)
		{
			lock();
			TaskLogEntryMap::iterator it = mTaskLogEntrys.find(id);
			if (it != mTaskLogEntrys.end()) {
				DR_SAVE_DELETE(it->second);
				mTaskLogEntrys.erase(it);
			}
			unlock();
		}
		void CPUShedulerTasksLog::printCurrentlyRunningTasks()
		{
			EngineLog.writeToLogDirect(getCurrentlRunningTasksTableString());
		}
		std::string CPUShedulerTasksLog::getCurrentlRunningTasksTableString()
		{
			std::stringstream ss;
			ss << "<tr><td><table style='color:grey;font-size:10px;border: 1px solid grey'>";
			ss << "<tr><th>Thread Name</th><th>Resource Type</th><th>Task Name</th></tr>";
			lock();
			for (TaskLogEntryMap::iterator it = mTaskLogEntrys.begin(); it != mTaskLogEntrys.end(); it++) {
				ss << "<tr><td>";
				ss << it->second->threadName;
				ss << "</td><td>";
				ss << it->second->resourceTypeName;
				ss << "</td><td>";
				ss << it->second->name;
				ss << "</td></tr>";
			}
			unlock();
			ss << "</table></td></tr>";
			return ss.str();
		}
	}
}