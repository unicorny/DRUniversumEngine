#include "model/geometrie/Plane.h"
#include "model/geometrie/BaseGeometrieContainer.h"

namespace UniLib {
	namespace model {
		namespace geometrie {

			Plane::Plane(BaseGeometrieContainer* geometrieContainer)
				: BaseGeometrie(geometrieContainer)
			{
			}
			
			Plane::~Plane()
			{

			}

			void Plane::generateVertices(GeometrieDataType type)
			{
				DRVector2 baseEdges[] = {DRVector2(0.0f), DRVector2(1.0f, 0.0f), DRVector2(1.0f, 1.0f), DRVector2(0.0f, 1.0f)};
				for(int i = 0; i < 4; i++) {
					if(type & GEOMETRIE_VERTICES) {
						mContainer->addVector(DRVector3(baseEdges[i].x, 0.0f, baseEdges[i].y), GEOMETRIE_VERTICES);
					}
					if(type & GEOMETRIE_NORMALS) {
						mContainer->addVector(DRVector3(0.0f, 1.0f, 0.0f), GEOMETRIE_NORMALS);
					}
					if(type & GEOMETRIE_TEX2D_1) {
						mContainer->addVector(baseEdges[i], GEOMETRIE_TEX2D_1);
					}
					mContainer->addIndice(i);
				}
			}
		}
	}
}