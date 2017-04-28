#include "controller/ShaderManager.h"
#include "model/ShaderProgram.h"
#include "controller/BindToRenderer.h"

namespace UniLib {
	namespace controller {
	using namespace model;


		ShaderManager::ShaderManager()
			: mInitalized(false)
		{

		}

		ShaderManager::ShaderManager(const ShaderManager&)
			: mInitalized(false)
		{

		}

		ShaderManager* const ShaderManager::getInstance() 
		{
			static ShaderManager TheOneAndOnly;
			return &TheOneAndOnly;
		}
		
		DRReturn ShaderManager::init()
		{
			mInitalized = true;
			LOG_INFO("ShaderManager initalisiert");
			return DR_OK;
		}

		void ShaderManager::exit()
		{
			mInitalized = false;

			mShaderProgramEntrys.clear();    

			LOG_INFO("ShaderManager beendet");
		}

		DHASH ShaderManager::makeShaderHash(const char* shaderProgramName)
		{
			DHASH hash = DRMakeStringHash(shaderProgramName);
#ifdef _UNI_LIB_DEBUG
			std::map<DHASH, DRString>::iterator it = mHashCollisionCheckMap.find(hash);
			if(it != mHashCollisionCheckMap.end()) {
				if(std::string(shaderProgramName) != it->second) {
					throw "Shader Hash collision";
				}
			} else {
				mHashCollisionCheckMap.insert(HASH_SHADER_ENTRY(hash, DRString(shaderProgramName)));
			}
#endif
			return hash;
		}

		model::ShaderProgramPtr ShaderManager::getShaderProgram(const char* shaderProgramName)
		{
			DHASH id = makeShaderHash(shaderProgramName);
			return model::ShaderProgramPtr(getShaderProgram(id));
		}

		model::ShaderProgramPtr ShaderManager::getShaderProgram(DHASH id)
		{
			lock();
			if (mShaderProgramEntrys.find(id) != mShaderProgramEntrys.end())
			{
				unlock();
				return mShaderProgramEntrys[id];
			}
			unlock();
			return NULL;
		}

		//! lädt oder return instance auf Textur
		ShaderProgramPtr ShaderManager::getShaderProgram(const char* shaderProgramName, const char* vertexShader, const char* fragmentShader)
		{
			if(!mInitalized) return NULL;
			if(!g_RenderBinder) LOG_ERROR("render binder is not set", NULL);

			//Schauen ob schon vorhanden
			DHASH id = makeShaderHash(shaderProgramName);
			ShaderProgramPtr exist = getShaderProgram(id);
			if (exist.getResourcePtrHolder()) return exist;

			ShaderProgramPtr shaderProgram(g_RenderBinder->newShaderProgram(shaderProgramName, id));
			//shaderProgram->init(getShader(vertexShader, SHADER_VERTEX), getShader(fragmentShader, SHADER_FRAGMENT));
			shaderProgram->addShader(vertexShader, SHADER_VERTEX);
			shaderProgram->addShader(fragmentShader, SHADER_FRAGMENT);

			if (g_HarddiskScheduler) {
				controller::TaskPtr task(new ShaderLoadingTask(shaderProgram, g_HarddiskScheduler));
#ifdef _UNI_LIB_DEBUG
				std::stringstream ss;
				if (vertexShader) ss << vertexShader;
				if (vertexShader && fragmentShader) ss << ", ";
				if (fragmentShader) ss << fragmentShader;
				task->setName(ss.str().data());
#endif
				task->scheduleTask(task);

			}
			else {
				shaderProgram->loadShaderDataIntoMemory();
			}
			//*/
			lock();
			if (!mShaderProgramEntrys.insert(SHADER_PROGRAM_ENTRY(id, shaderProgram)).second)
			{
				unlock();
				LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShaderProgram aufgetreten", 0);
			}
			unlock();
			return shaderProgram;

		}

		ShaderPtr ShaderManager::getShader(const char* shaderName, ShaderType shaderType)
		{
			if(!mInitalized) return NULL;
			if(!g_RenderBinder) LOG_ERROR("render binder is not set", NULL);

			DHASH id = DRMakeFilenameHash(shaderName);

			//Schauen ob schon vorhanden
			lock();
			if(mShaderEntrys.find(id) != mShaderEntrys.end())
			{
				unlock();
				return mShaderEntrys[id];
			}
			unlock();

			EngineLog.writeToLog("[ShaderManager::getShader] start loading shader (%s)!", shaderName);

			ShaderPtr shader(g_RenderBinder->newShader(id));

			const char* path = DRFileManager::Instance().getWholePfad(shaderName);
			DRString shaderString = shaderName;
			if(path)
				shaderString = DRString(DRString(path)+"/"+DRString(shaderName));

			if(!shader.getResourcePtrHolder()->mResource)
				LOG_ERROR("No Memory for new shader left", 0);

			if(shader->init(shaderString.data(), shaderType))
				LOG_ERROR("error loading shader", NULL);

			if(!mShaderEntrys.insert(SHADER_ENTRY(id, shader)).second)
			{
				LOG_ERROR("Unerwarteter Fehler in ShaderManager::getShader aufgetreten", 0);
			}

			EngineLog.writeToLog("[ShaderManager::getShader] end loading shader!");
			return shader;
		}
	}
}

