#pragma once
#include <utility>

template<typename T>
class ComPtr
{
public:
	ComPtr() {}
	ComPtr(T* arg)
		: mPtr{ arg }
	{
	}

	// lvalue reference
	ComPtr(ComPtr& other) = delete;
	ComPtr& operator = (ComPtr& other) = delete;

	// rvalue reference
	ComPtr(ComPtr&& other) = delete;
	ComPtr& operator = (ComPtr&& other)
	{
		std::swap(mPtr, other.mPtr);
		return *this;
	}

	ComPtr& operator = (T* arg)
	{
		if (mPtr)
			mPtr->Release();
		mPtr = arg;
		return *this;
	}

	~ComPtr()
	{
		if (mPtr)
			mPtr->Release();
	}

	T* operator->()
	{
		return mPtr;
	}

	operator T* ()
	{
		return mPtr;
	}
private:
	T* mPtr{ nullptr };
};