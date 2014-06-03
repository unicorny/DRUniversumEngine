#include "model/Unit.h"

namespace UniLib {
	Unit::Unit(const Unit& copyThis)
	: mUnitParts(NULL), mUnitPartsCount(copyThis.mUnitPartsCount), mType(copyThis.mType)
	{
		mUnitParts = new UnitPart[mUnitPartsCount];
		memcpy(mUnitParts, copyThis.mUnitParts, mUnitPartsCount * sizeof(UnitPart));
	}
	// -----------------------------------------------------------------
	Unit::Unit(double value/* = 0.0f*/, UnitTypes type/* = NONE*/)
		: mUnitParts(NULL), mUnitPartsCount(0), mType(type)
	{
		
	}
	// -----------------------------------------------------------------
	Unit::~Unit()
	{
		DR_SAVE_DELETE_ARRAY(mUnitParts);
		mUnitPartsCount = 0;
		mType = NONE;
	}

	// -----------------------------------------------------------------------
	// calculating
	// -----------------------------------------------------------------------
	Unit::operator double() const 
	{
		double result = 0.0;
		for(int i = 0; i < mUnitPartsCount; i++)
			result += mUnitParts[i];
		return 0.0;
	}

	// ---------------------------------------------------------------------------- 
	const char* Unit::getUnitTypeName(UnitTypes type)
	{
		switch(type)
		{
		case LENGTH: return "Length";
		case AREA: return "Area";
		case VOLUME: return "Volume";
		case WEIGHT: return "Weight"; 
		case TEMPERATURE: return "Temperature";
		case TIME: return "Time";
		case VELOCITY: return "Velocity";
		case ACCELERATION: return "Acceleration";
		default: return "-";
		};
		//return "-";
	}
	// ---------------------------------------------------------------------------------------
	Unit::UnitPart* Unit::collectNegativeUnitParts(double value, u8 &returnUnitPartCount)
	{
		printf("[Unit::collectNegativeUnitParts] value: %f\n", value);
		
		return NULL;
	}

	Unit::UnitPart* Unit::collectPositiveUnitParts(double value, u8 &returnUnitPartCount)
	{
		
		return NULL;
	}

	u8 Unit::countMaxUnitPartsCount(double value)
	{
		double beforeComma = floor(value);
		double afterComma = value - beforeComma;
		return 0;
	}

	// *******************************************************************************************************+
	Unit::UnitPart::UnitPart(float value /* = 0.0f*/, char modifier /* = 0*/)
	: mValue(value), mModifier(modifier)
	{
	}
	// ---------------------------------------------------------------------------------------------------------
	Unit::UnitPart::~UnitPart()
	{
	}
	// ----------------------------------------------------------------------------------------------------------
	const char* Unit::UnitPart::getModifierName()
	{
		switch(mModifier) 
		{
		case   1: return " ";
		case   3: return "kilo";
		case   6: return "mega";
		case   9: return "giga";
		case  -3: return "milli"; 
		case  -6: return "mikro";
		case  -9: return "nano";
		case  -12: return "piko";
		case  -15: return "femto";
		case   12: return "tera";
		case   15: return "peta";
		case   18: return "exa";
		case   21: return "zeta";
		case   24: return "yota";
		case  -18: return "atto";
		case  -21: return "zepto";
		case  -24: return "yokto";

		default: return "-";
		};
		//return "-";
	}
};