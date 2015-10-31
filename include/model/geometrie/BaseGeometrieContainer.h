/*/*************************************************************************
 *                                                                         *
 * UniversumLib, collection of classes for generating and go through a     *
 * whole universe. It is for my Gameproject Spacecraft					   * 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.													   *
 *																		   *
 * This program is distributed in the hope that it will be useful,	       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
 * GNU General Public License for more details.							   *
 *																		   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

/*!
 * \author: Dario Rekowski
 *
 * \date: 31.10.2015
 * 
 * \desc: Container saving Geometrie Data to CPU for CPU calculations and save it later to GPU
 */

#ifndef __UNIVERSUM_LIB_MODEL_GEOMETRIE_BASE_GEOMETRIE_CONTAINER__H
#define __UNIVERSUM_LIB_MODEL_GEOMETRIE_BASE_GEOMETRIE_CONTAINER__H

#include "UniversumLib.h"

namespace UniLib {
	namespace model {
		namespace geometrie {

			enum GeometrieDataType {
				GEOMETRIE_NONE = 0,
				GEOMETRIE_VERTICES = 1,
				GEOMETRIE_COLORS = 2,
				GEOMETRIE_NORMALS = 4,
				GEOMETRIE_TEX2D_1 = 8,
				GEOMETRIE_TEX2D_2 = 16,
				GEOMETRIE_TEX2D_3 = 32,
				GEOMETRIE_TEX2D_4 = 64,
				GEOMETRIE_3DVECTOR = GEOMETRIE_VERTICES + GEOMETRIE_NORMALS,
				GEOMETRIE_2DVECTOR = GEOMETRIE_TEX2D_1 + GEOMETRIE_TEX2D_2 + GEOMETRIE_TEX2D_3 + GEOMETRIE_TEX2D_4,
				GEOMETRIE_4DVECTOR = GEOMETRIE_COLORS,
				GEOMETRIE_MAX = 128
			};

			class UNIVERSUM_LIB_API BaseGeometrieContainer 
			{
			public: 
				BaseGeometrieContainer();
				virtual ~BaseGeometrieContainer();

				__inline__ void addVector(DRVector3 v3, GeometrieDataType type);
				__inline__ void addVector(DRVector2 v2, GeometrieDataType type);
				__inline__ void addVector(DRColor c, GeometrieDataType type);
				__inline__ void addIndice(int index);

				DRReturn copyToFastAccess();
				void deleteFillingStructures();
				void deleteFastAccessStructures();
				virtual void uploadToGPU() = 0;

			protected:
				
				class GeometriePartVector 
				{
				public:
					GeometriePartVector(GeometrieDataType type):mType(type) {}
					~GeometriePartVector(){}
					
					__inline__ void addVector(DRVector3 v3);
					__inline__ void addVector(DRVector2 v2);
					__inline__ void addVector(DRColor c);
					__inline__ DRVector3 getVector3 (int index); 
					__inline__ DRVector2 getVector2 (int index);
					__inline__ DRColor   getColor(int index);

					__inline__ GeometrieDataType getType() {return mType;}
					int		   getTypeSize();
					int		   getVertexCount();

				protected:
					GeometrieDataType mType;
					std::vector<float> mRawData;

					__inline__ void addFloats(float* c, int count);
				};
				// filling structure
				typedef std::map<GeometrieDataType, GeometriePartVector*>::iterator GeometrieDataMapIterator;
				typedef std::pair<GeometrieDataType, GeometriePartVector*> GeometrieDataMapPair;
				std::map<GeometrieDataType, GeometriePartVector*> mGeometrieDataMap;
				std::vector<int> mIndices;

				// fast access structure
				int*		mIndicesArray;
				int			mIndiceCount;
				float*		mVertices;
				int			mVertexCount;
				GeometrieDataType mVertexFormatFlags;
				int			mVertexSize;

				GeometriePartVector* getGeometriePartVector(GeometrieDataType type);
			};
		}
	}
}

#endif //__UNIVERSUM_LIB_MODEL_GEOMETRIE_BASE_GEOMETRIE_CONTAINER__H