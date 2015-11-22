#include "view/Camera.h"

namespace UniLib {
	namespace view {

		Camera::Camera(float fov /* = 90.0f */, float nearClipping /* = 0.1f */, float farClipping /* = 100.0f */)
			:  mFOV(fov), mAspectRatio(4.0f/3.0f), mClipping(nearClipping, farClipping),
			mCameraMatrix(DRMatrix::identity())
		{
		}

		// ----------------------------------------------------------------------

		Camera::~Camera()
		{
		}

		void Camera::updateCameraMatrix()
		{
			calculateMatrix();
			//DRMatrix invert = mMatrix.invert();
			DRMatrix proj = DRMatrix::perspective_projection(mFOV, mAspectRatio, mClipping.x, mClipping.y);
			mCameraMatrix = mMatrix * proj;
		}
	};
};