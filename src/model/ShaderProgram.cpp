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
			if (!file.isOpen())
				LOG_ERROR("cannot open file", NULL);

			int bytesinfile = file.getSize();
			unsigned char *buffer = (unsigned char*)malloc(bytesinfile + 1);
			long unsigned int bytesread = 0;
			file.read(buffer, 1, bytesinfile, &bytesread);
			buffer[bytesread] = 0; // Terminate the string with 0
			EngineLog.writeToLog("read shader file: %s", filename);

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
			mShaderProgram->parseShaderData();
			return DR_OK;
		}
		// ***********************************************************************

		ShaderProgram::ShaderProgram(HASH id/* = 0*/)
		: mId(id), mVertexShader(), mFragmentShader()
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

			controller::TaskPtr task(new ShaderCompileTask(this));
			task->scheduleTask(task);
		}



	}
}