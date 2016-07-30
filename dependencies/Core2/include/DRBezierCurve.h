/*/*************************************************************************
*                                                                         *
* Core, Core-Lib for my programs, Core doesn't need any libraries	   *
* Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
* Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
*                                                                         *
* This program is free software: you can redistribute it and/or modify    *
* it under the terms of the GNU General Public License as published by    *
* the Free Software Foundation, either version 3 of the License, or       *
* any later version.                                                      *
*									   *
* This program is distributed in the hope that it will be useful,	   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
* GNU General Public License for more details.				   *
*									   *
* You should have received a copy of the GNU General Public License	   *
* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
*                                                                         *
***************************************************************************/

#ifndef _DR_CORE_BEZIER_CURVE_H
#define _DR_CORE_BEZIER_CURVE_H

/*!
 *
 * \author: Dario Rekowski
 *
 * \date: 01.05.2016
 * 
 * \desc: class for simple 2D Bezier Curves with n dimensions
 * release node memory by default
 */
class CORE2_API DRBezierCurve 
{
public:
	//! \brief copy input data, create own memory
	DRBezierCurve(const DRVector2* input, const int nodeCount);
	//! \brief only create memory for nodes
	DRBezierCurve(const int nodeCount);
	//! \brief take pointer as own, attention! delete memory in deconstruction
	DRBezierCurve(DRVector2* input, const int nodeCount);
	//! \brief default empty constructor
	DRBezierCurve();
	//! \brief default copy constructor
	DRBezierCurve(const DRBezierCurve& ref);
	//! \brief free memory
	~DRBezierCurve();

	void setNodeMemory(DRVector2* nodeMemory, int nodeCount, bool releaseMemory = false);
	DRVector2& operator[](u32 index) { assert(index < mNodeCount); return mNodes[index]; }

	inline DRVector2* getNodes() { return mNodes; }
	inline u32 getNodeCount() { return mNodeCount; }

	//! \brief calculate one point on curve
	DRVector2 calculatePointOnCurve(float t);
	//! \brief calculate tCount points on curve
	//! \param resulArray point on enough memory for resulting points
	DRReturn calculatePointsOnCurve(float* ts, u32 tCount, DRVector2* resultArray);

	//! \brief split bezier curve into two bezier curves with de castelja algorithmus 
	//! https://en.wikipedia.org/wiki/De_Casteljau's_algorithm
	//! \param secondBezierCurve referenze on target new bezier curve
	//! \param hasMemory if set to true, no memory allocation for secondBezierCurve (enough memory must be reserved)
	//! \param hasMemory if set to false, new memory will be set to secondBezierCurve, without checking previous allocated memory!
	DRReturn splitWithDeCasteljau(DRBezierCurve& secondBezierCurve, bool hasMemory);

	//! \brief reduce bezier curve grade and split curve in two by more than 4 control points
	DRBezierCurve* gradreduktionAndSplit();

	//! \brief for debugging
	DRString getAsString();

	// helper for calculation
	//! \brief will be deleted on deconstruction or if and intern calculation need more
	//! set copy only the new pointer, if there was memory allocated previously, a memory leak will occur
	inline void setTempMemory(DRVector2* tempMemory, u32 tempMemorySize) { mTempMemory = tempMemory; mTempMemorySize = tempMemorySize; }
	//! \brief return current temporary memory
	inline DRVector2* getTempMemory(u32 &tempMemorySize) { tempMemorySize = mTempMemorySize; return mTempMemory; }

	inline u32 calculateControlPointsCount() { return (mNodeCount*(mNodeCount + 1)) / 2; }
protected:

	//! \brief check and create temp memory for calculation of points
	void prepareTempMemory(u32 size, bool copyNodesToBegin = false);

	//! \brief use an mixed algorithmus to get a pretty good approximation to original bezier curve with one control point less
	//! work with any number of control points/grade
	void gradreduktionDynamic();

	DRVector2* mNodes;
	u32		   mNodeCount;

	// temp memory for calculation
	DRVector2* mTempMemory;
	u32		  mTempMemorySize;
};

#endif // _DR_CORE_BEZIER_CURVE_H