#include "model/ShaderProgram.h"

namespace UniLib {
	namespace model {

		Shader::Shader(HASH id/* = 0*/)
		: mID(id)
		{
    
		}

		Shader::~Shader()
		{
		}

		unsigned char* Shader::readShaderFile(const char *filename)
		{
			const char* path = DRFileManager::Instance().getWholePfad(filename);
			std::string complete;
			if (path) {
				complete = std::string(path) + std::string("/") + std::string(filename);
			}
			else {
				complete = std::string(filename);
			}

			DRFile file(complete.data(), "rb");
			if (!file.isOpen()) {
				EngineLog.writeToLog("couldn't find shader file: %s", filename);
				LOG_ERROR("cannot open file", NULL);
			}

			int bytesinfile = file.getSize();
			unsigned char *buffer = (unsigned char*)malloc(bytesinfile + 1);
			long unsigned int bytesread = 0;
			file.read(buffer, 1, bytesinfile, &bytesread);
			buffer[bytesread] = 0; // Terminate the string with 0
			//EngineLog.writeToLog("read shader file: %s", filename);

			return buffer;
		}

		

		// ********************************************************************
		DRReturn ShaderLoadingTask::run()
		{
			mShaderProgram->loadShaderDataIntoMemory();
			return DR_OK;
		}
		// ---------------------------------------------------------------------------
		DRReturn ShaderCompileTask::run()
		{
			setName(mShaderProgram->getName());
			mShaderProgram->parseShaderData();
			return DR_OK;
		}
		// -------------------------------------------------------------------------------
		DRReturn LoadShaderCommand::taskFinished(controller::Task* task)
		{
			if (g_HarddiskScheduler) {
				controller::TaskPtr task(new ShaderLoadingTask(mShaderProgram, g_HarddiskScheduler));
#ifdef _UNI_LIB_DEBUG
				task->setName(mName.data());
#endif
				task->scheduleTask(task);

			}
			else {
				mShaderProgram->loadShaderDataIntoMemory();
			}
			return DR_OK;
		}
		// ***********************************************************************

		ShaderProgram::ShaderProgram(const char* name, HASH id/* = 0*/)
		: mId(id), mShaderCompileTask(new ShaderCompileTask(this)), mName(name)
		{

		}


		ShaderProgram::~ShaderProgram()
		{

		}

		DRReturn ShaderProgram::addShader(const char* filename, ShaderType type)
		{
			lock();
			mShaderToLoad.push_back(ShaderData(filename, type));
			unlock();
			return DR_OK;
		}

		void ShaderProgram::loadShaderDataIntoMemory()
		{
			lock();
			for (std::list<ShaderData>::iterator it = mShaderToLoad.begin(); it != mShaderToLoad.end(); it++) {
				if (!it->shaderFileInMemory) it->shaderFileInMemory = Shader::readShaderFile(it->filename.data());
			}
			setLoadingState(LOADING_STATE_PARTLY_LOADED);
			unlock();
#ifdef _UNI_LIB_DEBUG
			lock();
			mShaderCompileTask->setName(mShaderToLoad.front().filename.data());
			unlock();
#endif
			mShaderCompileTask->scheduleTask(mShaderCompileTask);
		}



	}
}