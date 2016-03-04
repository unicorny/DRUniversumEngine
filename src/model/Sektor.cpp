#include "model/Sektor.h"
#include "model/SektorID.h"

namespace UniLib {
	namespace model {


		// constructor
		Sektor::Sektor(Node* parent, SektorID* id, view::Sektor* sektorView /*= NULL*/)
			: Node(parent),mSektorView(sektorView), mID(id)
		{
			mType |= SEKTOR_NODE;
		}

		// ---------------------------------------------------------------------------

		Sektor::~Sektor()
		{
		}


	};
};

