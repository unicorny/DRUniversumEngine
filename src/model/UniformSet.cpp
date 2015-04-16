#include "model/UniformSet.h"

namespace UniLib
{
	namespace model {
		
        UniformSet::UniformSet()
        {
        }
        // -------------------------------------------------------------------
        UniformSet::~UniformSet()
        {
            for(std::list<UniformEntry*>::iterator it = mUniformEntrys.begin(); it != mUniformEntrys.end(); it++) {
                DR_SAVE_DELETE(*it);
            }
            mUniformEntrys.clear();
        }

        DRReturn UniformSet::setUniform(std::string name, int value)
        {
            return addUniform(new UniformEntry(UNIFORM_INT, value, name));
        }
        DRReturn UniformSet::setUniform(std::string name, float value)
        {
            return addUniform(new UniformEntry(UNIFORM_FLOAT, (int)value, name));
        }
        DRReturn UniformSet::setUniform(std::string name, DRVector2 value)
        {
            return addUniform(new UniformEntry(UNIFORM_VEC2, (int)new DRVector2(value), name));
        }
        DRReturn UniformSet::setUniform(std::string name, DRVector3 value)
        {
            return addUniform(new UniformEntry(UNIFORM_VEC3, (int)new DRVector3(value), name));
        }
        DRReturn UniformSet::setUniform(std::string name, DRColor value)
        {
            return addUniform(new UniformEntry(UNIFORM_VEC4, (int)new DRColor(value), name));
        }
        DRReturn UniformSet::setUniform(std::string name, DRVector3i value)
        {
            return addUniform(new UniformEntry(UNIFORM_VEC3I, (int)new DRVector3i(value), name));
        }
        DRReturn UniformSet::setUniform(std::string name, DRVector2i value)
        {
            return addUniform(new UniformEntry(UNIFORM_VEC2I, (int)new DRVector2i(value), name));
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PROTECTED AREA
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        UniformSet::UniformEntry::UniformEntry(int* data, size_t arrayEntryCount, std::string name)
            : type(0), intArray(NULL), name(name) 
        {
            intArray = new int[arrayEntryCount];
            memcpy(intArray, data, arrayEntryCount*sizeof(int));
        }
        UniformSet::UniformEntry::UniformEntry(float* data, size_t arrayEntryCount, std::string name)
            : type(0), floatArray(NULL), name(name)
        {
            floatArray = new float[arrayEntryCount];
            memcpy(floatArray, data, arrayEntryCount*sizeof(float));
        }
        UniformSet::UniformEntry::~UniformEntry()
        {
           DR_SAVE_DELETE_ARRAY(intArray);
        }

        DRReturn UniformSet::UniformEntry::update(UniformEntry* entry)
        {
            return DR_OK;
        }
        // ******************************************************************
        DRReturn UniformSet::setUniform(UniformEntry* newUniform)
        {
            if(!newUniform) return DR_ZERO_POINTER;
            HASH hash = DRMakeStringHash(newUniform->name.data());
            std::map<int, UniformEntry*>::iterator it = mUniformEntrys.find(hash);
            if(it != mUniformEntrys.end()) {
                DRReturn result = it->second->update(newUniform);
                DR_SAVE_DELETE(newUniform);
                return result;
            } else {
                mUniformEntrys.insert(UNIFORM_ENTRY_PAIR(hash, newUniform));
            }
            
            return DR_OK;
        }


	};
};