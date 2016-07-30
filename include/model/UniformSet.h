/*/*************************************************************************
*                                                                         *
* UniversumLib, collection of classes for generating and go through a     *
* whole universe. It is for my Gameproject Spacecraft					   *
* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *
* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *
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


#ifndef __DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H
#define __DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H

#include "UniversumLib.h"

namespace UniLib {
    namespace model {
        
		class ShaderProgram;
		//typedef DRResourcePtr<ShaderProgram> ShaderProgramPtr;

        /*!
         * \author Dario Rekowski
         * 
         * \date 13.04.15
         * 
         * \brief save uniform data set 
         * build like drink amount data from trink lotse
         * use a dynamic array for uniform data (cache hit)
         * simply said it is only a container for storing different data types in one list
         * can be easy implemented with templates
         */
        class UNIVERSUM_LIB_API UniformSet : public lib::MultithreadContainer
        {
        public:
            UniformSet();
            ~UniformSet();

            DRReturn setUniform(const char* name, int value);
            DRReturn setUniform(const char* name, float value);
            DRReturn setUniform(const char* name, DRVector2 value);
            DRReturn setUniform(const char* name, DRVector3 value);
            DRReturn setUniform(const char* name, DRColor value);
            DRReturn setUniform(const char* name, DRVector3i value);
            DRReturn setUniform(const char* name, DRVector2i value);
			DRReturn setUniform(const char* name, DRMatrix value);

			DRReturn addUniformMapping(const char* name, void* location, HASH programID);
			// must be called from render thread
			virtual DRReturn addLocationToUniform(const char* name, ShaderProgram* program) = 0;
			
			DRReturn removeUniformMapping(const char* name, HASH programID);
			// must be called from render thread
			virtual DRReturn removeLocationFromUniform(const char* name, ShaderProgram* program) = 0;
			
			// must be called from render thread, after binding shader
			virtual void updateUniforms(ShaderProgram* program) = 0;

			DRMatrix getUniformMatrix(const char* name);

			__inline__ bool isDirty() {lock(); bool d = mDirtyFlag; unlock(); return d;}
			__inline__ void unsetDirty() {lock(); mDirtyFlag = false; unlock();}
			void updateDirtyFlags();

        protected:

            struct UniformEntry
            {
                UniformEntry(): type(0),intArray(NULL) {}
                UniformEntry(int* data, size_t arrayEntryCount, const char* name);
                UniformEntry(float* data, size_t arrayEntryCount, const char* name);
                ~UniformEntry();

                DRReturn update(void* data, size_t arrayEntryCount, const char* name);
				void addLocation(void* location, HASH programID);
				void removeLocation(HASH programID);
				__inline__ bool isDirty() {return (type & 64) == 64;}
				__inline__ void unsetDirty() {type &= 191;}
				__inline__ void setDirty() { type |= 64;}
				void checkDirty();
				__inline__ bool isFloat() {return (type & 128) == 128;}
				__inline__ size_t getArraySize() {return type & 63;}
                // ---------------------------------------------
				//! first 6 bit tell the array size
				//! 7 bit is dirty flag
				//! if last bit set it is a float array
                u8 type;
                union {
                    int* intArray;
                    float* floatArray;
                };
                std::string name;
				struct Location {
					Location(void* location): location(location), dirty(true)
					{}
					Location():location(NULL), dirty(true) {}
					void* location;
					bool dirty;
				};
				std::map<HASH, Location> locations;
				typedef std::pair<HASH, Location> LOCATION_PAIR;
            };
            std::map<HASH, UniformEntry*> mUniformEntrys;
            typedef std::pair<HASH, UniformEntry*> UNIFORM_ENTRY_PAIR; 

			bool			mDirtyFlag;

            //DRReturn setUniform(UniformEntry* newUniform);
			DRReturn setUniform(void* data, size_t arrayEntryCount, const char* name, bool typeFloat = false);
			void* getUniform(const char* name, size_t arrayEntryCount);

			UniformEntry* getUniformEntry(const char* name);
        };

        // *****************************************************************
        // store all uniform in memory directly next, if it make sense 
        class UNIVERSUM_LIB_API FastUniformSet: public UniformSet
        {

        };
    };
};
#endif //__DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H