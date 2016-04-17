#include "Core2Main.h"

void DRBoundingBox::expandBy(const DRBoundingBox& otherBoundingBox)
{
	// xmin
	if (otherBoundingBox.mMin.x < mMin.x) mMin.x = otherBoundingBox.mMin.x;
	// xmax
	if (otherBoundingBox.mMax.x > mMax.x) mMax.x = otherBoundingBox.mMax.x;
	// ymin
	if (otherBoundingBox.mMin.y < mMin.y) mMin.y = otherBoundingBox.mMin.y;
	// ymax
	if (otherBoundingBox.mMax.y > mMax.y) mMax.y = otherBoundingBox.mMax.y;
}

bool DRBoundingBox::intersects(const DRBoundingBox& otherBoundingBox) const
{
	return (max(mMin.x, otherBoundingBox.mMin.x) <= min(mMax.x, otherBoundingBox.mMax.x) &&
		    max(mMin.y, otherBoundingBox.mMin.y) <= min(mMax.y, otherBoundingBox.mMax.y));
}
bool DRBoundingBox::inside(const DRBoundingBox& otherBoundingBox) const
{
	return otherBoundingBox.mMin.x >= mMin.x && otherBoundingBox.mMax.x <= mMax.x &&
		   otherBoundingBox.mMin.y >= mMin.y && otherBoundingBox.mMax.y <= mMax.y;
}
