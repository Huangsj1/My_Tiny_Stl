#pragma once
#include "hh_alloc.h"

namespace hh_stl
{
namespace hh_test
{
namespace hh_alloc_test
{

struct small_obj {
	int a;
	double b;
};

struct mid_obj {
	int a;				//4
	double b, c, d, e;	//32
	int* p;				//8
};

struct big_obj {
	int a, b, c, d;		//16
	double e, f, g, h, i, j, k;	//56
};

void alloc_test()
{
	int* a = reinterpret_cast<int*> (hh_stl::hh_alloc::allocate(sizeof(int)));
	hh_stl::hh_alloc::print_test();
	std::cout << "  &a = " << a << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	double* b = reinterpret_cast<double*> (hh_stl::hh_alloc::allocate(sizeof(double)));
	hh_stl::hh_alloc::print_test();
	std::cout << "  &b = " << b << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	mid_obj* c = reinterpret_cast<mid_obj*> (hh_stl::hh_alloc::allocate(sizeof(mid_obj)));
	hh_stl::hh_alloc::print_test();
	std::cout << "  &c = " << c << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	big_obj* d = reinterpret_cast<big_obj*> (hh_stl::hh_alloc::allocate(sizeof(big_obj)));
	hh_stl::hh_alloc::print_test();
	std::cout << "  &d = " << d << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	hh_stl::hh_alloc::deallocate(a, sizeof(int));
	hh_stl::hh_alloc::deallocate(b, sizeof(double));
	hh_stl::hh_alloc::deallocate(c, sizeof(mid_obj));
	hh_stl::hh_alloc::deallocate(d, sizeof(big_obj));

	//small_obj* c = reinterpret_cast<small_obj*> (hh_stl::hh_alloc::allocate(sizeof(small_obj)));
	//hh_stl::hh_alloc::print_test();
	//std::cout << "  &c = " << c << std::endl;
	//std::cout << "---------------------------------------------------------------" << std::endl;

	//for (int i = 0; i < 50; ++i)
	//{
	//	int* a = reinterpret_cast<int*> (hh_stl::hh_alloc::allocate(sizeof(int)));
	//	hh_stl::hh_alloc::print_test();
	//	std::cout << "  &a = " << a << std::endl;
	//	std::cout << "---------------------------------------------------------------" << std::endl;
	//}

	//hh_stl::hh_alloc::deallocate(c, sizeof(small_obj));
}

}
}
}
