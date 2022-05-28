#pragma once

//类型萃取type_traits
namespace hh_stl
{

	struct true_type {};
	struct false_type{};

	//一般的类
	template <class Type>
	struct type_traits
	{
		typedef false_type has_trival_default_constructor;
		typedef false_type has_trival_copy_constructor;
		typedef false_type has_trival_assignment_operator;
		typedef false_type has_trival_destructor;
		typedef false_type is_POD_type;
	};

	template <class Type>
	struct type_traits<Type*>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};


	//内置类型
	template <>
	struct type_traits<char>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<signed char>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned char>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<short>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned short>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<int>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned int>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<long>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned long>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<long long>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<unsigned long long>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<float>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<double>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};

	template <>
	struct type_traits<long double>
	{
		typedef true_type has_trival_default_constructor;
		typedef true_type has_trival_copy_constructor;
		typedef true_type has_trival_assignment_operator;
		typedef true_type has_trival_destructor;
		typedef true_type is_POD_type;
	};



};