#include "model/UniformSet.h"

namespace UniLib
{
	namespace model {
		
        UniformSet::UniformSet()
			:mDirtyFlag(false)
        {
        }
        // -------------------------------------------------------------------
        UniformSet::~UniformSet()
        {
            for(std::map<int, UniformEntry*>::iterator it = mUniformEntrys.begin(); it != mUniformEntrys.end(); it++) {
                delete it->second;
            }
            mUniformEntrys.clear();
        }

        DRReturn UniformSet::setUniform(std::string& name, int value)
        {
            return setUniform(&value, 1, name, false);
        }
        DRReturn UniformSet::setUniform(std::string& name, float value)
        {
            return setUniform(&value, 1, name, false);
        }
        DRReturn UniformSet::setUniform(std::string& name, DRVector2 value)
        {
            return setUniform(value.c, 2, name, true);
        }
        DRReturn UniformSet::setUniform(std::string& name, DRVector3 value)
        {
            return setUniform(value.c, 3, name, true);
        }
        DRReturn UniformSet::setUniform(std::string& name, DRColor value)
        {
            return setUniform(value.c, 4, name, true);
        }
        DRReturn UniformSet::setUniform(std::string& name, DRVector3i value)
        {
            return setUniform(value.c, 3, name, false);
        }
        DRReturn UniformSet::setUniform(std::string& name, DRVector2i value)
        {
            return setUniform(value.c, 2, name, false);
        }
		DRReturn UniformSet::setUniform(std::string& name, DRMatrix value)
		{
			return setUniform(value.n, 16, name, true);
		}

		DRMatrix UniformSet::getUniformMatrix(std::string& name)
		{
			return DRMatrix((float*)getUniform(name, 16));
		}

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PROTECTED AREA
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        UniformSet::UniformEntry::UniformEntry(int* data, size_t arrayEntryCount, std::string& name)
            : type(arrayEntryCount), intArray(NULL), name(name), location(NULL)
        {
			assert(arrayEntryCount < 64);
            intArray = new int[arrayEntryCount];
            memcpy(intArray, data, arrayEntryCount*sizeof(int));
        }
        UniformSet::UniformEntry::UniformEntry(float* data, size_t arrayEntryCount, std::string& name)
            : type(arrayEntryCount), floatArray(NULL), name(name), location(NULL)
        {
			assert(arrayEntryCount < 64);
			type |= 128;
            floatArray = new float[arrayEntryCount];
            memcpy(floatArray, data, arrayEntryCount*sizeof(float));
        }
        UniformSet::UniformEntry::~UniformEntry()
        {
           DR_SAVE_DELETE_ARRAY(intArray);
		   type = 0;
        }

        DRReturn UniformSet::UniformEntry::update(void* data, size_t arrayEntryCount, std::string& name)
        {
			assert(arrayEntryCount == (type & 63));
			if((type & 128) == 128) {
				memcpy(floatArray, data, arrayEntryCount*sizeof(float));
			} else {
				memcpy(intArray, data, arrayEntryCount*sizeof(int));
			}
			type |= 64;
            return DR_OK;
        }
        // ******************************************************************
		DRReturn UniformSet::setUniform(void* data, size_t arrayEntryCount, std::string& name, bool typeFloat/* = false*/)
        //DRReturn UniformSet::setUniform(UniformEntry* newUniform)
        {
            if(!data) return DR_ZERO_POINTER;
			mDirtyFlag = true;
            HASH hash = DRMakeStringHash(name.data());
			lock();
            std::map<int, UniformEntry*>::iterator it = mUniformEntrys.find(hash);
            if(it != mUniformEntrys.end()) {
                DRReturn result = it->second->update(data, arrayEntryCount, name);
				unlock();
                return result;
            } else {
				UniformEntry* entry = NULL;
				if(typeFloat) entry = new UniformEntry((float*)data, arrayEntryCount, name);
				else entry = new UniformEntry((int*)data, arrayEntryCount, name);
                mUniformEntrys.insert(UNIFORM_ENTRY_PAIR(hash, entry));
            }
            unlock();
            return DR_OK;
        }

		void* UniformSet::getUniform(std::string& name, size_t arrayEntryCount)
		{
			HASH hash = DRMakeStringHash(name.data());
			lock();
			std::map<int, UniformEntry*>::iterator it = mUniformEntrys.find(hash);
			if(it != mUniformEntrys.end()) {
				unlock();
				assert(it->second->getArraySize() == arrayEntryCount);
				return it->second->intArray;
			} 
			unlock();
			return NULL;
		}


	};
};