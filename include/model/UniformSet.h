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

#include "lib/MultithreadContainer.h"

namespace UniLib {
    namespace model {
        
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

            DRReturn setUniform(std::string& name, int value);
            DRReturn setUniform(std::string& name, float value);
            DRReturn setUniform(std::string& name, DRVector2 value);
            DRReturn setUniform(std::string& name, DRVector3 value);
            DRReturn setUniform(std::string& name, DRColor value);
            DRReturn setUniform(std::string& name, DRVector3i value);
            DRReturn setUniform(std::string& name, DRVector2i value);
			DRReturn setUniform(std::string& name, DRMatrix value);

			DRMatrix getUniformMatrix(std::string& name);

			__inline__ bool isDirty() {return mDirtyFlag;}
			__inline__ void unsetDirty() {mDirtyFlag = false;}
        protected:
            struct UniformEntry
            {
                UniformEntry(): type(0),intArray(NULL) {}
                UniformEntry(int* data, size_t arrayEntryCount, std::string& name);
                UniformEntry(float* data, size_t arrayEntryCount, std::string& name);
                ~UniformEntry();

                DRReturn update(void* data, size_t arrayEntryCount, std::string& name);
				__inline__ bool isDirty() {return (type & 64) == 64;}
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
				void* location;
            };
            std::map<int, UniformEntry*> mUniformEntrys;
            typedef std::pair<int, UniformEntry*> UNIFORM_ENTRY_PAIR; 
			bool			mDirtyFlag;

            //DRReturn setUniform(UniformEntry* newUniform);
			DRReturn setUniform(void* data, size_t arrayEntryCount, std::string& name, bool typeFloat = false);
			void* getUniform(std::string& name, size_t arrayEntryCount);
        };

        // *****************************************************************
        // store all uniform in memory directly next, if it make sense 
        class UNIVERSUM_LIB_API FastUniformSet: public UniformSet
        {

        };
    };
};
#endif //__DR_UNIVERSUM_LIB_MODEL_UNIFORM_SET__H