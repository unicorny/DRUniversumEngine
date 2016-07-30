#include "Core2Main.h"

DRBezierCurve::DRBezierCurve(const DRVector2* input, const int nodeCount)
	: DRBezierCurve(nodeCount)
{
	memcpy(mNodes, input, sizeof(DRVector2)*nodeCount);
}

DRBezierCurve::DRBezierCurve(const int nodeCount)
	: mNodes(new DRVector2[nodeCount]), mNodeCount(nodeCount), mTempMemory(NULL), mTempMemorySize(0)
{
}

DRBezierCurve::DRBezierCurve()
	: mNodes(NULL), mNodeCount(0), mTempMemory(NULL), mTempMemorySize(0)
{
}
DRBezierCurve::DRBezierCurve(const DRBezierCurve& ref)
	: mNodes(new DRVector2[ref.mNodeCount]), mNodeCount(ref.mNodeCount), mTempMemory(NULL), mTempMemorySize(0)
{
	memcpy(mNodes, ref.mNodes, sizeof(DRVector2)*ref.mNodeCount);
}

DRBezierCurve::DRBezierCurve(DRVector2* input, const int nodeCount)
	: mNodes(input), mNodeCount(nodeCount), mTempMemory(NULL), mTempMemorySize(0)
{

}
DRBezierCurve::~DRBezierCurve()
{
	if (mNodes) {
		DR_SAVE_DELETE_ARRAY(mNodes);
	}
	if (mTempMemory) {
		DR_SAVE_DELETE_ARRAY(mTempMemory);
	}
}

void DRBezierCurve::setNodeMemory(DRVector2* nodeMemory, int nodeCount, bool releaseMemory/* = false*/)
{
	if (releaseMemory) DR_SAVE_DELETE_ARRAY(mNodes);
	mNodes = nodeMemory;
	mNodeCount = nodeCount;
}

DRVector2 DRBezierCurve::calculatePointOnCurve(float t)
{
	DRVector2 result;
	calculatePointsOnCurve(&t, 1, &result);
	return result;
}

DRReturn DRBezierCurve::calculatePointsOnCurve(float* ts, u32 tCount, DRVector2* resultArray)
{
	if (!resultArray) return DR_ZERO_POINTER;
	if (!ts) return DR_ZERO_POINTER;
	if (!tCount) return DR_ERROR;
	u32 controlPoints = calculateControlPointsCount();
	prepareTempMemory(controlPoints, true);
	for (u32 tIndex = 0; tIndex < tCount; tIndex++) {
		s32 currentBlock = mNodeCount - 1;
		s32 blockCursor = 0;
		for (u32 i = mNodeCount; i < controlPoints; i++) {
			mTempMemory[i] = mTempMemory[i - currentBlock - 1] + (mTempMemory[i - currentBlock] - mTempMemory[i - currentBlock - 1]) * ts[tIndex];
			blockCursor++;
			if (blockCursor >= currentBlock) {
				blockCursor = 0;
				currentBlock--;
				if (currentBlock < 0) LOG_ERROR("current block reached an unexcpected value, error in algorithmus!!!", DR_ERROR);
			}
		}
		resultArray[tIndex] = mTempMemory[controlPoints - 1];
	}
	//return mTempMemory[controlPoints - 1];
	return DR_OK;
}

DRReturn DRBezierCurve::splitWithDeCasteljau(DRBezierCurve& secondBezierCurve, bool hasMemory)
{
	// calculate center pointer and new endpoint of first bezier curve
	// temporary results stay in temp memory
	DRVector2 centerSplitPoint = calculatePointOnCurve(0.5f);
	u32 controlPointCount = calculateControlPointsCount();
	if (!hasMemory) {
		DRVector2* secondBezierMemory = new DRVector2[mNodeCount];
		secondBezierCurve.setNodeMemory(secondBezierMemory, mNodeCount);
	}
	// sort new points
	for (u32 i = 0; i < mNodeCount; i++) {
		mNodes[i] = mTempMemory[mNodeCount*i - (i*(i - 1)) / 2];
		secondBezierCurve.mNodes[i] = mTempMemory[controlPointCount - 1 - (i*(i + 1)) / 2];	
	}
	return DR_OK;
}

DRBezierCurve* DRBezierCurve::gradreduktionAndSplit()
{
	if (mNodeCount == 4) {
		DRVector2* newPoints = new DRVector2[mNodeCount - 1];
		newPoints[0] = mNodes[0];
		newPoints[2] = mNodes[3];
		newPoints[1] = 0.5f * (mNodes[1] + mNodes[2]);
		DR_SAVE_DELETE_ARRAY(mNodes);
		mNodes = newPoints;
		mNodeCount--;
	}
	else if (mNodeCount > 4) {
		DRBezierCurve* secondBezier = new DRBezierCurve(mNodeCount);
		splitWithDeCasteljau(*secondBezier, true);
		gradreduktionDynamic();
		secondBezier->setTempMemory(mTempMemory, mTempMemorySize);
		secondBezier->gradreduktionDynamic();
		secondBezier->setTempMemory(NULL, 0);
		return secondBezier;
	}
	return NULL;
}

DRString DRBezierCurve::getAsString()
{
	std::string str;
	for (int i = 0; i < mNodeCount; i++) {
		DRVector2 v = mNodes[i];
		str += std::string("point " + std::to_string(i) + ": (" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")\n");
	}
	return str;
}

void DRBezierCurve::gradreduktionDynamic()
{
	prepareTempMemory(2 * mNodeCount, false);
	u32 newCount = mNodeCount - 1;
	DRVector2* newPoints = new DRVector2[newCount];
	newPoints[0] = mNodes[0];
	newPoints[newCount - 1] = mNodes[mNodeCount - 1];
	DRVector2* hutTempPoints = mTempMemory;
	hutTempPoints[0] = mNodes[0];
	for (u32 j = 1; j < mNodeCount; j++) {
		hutTempPoints[j] = ((DRReal)mNodeCount*mNodes[j] - (DRReal)j*hutTempPoints[j - 1]) / (DRReal)(mNodeCount - j);
	}
	DRVector2* strichTempPoints = &mTempMemory[mNodeCount];
	strichTempPoints[mNodeCount - 1] = mNodes[mNodeCount - 1];
	for (u32 j = mNodeCount - 1; j > 0; j--) {
		strichTempPoints[j - 1] = ((DRReal)mNodeCount*mNodes[j] - (DRReal)(mNodeCount - j)*strichTempPoints[j]) / (DRReal)j;
	}
	for (u32 j = 1; j < newCount - 1; j++) {
		newPoints[j] = (1.0f - (DRReal)j / (DRReal)(mNodeCount - 1))*hutTempPoints[j] + (DRReal)j / (DRReal)(mNodeCount - 1)*strichTempPoints[j];
	}
	DR_SAVE_DELETE_ARRAY(mNodes);
	mNodes = newPoints;
	mNodeCount = newCount;
}


void DRBezierCurve::prepareTempMemory(u32 size, bool copyNodesToBegin/* = false*/)
{
	if (mTempMemorySize < size) {
		DR_SAVE_DELETE_ARRAY(mTempMemory);
		mTempMemory = new DRVector2[size];
		mTempMemorySize = size;
	}
	if (copyNodesToBegin) {
		memcpy(mTempMemory, mNodes, sizeof(DRVector2)*mNodeCount);
	}
}