#include "model/geometrie/BaseGeometrie.h"
#include "model/geometrie/BaseGeometrieContainer.h"

namespace UniLib {
	namespace model {
		namespace geometrie {

			BaseGeometrie::BaseGeometrie(BaseGeometrieContainer* container)
				: mContainer(container) 
			{

			
			}

			BaseGeometrie::~BaseGeometrie()
			{

			}
		}
	}
}