#include "model/Rotation.h"

namespace UniLib {
	namespace model {
		Rotation::Rotation(DRVector3 xAxis /* = DRVector3(1.0f, 0.0, 0.0f) */, DRVector3 yAxis /* = DRVector3(0.0f, 1.0f, 0.0f) */, DRVector3 zAxis /* = DRVector3(0.0, 0.0f, 1.0f) */)
			: mXAxis(xAxis), mYAxis(yAxis), mZAxis(zAxis)
		{

		}

		Rotation::~Rotation()
		{

		}

		void Rotation::rotateRel(const DRVector3& rotation)
		{
			DRMatrix mRot;
			if(rotation.x != 0.0f) {
				printf("miau\n");
			}
			// Rotation um die z-Achse des Objekts
			mRot = DRMatrix::rotationAxis(mZAxis, rotation.z);
			mXAxis = mXAxis.transformNormal(mRot);
			mYAxis = mYAxis.transformNormal(mRot);

			// Rotation um die y-Achse des Objekts
			mRot = DRMatrix::rotationAxis(mYAxis, rotation.y);
			mXAxis = mXAxis.transformNormal(mRot);
			mZAxis = mXAxis.cross(mYAxis);

			// Rotation um die x-Achse des Objekts
			mRot = DRMatrix::rotationAxis(mXAxis, rotation.x);
			mYAxis = mYAxis.transformNormal(mRot);
			mZAxis = mXAxis.cross(mYAxis);

		}
		void Rotation::rotateAbs(const DRVector3& rotation)
		{
			// Rotation um die x-Achse
			DRMatrix mRotation(DRMatrix::rotationX(rotation.x));
			mYAxis = mYAxis.transformNormal(mRotation);
			mZAxis = mXAxis.cross(mYAxis);

			// Rotation um die y-Achse
			mRotation = DRMatrix::rotationY(rotation.y);
			mXAxis = mXAxis.transformNormal(mRotation);
			mZAxis = mXAxis.cross(mYAxis);

			// Rotation um die z-Achse
			mRotation = DRMatrix::rotationZ(rotation.z);
			mXAxis = mXAxis.transformNormal(mRotation);
			mYAxis = mYAxis.transformNormal(mRotation);
		}

		DRMatrix Rotation::calculateMatrix()
		{
			return DRMatrix::axis(mXAxis, mYAxis, mZAxis);
		}
	}
}