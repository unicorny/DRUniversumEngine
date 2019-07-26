
#include "model/geometrie/Rect2DCollection.h"


namespace UniLib {
	namespace model {
		namespace geometrie {

			Rect2DCollection::Rect2DCollection()
			{

			}

			Rect2DCollection::~Rect2DCollection()
			{

			}

			DRReturn Rect2DCollection::generateVertices(std::vector<DRPack2D_Size_WithKey>* inputRects)
			{
				size_t rectCount = inputRects->size();
				DRVector2i* outputs = new DRVector2i[rectCount];

				DRPack2D packing;

				if (packing.findBestPack(inputRects, outputs, &mCollectionDimension)) {
					DR_SAVE_DELETE_ARRAY(outputs);
					LOG_ERROR("no valid order found", DR_ERROR);
				}
				setRenderMode(GEOMETRIE_RENDER_TRIANGLES);

				mVertexSize = 3;
				mVertexCount = 4 * rectCount;
				mIndiceCount = 6 * rectCount;
				mVertices = new float[mVertexSize * mVertexCount];
				mIndicesArray = new int[mIndiceCount];

				for (size_t i = 0; i < rectCount; i++) 
				{
					DRVector2 pos = outputs[i] / mCollectionDimension;
					DRVector2 size = (*inputRects)[i].size / mCollectionDimension;

					mVertices[i * 4 * mVertexSize]      = pos.x;
					mVertices[i * 4 * mVertexSize + 1]  = (*inputRects)[i].key;
					mVertices[i * 4 * mVertexSize + 2]  = pos.y;

					mVertices[i * 4 * mVertexSize + 3]  = pos.x;
					mVertices[i * 4 * mVertexSize + 4]  = (*inputRects)[i].key;
					mVertices[i * 4 * mVertexSize + 5]  = pos.y + size.y;

					mVertices[i * 4 * mVertexSize + 6]  = pos.x + size.x;
					mVertices[i * 4 * mVertexSize + 7]  = (*inputRects)[i].key;
					mVertices[i * 4 * mVertexSize + 8]  = pos.y + size.y;

					mVertices[i * 4 * mVertexSize + 9]  = pos.x + size.x;
					mVertices[i * 4 * mVertexSize + 10] = (*inputRects)[i].key;
					mVertices[i * 4 * mVertexSize + 11] = pos.y;

					mIndicesArray[i * 6] = 0;
					mIndicesArray[i * 6 + 1] = 1;
					mIndicesArray[i * 6 + 2] = 2;
					mIndicesArray[i * 6 + 3] = 1;
					mIndicesArray[i * 6 + 4] = 2;
					mIndicesArray[i * 6 + 5] = 3;
				}


				DR_SAVE_DELETE_ARRAY(outputs);

				return DR_OK;
			}

		}
	}
}