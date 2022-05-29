#pragma once

//有construct和destroy；还有空间配接器
#include <new>
#include "hh_type_traits.h"
#include "hh_iterator.h"

namespace hh_stl
{

	template <class T>
	void construct(T* p)
	{
		::new (p) T();
	}

	template <class T1, class T2>
	void construct(T1* p, const T2& val)
	{
		::new (p) T1(val);
	}

	//--------------------------------------------------------------------------------

	template <class T>
	void destory(T* p)
	{
		_destory(p, type_traits< typename iterator_traits<T>::value_type >::has_trival_destructor());	//萃取出迭代器的value_type再萃取出该类型是否有trival_destructor
	}

	template <class T>
	void _destory(T* p, false_type)
	{
		p->~T();
	}
	
	template <class T>
	void _destory(T* p, true_type) {}

	template <class ForwardIterator>
	void destroy(ForwardIterator first, ForwardIterator last)
	{
		_destory(first, last, type_traits< typename iterator_traits<ForwardIterator>::value_type >::has_trival_destructor());
	}

	template <class ForwardIterator>
	void _destory(ForwardIterator first, ForwardIterator last, false_type)
	{
		for (; first < last; ++first)
			destory(&*first);
	}

	template <class ForwariIterator>
	void _destory(ForwariIterator first, ForwariIterator last, true_type) {}




	//---------------------------------------------------------------------空间配接器------------------------------------------------------------------
	template <class T, class Alloc>
	class simple_alloc
	{
	public:
		static T* allocate()
		{
			return reinterpret_cast<T*> (Alloc::allocate(sizeof(T)));
		}

		static T* allocate(size_t n)		//n个T类型的对象
		{
			return n == 0 ? nullptr : reinterpret_cast<T*> (Alloc::allocate(n * sizeof(T)));
		}


		static void dellocate(T* p)
		{
			Alloc::dellocate(p, sizeof(T));
		}

		static void dellocate(T* p, size_t n)
		{
			if(0 != n) Alloc::dellocate(p, n * sizeof(T));
		}

	};

};