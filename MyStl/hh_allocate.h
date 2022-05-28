#pragma once

//全局operator new分配
#include <new>
#include <cstddef>

namespace hh_stl
{

	class hh_allocate
	{
	public:
		static void* allocate(size_t n)
		{
			return ::operator new(n);
		}


		static void deallocate(void* p)
		{
			::operator delete(p);
		}

		static void dellocate(void* p, size_t)
		{
			::operator delete(p);
		}
	};

};
