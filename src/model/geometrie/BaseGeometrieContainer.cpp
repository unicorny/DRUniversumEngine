#include "model/geometrie/BaseGeometrieContainer.h"

namespace UniLib {
	namespace model {
		namespace geometrie {

			BaseGeometrieContainer::BaseGeometrieContainer() 
				:mIndicesArray(NULL), mIndiceCount(0), mVertices(NULL), mVertexCount(0),
				mVertexFormatFlags(GEOMETRIE_NONE), mVertexSize(0)
			{
	
			}

			BaseGeometrieContainer::~BaseGeometrieContainer()
			{
				deleteFillingStructures();
				deleteFastAccessStructures();
			}

			BaseGeometrieContainer::GeometriePartVector* BaseGeometrieContainer::getGeometriePartVector(GeometrieDataType type)
			{
				GeometrieDataMapIterator it = mGeometrieDataMap.find(type);
				if(it == mGeometrieDataMap.end()) {
					mGeometrieDataMap.insert(GeometrieDataMapPair(type, new GeometriePartVector(type)));
					it = mGeometrieDataMap.find(type);
					assert(it != mGeometrieDataMap.end());
				}
				return it->second;
			}
			void BaseGeometrieContainer::addVector(DRVector3 v3, GeometrieDataType type)
			{
				getGeometriePartVector(type)->addVector(v3);
			}
			void BaseGeometrieContainer::addVector(DRVector2 v2, GeometrieDataType type)
			{
				getGeometriePartVector(type)->addVector(v2);
			}
			void BaseGeometrieContainer::addVector(DRColor c, GeometrieDataType type)
			{
				getGeometriePartVector(type)->addVector(c);
			}
			void BaseGeometrieContainer::addIndice(int index)
			{
				mIndices.push_back(index);
			}

			DRReturn BaseGeometrieContainer::copyToFastAccess()
			{
				deleteFastAccessStructures();
				int vertexCount = 0;
				for(GeometrieDataMapIterator it = mGeometrieDataMap.begin(); it != mGeometrieDataMap.end(); it++) {
					mVertexFormatFlags = (GeometrieDataType)(mVertexFormatFlags | (int)it->second->getType());
					mVertexSize += it->second->getTypeSize();
					if(!vertexCount)
						vertexCount = it->second->getVertexCount();
					if(vertexCount != it->second->getVertexCount())
						LOG_ERROR("vertex Count isn't the same by all different vector datas", DR_ERROR);
				}
				mVertexCount = vertexCount;
				mVertices = new float[vertexCount*mVertexSize];
				for(int i = 0; i < vertexCount; i++) {
					for(int x = 1; x < GEOMETRIE_MAX; x*x)  {
						if(x & mVertexFormatFlags) {
							GeometrieDataMapIterator it = mGeometrieDataMap.find((GeometrieDataType)x);
							assert(it != mGeometrieDataMap.end());
							if(x & GEOMETRIE_2DVECTOR)
								memcpy(&mVertices[i*mVertexSize], it->second->getVector2(i).c, sizeof(float)*2);
							else if(x & GEOMETRIE_3DVECTOR)
								memcpy(&mVertices[i*mVertexSize], it->second->getVector3(i).c, sizeof(float)*3);
							else if(x & GEOMETRIE_4DVECTOR)
								memcpy(&mVertices[i*mVertexSize], it->second->getColor(i).c, sizeof(float)*4);
						}
					}
					
				}

				//indices
				mIndiceCount = mIndices.size();
				mIndicesArray = new int[mIndiceCount];
				for(int i = 0; i < mIndiceCount; i++) {
					mIndicesArray[i] = mIndices[i];
				}
				return DR_OK;

			}
			void BaseGeometrieContainer::deleteFillingStructures()
			{
				for(GeometrieDataMapIterator it = mGeometrieDataMap.begin(); it != mGeometrieDataMap.end(); it++) {
					DR_SAVE_DELETE(it->second);
				}
				mGeometrieDataMap.clear();
			}
			void BaseGeometrieContainer::deleteFastAccessStructures()
			{
				mVertexFormatFlags = GEOMETRIE_NONE;
				mIndiceCount = 0;
				DR_SAVE_DELETE_ARRAY(mIndicesArray);
				mVertexSize = 0;
				mVertexCount = 0;
				DR_SAVE_DELETE_ARRAY(mVertices);
			}

			// ******************************************************************************************

			DRVector3 BaseGeometrieContainer::GeometriePartVector::getVector3 (int index)
			{
				assert(mType & GEOMETRIE_3DVECTOR && index < mRawData.size()*3);
				return DRVector3(&mRawData[index*3]);
			}

			DRVector2 BaseGeometrieContainer::GeometriePartVector::getVector2(int index)
			{
				assert(mType & GEOMETRIE_2DVECTOR && index < mRawData.size()*2);
				return DRVector2(&mRawData[index*2]);
			}

			DRColor BaseGeometrieContainer::GeometriePartVector::getColor(int index)
			{
				assert(mType & GEOMETRIE_4DVECTOR && index < mRawData.size()*4);
				return DRColor(&mRawData[index*4]);
			}

			void BaseGeometrieContainer::GeometriePartVector::addVector(DRVector2 v2)
			{
				assert(mType & GEOMETRIE_2DVECTOR);
				addFloats(v2.c, 2);
			}
			void BaseGeometrieContainer::GeometriePartVector::addVector(DRVector3 v3)
			{
				assert(mType & GEOMETRIE_3DVECTOR);
				addFloats(v3.c, 3);
			}
			void BaseGeometrieContainer::GeometriePartVector::addVector(DRColor c)
			{
				assert(mType & GEOMETRIE_4DVECTOR);
				addFloats(c, 4);
			}
			void BaseGeometrieContainer::GeometriePartVector::addFloats(float* c, int count)
			{
				for(int i = 0; i < count; i++)
					mRawData.push_back(c[i]);
			}	

			int BaseGeometrieContainer::GeometriePartVector::getTypeSize()
			{
				if(mType & GEOMETRIE_2DVECTOR) return 2;
				if(mType & GEOMETRIE_3DVECTOR) return 3;
				if(mType & GEOMETRIE_4DVECTOR) return 4;
				return 0;
			}

			int BaseGeometrieContainer::GeometriePartVector::getVertexCount()
			{
				return mRawData.size() / getTypeSize();
			}

		}
	}
}