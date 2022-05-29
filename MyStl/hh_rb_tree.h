#pragma once

#include "hh_construct.h"
#include "hh_iterator.h"
#include "hh_alloc.h"
#include "hh_allocate.h"
#include <utility>		//暂时使用其pair

namespace hh_stl
{

	typedef bool rb_tree_color_type;
	const rb_tree_color_type rb_tree_red = false;
	const rb_tree_color_type rb_tree_black = true;


	//-----------------------------------------------------------------node-------------------------------------------------------------------------
	struct rb_tree_node_base
	{
		typedef rb_tree_node_base* base_pointer;
		typedef rb_tree_color_type color_type;

		color_type color;
		base_pointer parent;
		base_pointer left;
		base_pointer right;

		static base_pointer minimum(base_pointer p)
		{
			if (p == nullptr) return nullptr;

			while (p->left)	p = p->left;
			return p;
		}

		static base_pointer maximum(base_pointer p)
		{
			if (p == nullptr) return nullptr;

			while (p->right) p = p->right;
			return p;
		}
	};

	template<class Value_type>
	struct rb_tree_node : public rb_tree_node_base
	{
		typedef rb_tree_node<Value_type>* node_pointer;
		Value_type value;
	};


	//----------------------------------------------------------------iterator-------------------------------------------------------------------------
	struct rb_tree_base_iterator
	{
		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef rb_tree_node_base::base_pointer base_pointer;

		base_pointer node;

		void increment()
		{
			if (node->right != nullptr)				//有右节点
			{
				node = rb_tree_node_base::minimum(node->right);
			}
			else
			{
				base_pointer p = node->parent;
				while (p->right == node)
				{
					node = p;
					p = p->parent;
				}
				if (node->right != p)			//超出范围会跑到header
					node = p;
			}
		}

		void decrement()
		{
			if (node->parent->parent == node && node->color == rb_tree_red)		//node是header，指向max
			{
				node = node->right;
			}
			else if (node->left != nullptr)
			{
				node = rb_tree_node_base::maximum(node->left);
			}
			else																//node不是header也无左节点
			{
				base_pointer p = node->parent;
				while (p->left == node)
				{
					node = p;
					p = p->parent;
				}
				node = p;					//不是很理解(当无最小时会怎么样）
			}
		}
	};

	template <class Value, class Ref, class Ptr>
	struct rb_tree_iterator : rb_tree_base_iterator
	{
		typedef Value	value_type;
		typedef Ref		reference;
		typedef Ptr		pointer;

		typedef rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
		typedef rb_tree_iterator<Value, Ref, Ptr> self;

		typedef typename rb_tree_node<Value>::node_pointer node_pointer;


		//构造函数
		rb_tree_iterator() {}
		rb_tree_iterator(base_pointer x) : node(x) {}
		rb_tree_iterator(node_pointer x) : node(x) {}
		rb_tree_iterator(const iterator& x) : node(x.node) {}
		rb_tree_iterator(const const_iterator& x) : node(x.node) {}


		//重载操作符
		reference operator*() const { return (dynamic_cast<node_pointer>(node))->value; }
		pointer operator->() const { return &*node; }

		self& operator++()
		{
			increment();
			return *this;
		}

		self operator++(int)
		{
			self temp(*this);
			increment();
			return self;
		}

		self& operator--()
		{
			decrement();
			return *this;
		}

		self operator--(int)
		{
			self temp(*this);
			decrement();
			return self;
		}
	};


	//-----------------------------------------------------------------------rb_tree----------------------------------------------------------------
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	class hh_rb_tree			//分配的是一个节点的空间，包含了Value值
	{
	public:
		typedef rb_tree_color_type color_type;
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& cnost_reference;

		typedef rb_tree_node_base* base_pointer;
		typedef rb_tree_node<Value> node;
		typedef node* node_pointer;

		typedef rb_tree_iterator<value_type, reference, pointer>	iterator;
		typedef typename iterator::const_iterator					const_iterator;
		typedef hh_stl::reverse_iterator<iterator>					reverse_iterator;
		typedef hh_stl::reverse_iterator<const_iterator>			const_reverse_iterator;

		typedef simple_alloc<node, Alloc> rb_tree_node_allocator;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	private:
		size_type node_count;
		node_pointer header;
		Compare key_compare;

	private:
		//节点创建删除操作
		node_pointer get_node() { return rb_tree_node_allocator::allocate(); }			//分配一个节点空间
		void put_node(node_pointer p) { rb_tree_node_allocator::dellocate(p); }			//回收一个节点空间
		node_pointer creat_node(const value_type& x)									//创建一个节点
		{
			node_pointer p = get_node();
			construct(&p->value, x);
			return p;
		}
		node_pointer clone_node(node_pointer p)											//复制一个节点
		{
			node_pointer q = creat_node(p->value);
			q->color = p->color;
			q->left = nullptr;
			q->right = nullptr;
			return q;
		}
		void destory_node(node_pointer p)												//删除一个节点
		{
			destory(&p->value);
			put_node(p);
		}

	private:
		//header的相关操作
		node_pointer& root() { return dynamic_cast<node_pointer>(header->parent); }
		node_pointer& left_most() { return dynamic_cast<node_pointer>(header->left); }
		node_pointer& right_most() { return dynamic_cast<node_pointer>(header->right); }

	private:
		//辅助节点操作，方便得到node_pointer
		static node_pointer& left(node_pointer x) { return dynamic_cast<node_pointer&>(x->left); }
		static node_pointer& right(node_pointer x) { return dynamic_cast<node_pointer&>(x->right); }
		static node_pointer& parent(node_pointer x) { return dynamic_cast<node_pointer&>(x->parent); }
		static reference value(node_pointer x) { return x->value; }
		static const key_type& key(node_pointer x) { return KeyOfValue(value(x)); }
		static color_type& color(node_pointer x) { return x->color; }

		static node_pointer& left(base_pointer x) { return dynamic_cast<node_pointer&>(x->left); }
		static node_pointer& right(base_pointer x) { return dynamic_cast<node_pointer&>(x->right); }
		static node_pointer& parent(base_pointer x) { return dynamic_cast<node_pointer&>(x->parent); }
		static reference value(base_pointer x) { return (dynamic_cast<node_pointer>(x))->value; }
		static const key_type& key(base_pointer x) { return KeyOfValue(value(x)); }
		static color_type& color(base_pointer x) { return (dynamic_cast<node_pointer>(x))->color; }

	public:
		//构造，赋值，析构
		hh_rb_tree(const Compare& cmp = Compare()) : node_count(0), key_compare(cmp) { init(); }
		hh_rb_tree(const hh_rb_tree& other);
		hh_rb_tree(hh_rb_tree&& other);
		hh_rb_tree& operator= (const hh_rb_tree& other);
		hh_rb_tree& operator= (hh_rb_tree&& other);
		~hh_rb_tree()
		{
			clear();
			put_node(header);
		}

	public:
		//迭代器相关
		iterator begin() { return left_most(); }
		const_iterator begin() const { return left_most(); }
		iterator end() { return header; }
		const_iterator end() const { return header; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return reverse_iterator(begin()); }

		const_iterator cbegin() const { return begin(); }
		const_reverse_iterator crbegin() const { return rbegin(); }
		const_iterator cend() const { return end(); }
		const_reverse_iterator crend() const { return rend(); }

	public:
		//元素数量
		bool empty() { return node_count == 0; }
		size_type size() { return node_count; }
		size_type max_size() { return static_cast<size_type>(-1); }

	private:
		//辅助树的操作
		void init();
		void clear();
		iterator _insert(base_pointer x, base_pointer y, const value_type& v);
		void _erase(node_pointer& p);

	public:
		//插入节点
		std::pair<iterator, bool> insert_unique(const value_type& x);
		iterator insert_equal(const value_type& x);
	};

};