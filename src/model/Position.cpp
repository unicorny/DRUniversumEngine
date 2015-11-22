#include "model/Position.h"

namespace UniLib {
	namespace model {

		Position::Position(const DRVector3& pos /* = DRVector3(0.0f) */, const DRVector3& scale /* = DRVector3(1.0f) */)
			: mPosition(pos), mScale(scale)
		{

		}

		Position::~Position()
		{

		}

		DRMatrix Position::calculateMatrix(const DRMatrix& mat)
		{
			mPosition += mPositionAdd.transformNormal(mat);
			mPositionAdd = DRVector3(0.0f);
			return DRMatrix::translation(mPosition) * DRMatrix::scaling(mScale);
		}

	}
};