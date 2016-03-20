#include "view/Geometrie.h"

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