#pragma once

//还需reverse_iterator未完成
//迭代器，迭代器萃取器
#include <cstddef>

namespace hh_stl
{

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//iterator模板
	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Pointer		pointer;
		typedef Reference   reference;
		typedef Distance	difference_type;
	};


	//-------------------------------------------------------------------iterator_traits----------------------------------------------------------
	//普通迭代器
	template <class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::difference_type		difference_type;
	};

	//指针类型
	template <class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef T*							pointer;
		typedef T&							reference;
		typedef ptrdiff_t					difference_type;
	};

	template <class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef ptrdiff_t					difference_type;
	};


	//-------------------------------------------------------------------迭代器函数-----------------------------------------------------------------
	//1.距离
	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++n;
			++first;
		}
		return n;
	}

	template <class RandomAccessIterator>
	typename iterator_traits<RandomAccessIterator>::difference_type _distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template <class Iterator>
	typename iterator_traits<Iterator> distance(Iterator first, Iterator last)
	{
		return _distance(first, last, typename iterator_traits<Iterator>::iterator_category());
	}


	//2.前进
	template <class InputIterator, class Distance>
	void _advance(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--) ++i;
	}

	template <class BidirectionalIterator, class Distance>
	void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template <class RandomAccessIterator, class Distance>
	void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template <class Iterator, class Distance>
	void advance(Iterator i, Distance n)
	{
		_advance(i, n, typename iterator_traits<Iterator>::iterator_category());
	}
};