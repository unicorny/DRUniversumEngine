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
            for(std::map<HASH, UniformEntry*>::iterator it = mUniformEntrys.begin(); it != mUniformEntrys.end(); it++) {
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
			lock();
			DRMatrix matrix((float*)getUniform(name, 16));
			unlock();
			return matrix;
		}

		DRReturn UniformSet::addUniformMapping(std::string& name, void* location)
		{
			lock();
			UniformEntry* entry = getUniformEntry(name);
			if(entry) {
				entry->addLocation(location);
				unlock();
				return DR_OK;
			}
			unlock();
			return DR_ERROR;
		}
		DRReturn UniformSet::removeUniformMapping(std::string& name, void* location)
		{
			lock();
			UniformEntry* entry = getUniformEntry(name);
			if(entry) {
				entry->removeLocation(location);
				unlock();
				return DR_OK;
			}
			unlock();
			return DR_ERROR;
		}
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PROTECTED AREA
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        UniformSet::UniformEntry::UniformEntry(int* data, size_t arrayEntryCount, std::string& name)
            : type(arrayEntryCount + 64), intArray(NULL), name(name)
        {
			assert(arrayEntryCount < 64);
            intArray = new int[arrayEntryCount];
            memcpy(intArray, data, arrayEntryCount*sizeof(int));
        }
        UniformSet::UniformEntry::UniformEntry(float* data, size_t arrayEntryCount, std::string& name)
            : type(arrayEntryCount + 64), floatArray(NULL), name(name)
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
			setDirty();
            return DR_OK;
        }
		void UniformSet::UniformEntry::addLocation(void* location)
		{
			locations.push_back(location);
		}
		void UniformSet::UniformEntry::removeLocation(void* location)
		{
			for(std::list<void*>::iterator it = locations.begin(); it != locations.end(); it++)
			{
				if(*it == location) it = locations.erase(it);
			}
		}
        // ******************************************************************
		DRReturn UniformSet::setUniform(void* data, size_t arrayEntryCount, std::string& name, bool typeFloat/* = false*/)
        //DRReturn UniformSet::setUniform(UniformEntry* newUniform)
        {
            if(!data) return DR_ZERO_POINTER;
			lock();
			mDirtyFlag = true;
			UniformEntry* entry = getUniformEntry(name);
            if(entry) {
                DRReturn result = entry->update(data, arrayEntryCount, name);
				unlock();
                return result;
            } else {
				HASH hash = DRMakeStringHash(name.data());
				if(typeFloat) entry = new UniformEntry((float*)data, arrayEntryCount, name);
				else entry = new UniformEntry((int*)data, arrayEntryCount, name);
                mUniformEntrys.insert(UNIFORM_ENTRY_PAIR(hash, entry));
            }
            unlock();
            return DR_OK;
        }

		void* UniformSet::getUniform(std::string& name, size_t arrayEntryCount)
		{
			UniformEntry* entry = getUniformEntry(name);
			if(entry) {
				assert(entry->getArraySize() == arrayEntryCount);
				return entry->intArray;
			} 
			return NULL;
		}

		UniformSet::UniformEntry* UniformSet::getUniformEntry(std::string& name)
		{
			HASH hash = DRMakeStringHash(name.data());
			std::map<HASH, UniformEntry*>::iterator it = mUniformEntrys.find(hash);
			if(it != mUniformEntrys.end()) {
				return it->second;
			}
			return NULL;
		}

	};
};