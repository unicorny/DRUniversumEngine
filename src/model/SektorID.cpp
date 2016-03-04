#include "model/SektorID.h"

namespace UniLib {
    namespace model {
		
		SektorID::SektorID(u8 id, SektorIdType type, SektorID* parent/* = NULL*/) : SektorID(&id, 1, type, parent) {}
		SektorID::SektorID(u16 id, SektorIdType type, SektorID* parent/* = NULL*/) : SektorID((u8*)&id, 2, type, parent) {}
		SektorID::SektorID(u32 id, SektorIdType type, SektorID* parent/* = NULL*/) : SektorID((u8*)&id, 4, type, parent) {}
		SektorID::SektorID(u64 id, SektorIdType type, SektorID* parent/* = NULL*/) : SektorID((u8*)&id, 8, type, parent) {}
		SektorID::SektorID(u8* id, u8 arraySize, SektorIdType type, SektorID* parent/* = NULL*/)
		 : mID(NULL), mArraySize(arraySize), mType(SEKTOR_ID_DEFAULT), mParent(parent)
        {
			mID = new u8[arraySize];
			memcpy(mID, id, arraySize);
        }
        SektorID::~SektorID()
        {
			DR_SAVE_DELETE_ARRAY(mID);
        }

		DRReturn SektorID::serialize(u8* buffer) {
			assert(buffer != NULL);
			u8 startIndex = 0;
			if (mParent) {
				startIndex = mParent->getSeralizedSize();
			}
			buffer[startIndex++] = mType;
			buffer[startIndex++] = mArraySize;
			memcpy(&buffer[startIndex], mID, mArraySize);
			if (mParent) {
				return mParent->serialize(buffer);
			}
			return DR_OK;

		}

		int SektorID::getSeralizedSize() 
		{ 
			int size = mArraySize + sizeof(u8)*2;
			if (mParent) size += mParent->getSeralizedSize();
			return size;
		}

		SektorID* SektorID::createSektorID(u8* id, u8 arraySize, SektorID* parent/* = NULL*/)
		{
			assert(id != NULL);
			assert(arraySize > 2);
			int currentIndex = 0;
			do {
				u8 type = id[currentIndex++];
				u8 size = id[currentIndex++];
				assert(size <= arraySize - currentIndex);
				parent = new SektorID(&id[currentIndex], size, (SektorIdType)type, parent);
				currentIndex += size;
			} while (currentIndex < arraySize);
			return parent;
		}

		//********************************************************************************************
	
    }
}