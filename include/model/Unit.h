/*/*************************************************************************
 *                                                                         *
 * UniversumLib, collection of classes for generating and go through a     *
 * whole universe. It is for my Gameproject Spacecraft					   * 
 * Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.													   *
 *																		   *
 * This program is distributed in the hope that it will be useful,	       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *
 * GNU General Public License for more details.							   *
 *																		   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

#ifndef __DR_UNIVERSUM_LIB_UNIT_H__ 
#define __DR_UNIVERSUM_LIB_UNIT_H__

#include "UniversumLib.h"

namespace UniLib {
	
enum UnitTypes
{
    NONE = 0,
    LENGTH = 1, // meter - length
	AREA = 2, // qm - A
	VOLUME = 3, // m³ - Volume
    WEIGHT = 4, // g
    TEMPERATURE = 5, // temperature in °C
	TIME = 6, // time in seconds 
	VELOCITY = 7, //speed m/s
	ACCELERATION = 8, // m/s²

};

class UNIVERSUM_LIB_API Unit 
{
public:
	Unit(const Unit& copyThis);
	Unit(double value = 0.0f, UnitTypes type = NONE);
	~Unit();

	// calculating
	//! \return true if both have the same type
    bool compareType(Unit& b) const {if(this->mType == b.mType) return true; return false;}
    
/*    // for calculating
    Unit operator + (const Unit& b) const;
    inline Unit operator += (const Unit& b) {*this = *this + b; return *this;}
    
    Unit operator -(const Unit& b) const;
    __inline__ Unit operator -() const {return Unit(-this->mValue, this->mUnitType);}
    __inline__ Unit operator -= (const Unit& b) {*this = *this - b; return *this;}
    
    Unit operator * (const Unit& b) const;
    __inline__ Unit operator *= (const Unit& b) {*this = *this * b; return *this;}
    
    __inline__ Unit operator *  (const double b) const {return Unit(this->mValue*b, this->mUnitType);}
    __inline__ Unit operator *= (const double b) {*this = Unit(this->mValue*b, this->mUnitType); return *this;}
        
    double operator /(const Unit& b) const;
    //__inline__ double operator /= (const Unit& b) {*this = *this / b; return *this;}
    
    __inline__ Unit operator / (const double b) const {return Unit(this->mValue/b, this->mUnitType);}
    __inline__ Unit operator /= (const double b) {*this = Unit(this->mValue/b, this->mUnitType); return *this;}
    
    __inline__ bool operator == (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return true; return false;}
    __inline__ bool operator != (const Unit& b) const {if(this->mUnitType == b.mUnitType && this->mValue == b.mValue) return false; return true;}
    
    bool operator < (const Unit& b) const;
    bool operator <= (const Unit& b) const;
  */  
    //__inline__ operator double() const {return this->mValue;}
    
    static const char* getUnitTypeName(UnitTypes unitType);
    __inline__ UnitTypes getType() const {return mType;}
    
    operator double() const;
private:

	class UnitPart
	{
	public:
		UnitPart(float value = 0.0f, char modifier = 1);
		~UnitPart();

		// return kilo, mega, giga and other endings
		const char* getModifierName();
		__inline__ operator double() const {return pow(mValue, mModifier);};
	private:
		float mValue; // value
		char mModifier; // 10^modifier
	};
	UnitPart* mUnitParts;
	u8		  mUnitPartsCount;
	UnitTypes mType;

	// ----------------------------------------------------------------------
	/*
     * \param value input value to cut
	 * \param returnUnitPartCount referenz to write unitPartCount
	 * \return pointer (to release with delete []) to UnitPart Array
	*/
	UnitPart* collectNegativeUnitParts(double value, u8 &returnUnitPartCount);
	UnitPart* collectPositiveUnitParts(double value, u8 &returnUnitPartCount);
	u8 countMaxUnitPartsCount(double value);
	
};

};



#endif //__DR_UNIVERSUM_LIB_UNIT_H__
