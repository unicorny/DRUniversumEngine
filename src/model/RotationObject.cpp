#include "model/RotationObject.h"

namespace UniLib {
	namespace model {
		RotationObject::RotationObject(Object* parent /* = NULL */, DRVector3 pos /* = DRVector3(0.0f) */, DRVector3 scale /* = DRVector3(1.0f) */)
			: Object(parent, pos, scale), 
			mXAxis(1.0f, 0.0f, 0.0f), mYAxis(0.0f, 1.0f, 0.0f), mZAxis(0.0f, 0.0f, 1.0f)
		{
		}

		RotationObject::~RotationObject()
		{

		}

		void RotationObject::rotateRel(const DRVector3& rotation)
		{
			DRMatrix mRot;
			lock();
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

			unlock();
		}
		
		void RotationObject::rotateAbs(const DRVector3& rotation)
		{
			lock();
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
			unlock();
		}

		void RotationObject::lookAt(DRVector3 targetPosition, DRVector3 upVector/* = DRVector3(0.0f, 1.0f, 0.0f)*/)
		{
			lock();
			mZAxis = DRVector3(mPosition - targetPosition).normalizeEx();
			mXAxis = upVector.cross(mZAxis).normalizeEx();
			mYAxis = mZAxis.cross(mXAxis).normalizeEx();
			unlock();
		}
	}
}