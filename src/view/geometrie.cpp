#include "view/Geometrie.h"
#include "model/geometrie/BaseGeometrie.h"

namespace UniLib {
	namespace view {

		Geometrie::Geometrie(model::geometrie::BaseGeometrie* baseGeometrie)
			: mGeometrieModel(baseGeometrie)
		{

		}

		Geometrie::~Geometrie()
		{
			DR_SAVE_DELETE(mGeometrieModel);
		}
	}
}