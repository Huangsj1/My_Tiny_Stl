#pragma once

//内存池分配
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <new>

namespace hh_stl
{
	enum { H_Align = 8 };			//区块上调边界
	enum { H_Max_Bytes = 128 };		//最大上调边界
	enum { H_FreeListNum = H_Max_Bytes - H_Align };		//FreeList个数

	union H_Node
	{
		union H_Node* next;
		char data[1];
	};
	//-----------------------------------------------------------------------------------------------------------------------

	class hh_alloc
	{
	private:
		static H_Node* free_list[H_FreeListNum];		//链表
		
		static char* pool_start;						//当前内存池起始
		static char* pool_end;							//结束
		static size_t heap_size;						//总分配的内存字节数

	public:
		static void* allocate(size_t n);		//n是字节
		static void deallocate(void* p, size_t n);
		static void* reallocate(void* p, size_t old_size, size_t new_size);

		static void print_test();		//测试函数

	private:
		static size_t my_freelist_index(size_t n);			//对应链表位置
		static size_t my_round_up(size_t n);				//上调至H_Align(8)的边界
		static void* my_refill(size_t n);				//填充链表
		static char* chunk_alloc(size_t size, size_t& nblock);		//内存池分配空间给链表用（用char* 是为了+n跳到下一个节点）
	};
	//-------------------------------------------------------------------------------------------------------------------------


	char* hh_alloc::pool_start = nullptr;
	char* hh_alloc::pool_end = nullptr;
	size_t hh_alloc::heap_size = 0;

	H_Node* hh_alloc::free_list[H_FreeListNum] = {
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	};



	void* hh_alloc::allocate(size_t n)
	{
		if (n > static_cast<size_t> (H_Max_Bytes))			//大于链表最大字节数（直接用malloc）
			return ::malloc(n);

		H_Node** my_free_list = free_list + my_freelist_index(n);
		H_Node* result = *my_free_list;
		if (result == nullptr)								//当前链表为空（向内存池中分配空间）
			return my_refill(my_round_up(n));
		*my_free_list = result->next;						//当前链表仍有空间
		return result;
	}


	void hh_alloc::deallocate(void* p, size_t n)
	{
		if (n > static_cast<size_t> (H_Max_Bytes))			
			return ::free(p);

		H_Node** my_free_list = free_list + my_freelist_index(n);		//将回收的节点放入到链表的第一个位置
		H_Node* q = reinterpret_cast<H_Node*> (p);
		q->next = *my_free_list;
		*my_free_list = q;
	}

	
	void* hh_alloc::reallocate(void* p, size_t old_size, size_t new_size)			//回收当前指针所分配的内存，为其分配新的内存
	{
		deallocate(p, old_size);
		p = allocate(new_size);
		return p;
	}


	size_t hh_alloc::my_freelist_index(size_t n) {		
		return ((n + H_Align - 1) / H_Align - 1);
	}

	size_t hh_alloc::my_round_up(size_t n) {
		return ((n + H_Align - 1) & ~(H_Align - 1));
	}


	void* hh_alloc::my_refill(size_t n)			//n已上调至8的边界
	{
		size_t nblock = 20;
		H_Node** my_free_list;
		H_Node* result, *cur, *next;
		char* chunk = chunk_alloc(n, nblock);

		if (1 == nblock)						//只有一块block，直接返回用户
			return chunk;

		my_free_list = free_list + my_freelist_index(n);
		result = reinterpret_cast<H_Node*> (chunk);				//将一块block分给用户
		*my_free_list = next = reinterpret_cast<H_Node*> (chunk + n);
		for (int i = 1; i < nblock - 1; ++i)					//将剩余的链表连起来
		{
			cur = next;
			next = reinterpret_cast<H_Node*> ((char*)cur + n);
			cur->next = next;
		}
		next->next = nullptr;
		return result;
	}


	char* hh_alloc::chunk_alloc(size_t size, size_t& nblock)
	{
		char* result;
		size_t total_bytes = size * nblock;
		size_t left_bytes = pool_end - pool_start;

		if (left_bytes >= total_bytes)				//内存池足够分配nblock块
		{
			result = pool_start;
			pool_start += total_bytes;
			return result;
		}

		else if (left_bytes >= size)				//内存池可分配一个以上，但不足nblock块
		{
			result = pool_start;
			nblock = left_bytes / size;
			total_bytes = size * nblock;
			pool_start += total_bytes;
			return result;
		}

		else										//内存池空间不足（一个都分配不了）
		{
			if (left_bytes > 0)			//碎片内存的利用（一定是H_Align(8)的倍数）
			{
				H_Node** my_free_list = free_list + my_freelist_index(left_bytes);
				((H_Node*)pool_start)->next = *my_free_list;
				*my_free_list = (H_Node*)pool_start;
			}

			size_t bytes_to_get = (total_bytes << 1) + my_round_up(heap_size >> 4);		//分配的内存是H_Align(8)的倍数
			pool_start = reinterpret_cast<char*> (std::malloc(bytes_to_get));
			if (pool_start == nullptr)			//分配内存失败
			{
				H_Node** my_free_list;
				H_Node* p;
				for (size_t i = size; i <= H_Max_Bytes; i += H_Align)		//搜寻后面的链表，若有空间就拿出一块来放到 内存池 里面，再递归重新分配
				{
					my_free_list = free_list + my_freelist_index(i);
					p = *my_free_list;
					if (p)
					{
						pool_start = (char*)p;
						pool_end = pool_start + i;
						*my_free_list = p->next;
						return chunk_alloc(size, nblock);
					}
				}
				pool_end = nullptr;											//山穷水尽，无内存可分配
				std::printf("Out of memory!");
				throw std::bad_alloc();
			}

			pool_end = pool_start + bytes_to_get;
			heap_size += bytes_to_get;
			return chunk_alloc(size, nblock);
		}
	}

	void hh_alloc::print_test()
	{
		std::cout << "heap_size : " << heap_size << std::endl;

		H_Node** my_free_list;
		for (size_t i = 0; i <= H_Max_Bytes; i += H_Align)
		{
			my_free_list = free_list + my_freelist_index(i);
			if (*my_free_list != nullptr)
				std::cout << my_freelist_index(i) << " list is creat. " << std::endl;
		}
	}

};