#include "controller/NetworkTask.h"
#include "lib/DRINetwork.h"

namespace UniLib {
    namespace controller {

        // constructor
        NetworkTask::NetworkTask(DRNetRequest& request, u16 connectionNumber)
			: mRequest(request), mConnectionNumber(connectionNumber)
        {

        }
		NetworkTask::NetworkTask(size_t taskDependenceCount, u16 connectionNumber)
			: Task(taskDependenceCount), mConnectionNumber(connectionNumber) 
		{
		}
        // deconstructor
        NetworkTask::~NetworkTask()
        {
        }

		DRReturn NetworkTask::run()
		{
			DRINetwork* p = DRINetwork::Instance();
			p->send(mRequest, mConnectionNumber, this);
			return DR_OK;
		}
		void NetworkTask::execute(DRNet_Status status, std::string& data)
		{
			lock();
			mResult = data;
			unlock();
		}

		bool NetworkTask::isTaskFinished()
		{
			bool finished = false;
			lock();
			finished = mResult.size() > 0;
			unlock();
			return finished;
		}
    }
}