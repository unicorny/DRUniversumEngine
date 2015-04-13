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
        UniformSet::UniformEntry::~UniformEntry()
        {
            switch(type) {
            case UNIFORM_VEC2: DRVector2* v2 = (DRVector2*)data; DR_SAVE_DELETE(v2); break;
            case UNIFORM_VEC3: DRVector3* v3 = (DRVector3*)data; DR_SAVE_DELETE(v3); break;
            case UNIFORM_VEC4: DRColor* col = (DRColor*)data; DR_SAVE_DELETE(col); break;
            case UNIFORM_VEC3I: DRVector3i* v3i = (DRVector3i*)data; DR_SAVE_DELETE(v3i); break;
            case UNIFORM_VEC2I: DRVector2i* v2i = (DRVector2i*)data; DR_SAVE_DELETE(v2i); break;
            }
            data = 0;
        }
        // ******************************************************************
        DRReturn UniformSet::addUniform(UniformEntry* newUniform)
        {
            mUniformEntrys.push_back(newUniform);
            return DR_OK;
        }


	};
};