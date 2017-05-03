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

#ifndef __DR_CORE2_VIRTUAL_FILE__
#define __DR_CORE2_VIRTUAL_FILE__

#define VIRTUAL_FILE_HEAD "DRVirtFile"

/*!
* Virtual File Klasse von Dario Rekowski
* Zum laden, speichern und erstellen von Datein
*
* \author: Dario Rekowski
*
* \date: 03.05.17
*
* \desc: A class for a virtual file, which handle adding various parts
* and saving everything to storage
*/

namespace DRFilePart {
	enum Type {
		FILE_PART_BIN = 0,
		FILE_PART_INT = 1,
		FILE_PART_FLOAT = 2,
		FILE_PART_STRING = 3,
		FILE_PART_SHORT = 4,
		FILE_PART_LONG = 5,
		FILE_PART_BYTE = 6,
		FILE_PART_ARRAY_FLOAT = 7,
		FILE_PART_ARRAY_INT = 8,
		FILE_PART_UNKNOWN = 255

	};
	class FilePart {
	public:
		virtual ~FilePart() {};
		virtual size_t size() const = 0;
		virtual size_t count() const = 0;
		virtual const void* data() const = 0;
		virtual Type type() const = 0;

	};
	class Binary : public FilePart {
	public: 
		Binary(void* _data, size_t _size, bool copyData = false, bool freeMemory = true)
			: mSize(_size), mFreeMemory(freeMemory) {
			if (copyData) {
				mData = (void*)malloc(_size);
				memcpy(mData, _data, _size);
			}
			else {
				mData = _data;
			}
		}
		virtual ~Binary() {
			if (mFreeMemory) {
				free(mData);
				mSize = 0;
				mData = NULL;
			}
		}
		virtual size_t size() const { return mSize; }
		virtual size_t count() const { return 1; }
		virtual const void* data() const { return mData; }
		virtual Type type() const { return FILE_PART_BIN;}
	protected:
		void* mData;
		size_t mSize;
	private: 
		bool   mFreeMemory;
	};

	template <class T>
	class SimpleTypes : public FilePart
	{
	public:
		SimpleTypes(T value) : mValue(value) {}
		virtual size_t size() const { return sizeof(T); }
		virtual size_t count() const { return 1; }
		virtual const void* data() const { return &mValue; }
		
		virtual Type type() const {
			if (std::is_same<T, s32>::value) return FILE_PART_INT;
			else if (std::is_same<T, s64>::value) return FILE_PART_LONG;
			else if (std::is_same<T, float>::value) return FILE_PART_FLOAT;
			else if (std::is_same<T, s16>::value) return FILE_PART_SHORT;
			else if (std::is_same<T, u8>::value) return FILE_PART_BYTE;
			return FILE_PART_UNKNOWN;
		}
	protected:
		T mValue;
	};
	typedef SimpleTypes<s32> Integer;
	typedef SimpleTypes<s64> Long;
	typedef SimpleTypes<float> Float;
	typedef SimpleTypes<s16> Short;
	typedef SimpleTypes<u8> Byte;

	template <class T>
	class ArrayTypes : public FilePart
	{
	public: 
		ArrayTypes(T* value, size_t _count, bool copyData = false, bool freeMemory = true) :
			mCount(_count), mFreeMemory(freeMemory) {
			if (copyData) {
				mValue = new T[_count];
				memcpy(mValue, value, _count * sizeof(T));
			}
			else {
				mValue = value;
			}
		}
		virtual ~ArrayTypes() {
			if (mFreeMemory) {
				DR_SAVE_DELETE_ARRAY(mValue);
				mCount = 0;
			}
			mCount = 0;
			mValue = NULL;
		}
		virtual size_t size() const { return sizeof(T); }
		virtual size_t count() const { return mCount; }
		virtual const void* data() const { return mValue; }
		virtual Type type() const {
			if (std::is_same<T, s32>::value) return FILE_PART_ARRAY_INT;
			else if (std::is_same<T, float>::value) return FILE_PART_ARRAY_FLOAT;
			return FILE_PART_UNKNOWN;
		}
	protected:
		T* mValue;
		size_t mCount;
	private:
		bool mFreeMemory;
	};

	typedef ArrayTypes<s32> IntegerArray;
	typedef ArrayTypes<float> FloatArray;

	class String : public FilePart {
	public:
		String(std::string& zeichenkette) : mString(zeichenkette) {}
		String(const char* zeichen) : mString(zeichen) {}
		virtual size_t size() const { return sizeof(char); }
		virtual size_t count() const { return mString.size(); }
		virtual const void* data() const { return mString.data(); }
		virtual Type type() const { return FILE_PART_STRING; }
	protected:
		std::string mString;
	};

}

class CORE2_API DRVirtualFile
{
public:
	virtual DRReturn saveToFile(const char* filename) = 0;
	virtual DRReturn readFromFile(const char* filename) = 0;

};

class CORE2_API DRVirtualBinaryFile : public DRVirtualFile
{
public:
	DRVirtualBinaryFile(bool freeMemory = true) :
		mBinSize(0), mBinData(NULL), mFreeMemory(freeMemory) {}
	DRVirtualBinaryFile(void* data, size_t size, bool copyData = true, bool freeMemory = true) :
		mBinSize(size), mFreeMemory(freeMemory) {
		if (copyData) {
			mBinData = (void*)malloc(size);
			memcpy(mBinData, data, size);
		}
		else {
			mBinData = data;
		}
	}
	~DRVirtualBinaryFile() {
		if (mFreeMemory) free(mBinData);
	}

	virtual DRReturn saveToFile(const char* filename);
	virtual DRReturn readFromFile(const char* filename);

	const size_t getSize() const { return mBinSize; }
	void* getData() { return mBinData;  }

protected:
	size_t		mBinSize;
	void*		mBinData;
	bool		mFreeMemory;
};

class CORE2_API DRVirtualCustomFile : public DRVirtualFile 
{
public:
	DRVirtualCustomFile(bool freeMemory = true) :
		mFreeMemory(freeMemory) {}
	~DRVirtualCustomFile();

	void write(DRFilePart::FilePart* part);
	// return pointer must be deleted afterwards
	DRFilePart::FilePart* read();

	virtual DRReturn saveToFile(const char* filename);
	virtual DRReturn readFromFile(const char* filename);

protected:
	bool							mFreeMemory;
	typedef std::list<DRFilePart::FilePart*>::iterator FilePartListIterator;
	std::list<DRFilePart::FilePart*> mFileParts;

};

#endif 
