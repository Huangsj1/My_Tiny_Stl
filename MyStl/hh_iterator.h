#pragma once

//还需reverse_iterator未完成
//迭代器，迭代器萃取器
#include <cstddef>

namespace hh_stl
{
	template<class Iterator>
	struct iterator_traits;

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//-------------------------------------------------------------------------正向迭代器--------------------------------------------------------------
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

	//--------------------------------------------------------------------------反向迭代器-----------------------------------------------------------
	//reverse_iterator模板
	template<class Iterator>
	class reverse_iterator
	{
	public:
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type			value_type;
		typedef typename iterator_traits<Iterator>::pointer				pointer;
		typedef typename iterator_traits<Iterator>::reference			reference;
		typedef typename iterator_traits<Iterator>::difference_type		difference_type;

		typedef Iterator iterator_type;
		typedef reverse_iterator<Iterator> self;

	private:
		iterator_type current;

	public:
		iterator_type& base() const { return current; }

	public:
		//构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type it) : current(it) {}
		reverse_iterator(const self& rhs) : current(rhs.current) {}

	public:
		//重载操作符
		reference operator*() const
		{
			self temp(*this);
			return *--temp;
		}

		pointer operator->() const
		{
			return &*this;
		}

		reference operator[](difference_type n) const		//不可直接交给current去做（current[n]），因为是反向，与正向不同
		{
			return *(*this + n);
		}

		self& operator++()
		{
			--current;
			return *this;
		}

		self operator++(int)
		{
			self temp(*this);
			--current;
			return temp;
		}

		self& operator--()
		{
			++current;
			return *this;
		}

		self operator--(int)
		{
			self temp(*this);
			++current;
			return temp;
		}

		self& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}

		self& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}

		self operator+(difference_type n) const
		{
			return self(current - n);
		}

		self operator-(difference_type n) const
		{
			return self(current + n);
		}
	};

	//重载其他的操作符（不属于该类）
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator- (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}

	template <class Iterator>
	bool operator== (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <class Iterator>
	bool operator!= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Iterator>
	bool operator< (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)		
	{
		return rhs.base() < lhs.base();			//反向了
	}

	template <class Iterator>
	bool operator> (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs < lhs;						//调用上面<
	}

	template <class Iterator>
	bool operator<= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs > rhs);
	}

	template <class Iterator>
	bool operator>= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return !(lhs < rhs);
	}

	//-------------------------------------------------------------------iterator_traits---------------------------------------------------------------
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