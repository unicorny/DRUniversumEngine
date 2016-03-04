#include "include/HashTest.h"


namespace UniversumLibTest {
	HashTest::HashTest()
	{

	}

	HashTest::~HashTest()
	{


	}


	DRReturn HashTest::init()
	{
		return DR_OK;
	}

	DRReturn HashTest::test()
	{
		DRVector3i v[] = {DRVector3i(1,2,4),
					      DRVector3i(2,1,3),
						  DRVector3i(8,2,212),
						  DRVector3i(-1,3,-6),
						  DRVector3i(2,3,28),
						  DRVector3i(82, 212, 128)};
		int count = sizeof(v) / sizeof(DRVector3i);
		
		for(int i = 0; i < count; i++) {
			HASH h = DRMakeSmallVector3DHash(v[i]);
			//HASH h = v[i].x & 0x0000ff | (v[i].y & 0x0000ff) << 8 | (v[i].z & 0x0000ff) << 16;
			printf("x as hex: %x   ", v[i].x);
			printf("i: %d, hash: %d\n", i, h);
			v[i].print();
			printf("binaer: %s\n", UniLib::getValueAsBinaryString((int)h).data());
		}
		return DR_OK;
	}
}