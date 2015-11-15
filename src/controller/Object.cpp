#include "controller/Object.h"

namespace UniLib {
	namespace controller {

		Object::Object(Object* parent /* = NULL */, DRVector3 pos /* = DRVector3(0.0f) */, DRVector3 scale /* = DRVector3(0.0f) */)
			: view::Object(parent, pos, scale)
		{

		}

		Object::~Object()
		{

		}

		void Object::updateMatrix() 
		{
			lock();
			mMatrixMutex.lock();
			mMatrix = DRMatrix::translation(mPosition) * DRMatrix::scaling(mScale);
			unlock();
			if(mParent) {
				mMatrix *= mParent->getMatrix();
			}
			mMatrixMutex.unlock();
			for(std::list<model::Object*>::iterator it = mChilds.begin(); it != mChilds.end(); it++) {
				if((*it)->isControllerObject()) {
					Object* child = dynamic_cast<Object*>(*it);
					child->updateMatrix();
				}
			}
		}
		void Object::updateMatrixAsync()
		{
			LOG_WARNING("not implementet yet");
		}
	}
}