//	SimpleVector.h                                      JJS  10 Dec 1999

//	SimpleVector.h
//
//	This is a simple replacement for the STL vector template class.
//	It's considerably smaller, leaner, and easier to use than the STL vector.
//	Much of the interface is the same, however, so if we later decide to
//	switch to the STL vector class or some other container, it shouldn't
//	be too difficult.
//
//	NOTE: this container does bytewise copies of its elements, so it won't
//	work for classes with non-trivial copy-constructors.  (It's mainly
//	intended for small elements like numbers and pointers.)

#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

#include <string.h>			// (needed for memcpy)
#include "QueegMacros.h"	// (needed for 'or' on broken compilers)

template <class T>
class SimpleVector {

  public:
	// constructors
	inline SimpleVector();						// sets size to 0
	inline SimpleVector(const short n);			// allocates n slots
	inline SimpleVector(const SimpleVector<T>&);	// copy-constructor (copies data)

	// assignment-op
	inline SimpleVector& operator=(const SimpleVector<T>&);	// (copies data)

	// destructor
	inline ~SimpleVector();
	
	// inspectors
	inline unsigned short size() const;			// get number of items
	inline unsigned short bufitems() const;		// number of items the buffer can hold
	inline TQ3Uns32 bufbytes() const;		// size of buffer in bytes
	
	// insertion (moves all following items)
	inline void insert(const T& item, const short idx);

	// treating it like an array
	inline T& operator[](const short idx);		// get item reference by index
	inline T& operator[](const short idx) const;
	
	// treating it like a stack
	inline void push_back(const T& item);		// push onto end of the vector
	inline T pop_back();						// get last item, remove from vector

	// other ways to delete items
	inline void deleteIdx(const short idx);		// delete an item by its index
	inline void deleteAll();					// delete all items
		
	// buffer management
	inline void resize(const short n);			// allocate n slots, keeping current data

	unsigned short mBlockItems;		// number of items to allocate when we expand

  protected:
	T *mBuf;						// array of items
	unsigned short mQtyItems;		// how many items we actually have
	unsigned short mBufItems;		// number of items the buffer can hold
};

#define VecIterate(var,vec) for (short var=0;var<vec.size();var++)
#define VecReverseIterate(var,vec) for (short var=vec.size()-1;var>=0;var--)

template <class T>
inline SimpleVector<T>::SimpleVector()
: mBlockItems(16), mBufItems(0), mBuf(0), mQtyItems(0)
{
}

template <class T>
inline SimpleVector<T>::SimpleVector(const short n)
: mBlockItems(16), mBufItems(n), mQtyItems(n)
{
	if (n) {
		mBuf = new T[mBufItems];	// should throw on error
	} else mBuf = NULL;
}

template <class T>
inline SimpleVector<T>::SimpleVector(const SimpleVector<T>& vec)
: mBufItems(0), mBuf(0)
{
	*this = vec;
}
	
template <class T>
inline SimpleVector<T>& SimpleVector<T>::operator=(const SimpleVector<T>& vec)
{
	if (mBuf) delete[] mBuf;
	mBlockItems = vec.mBlockItems;
	mBufItems = vec.mBufItems;
	mQtyItems = vec.mQtyItems;
	mBuf = new T[mBufItems];		// should throw on error
	memcpy( mBuf, vec.mBuf, vec.bufbytes() );
	return *this;
}

template <class T>
inline SimpleVector<T>::~SimpleVector()
{
	if (mBuf) delete[] mBuf;
}

template <class T>
inline unsigned short SimpleVector<T>::size() const
{
	return mQtyItems;
}

template <class T>
inline unsigned short SimpleVector<T>::bufitems() const
{
	return mBufItems;
}

template <class T>
inline TQ3Uns32 SimpleVector<T>::bufbytes() const
{
	return mBufItems * sizeof(T);
}

template <class T>
inline T& SimpleVector<T>::operator[](const short idx)
{
	if (idx < 0 or idx >= mBufItems) {
		#if DEBUG
			DEBUGSTR("\pInvalid SimpleVector index!");
		#endif
		return mBuf[0];
	}
	
	// if the item requested is beyond the "top" item of the stack metaphor,
	// increase that up to the requested point
	if (idx >= mQtyItems) mQtyItems = idx+1;
	return mBuf[idx];
}

template <class T>
inline T& SimpleVector<T>::operator[](const short idx) const
{
	if (idx < 0 || idx >= mBufItems) {
		#if DEBUG
			DEBUGSTR("\pInvalid SimpleVector index!");
		#endif
		return;
	}
	return mBuf[idx];
}

template <class T>
inline void SimpleVector<T>::push_back(const T& item)
{
	// do we need to increase the buffer size?
	while (mQtyItems >= mBufItems) {
		// yes -- expand it by one block (should never need more than that!)
		resize( mBufItems + mBlockItems );
	}
	// stuff the item
	mBuf[mQtyItems] = item;
	mQtyItems++;
}

template <class T>
inline T SimpleVector<T>::pop_back()
{
	if (mQtyItems > mBufItems || mQtyItems <= 0) {
		#if DEBUG
			DEBUGSTR("\pSimpleVector stack underflow!");
		#endif
		return;
	}
	return mBuf[--mQtyItems];
}

template <class T>
inline void SimpleVector<T>::insert(const T& item, const short idx)
{
	if (idx < 0 || idx > mQtyItems) {
		#if DEBUG
			DEBUGSTR("\pInvalid SimpleVector index!");
		#endif
		return;
	}

	// resize the buffer if needed
	while (mQtyItems >= mBufItems) {
		// yes -- expand it by one block (should never need more than that!)
		resize( mBufItems + mBlockItems );
	}
	
	// move all items past idx
	if (idx < mQtyItems) {
		memcpy(	&mBuf[idx+1],							// dest
				&mBuf[idx],								// source
				sizeof(T) * (mQtyItems - idx) );		// bytes
	}
	
	// finally, stuff the item
	mBuf[idx] = item;
	mQtyItems++;	
}

template <class T>
inline void SimpleVector<T>::deleteIdx(const short idx)
{
	if (idx < 0 || idx >= mQtyItems) {
		#if DEBUG
			DEBUGSTR("\pInvalid SimpleVector index!");
		#endif
		return;
	}
	
	if (idx == mQtyItems-1) {
		// special case -- deleting last item, no need to copy
		mQtyItems -= 1;
	} else {
		// if deleting any but the last item, move remaining ones down
		memcpy(	&mBuf[idx],								// dest
				&mBuf[idx+1],							// source
				sizeof(T) * (mQtyItems - idx - 1) );	// bytes
		mQtyItems -= 1;
	}
	// now check -- should we shrink the buffer down?					
	// do so if the unused spaces are more than twice the block size
	unsigned short unused = (mBufItems - mQtyItems);
	if (unused > mBlockItems*2) {
		unsigned short newqty = (1 + mQtyItems / mBlockItems) * mBlockItems;
		resize(newqty);
	}
}

template <class T>
inline void SimpleVector<T>::deleteAll()
{
	delete[] mBuf;
	mBuf = 0;
	mBufItems = mQtyItems = 0;
}

template <class T>
inline void SimpleVector<T>::resize(const short n)
{
	if (n == mBufItems) return;
	T *newbuf = new T[n];
//	if (!newbuf) throw memFullErr;	// (not needed, as new now throws if it fails)
	if (mBuf) {
		memcpy(		newbuf,							// dest
					mBuf,							// source
					sizeof(T) * mQtyItems );		// bytes
		delete[] mBuf;
	}
	mBuf = newbuf;
	mBufItems = n;
}

#endif
