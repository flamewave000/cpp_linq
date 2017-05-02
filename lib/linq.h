#pragma once

#ifndef _LINQ_H
#define _LINQ_H

#ifndef _VECTOR_
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#endif
#ifndef _FUNCTIONAL_
#ifndef _GLIBCXX_FUNCTIONAL
#include <functional>
#endif
#endif
#ifndef _ALGORITHM_
#ifndef _GLIBCXX_ALGORITHM
#include <algorithm>
#endif
#endif


namespace linq {
	namespace _inner_linw {
		// TEMPLATE STRUCT more
		template<class _Ty = void>
		struct more
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator< to operands
				return (_Left > _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION more
		template<>
		struct more<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator< to operands
				return (static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right));
			}
		};
		// TEMPLATE STRUCT more
		template<class _Ty = void>
		struct less
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator< to operands
				return (_Left < _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION more
		template<>
		struct less<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator< to operands
				return (static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right));
			}
		};
	}

	/// <summary>predicate used for sorting objects in ascending order</summary>
	_inner_linw::less<> ascending;
	/// <summary>predicate used for sorting objects in descending order</summary>
	_inner_linw::more<> descending;

	template<class _Left, class _Right>
	struct merge_pair {
		_Left left;
		_Right right;
	};

	/// <summary>
	/// Extension to the standard std::vector class. Provides specialized query methods for processing lists.
	/// </summary>
	/// <typeparam name="_Ty">Type for the elements to be held</typeparam>
	template<class _Ty>
	class array : public ::std::vector<_Ty> {
	public:
#ifdef _VECTOR_
		array() : vector() {}
		array(const ::std::size_t &_Count) : vector(_Count) {}
		array(const ::std::vector<_Ty>&_Vec) : vector(_Vec) {}
		array(::std::vector<_Ty>&& _Right) : vector(_Right) {}
		array(::std::initializer_list<_Ty> _Ilist) : vector(_Ilist) {}
		template<class _Iter, class = ::std::enable_if_t<::std::_Is_iterator<_Iter>::value>>
		array(_Iter _First, _Iter _Last, const _Alloc& _Al = _Alloc()) : vector(_First, _Last, _Al) {}
#elif _STL_VECTOR_H
		array() : ::std::vector<_Ty>() {}
		array(::std::size_t __n) : ::std::vector<_Ty>(__n) {}
		array(const ::std::vector<_Ty>&__x) : ::std::vector<_Ty>(__x) {}
		array(::std::vector<_Ty>&& __x) : ::std::vector<_Ty>(__x) {}
		array(::std::initializer_list<_Ty> __l) : ::std::vector<_Ty>(__l) {}
		template<typename _InputIterator, typename = std::_RequireInputIter<_InputIterator>>
		array(_InputIterator __first, _InputIterator __last) : ::std::vector<_Ty>(__first, __last) {}
#endif

	public:
		template<class _Ret>
		using conversion = ::std::function<_Ret(const _Ty&)>;
		typedef ::std::function<bool(const _Ty&)> conditional;
		template<class _Ty2>
		using comparison = ::std::function<bool(const _Ty&, const _Ty2&)>;
		template<class _Ty2, class _Ret>
		using merger = ::std::function<_Ret(const _Ty&, const _Ty2&)>;

	public:
		/// <summary>Performs an item selection which is expected to transform the data in some way and return a new array.</summary>
		/// <param name="selector">Lambda which defines how each item is transformed into the new type.</param>
		/// <param name="result">The array to be filled with the new items.</param>
		/// <typeparam name="_Ret">The new type being created and returned for the new array.</typeparam>
		template<class _Ret>
		void select(const conversion<_Ret> &selector, array<_Ret> &result) const {
			result.resize(this->size());
			for (::std::size_t c = 0, l = this->size(); c < l; c++) {
				result[c] = selector((*this)[c]);
			}
		}

		/// <summary>
		/// Performs an item selection which is expected to transform the data in some way and return a new array.
		/// </summary>
		/// <param name="selector">Lambda which defines how each item is transformed into the new type.</param>
		/// <typeparam name="_Ret">The new type being created and returned for the new array.</typeparam>
		/// <returns>The new array of transformed items.</returns>
		template<class _Ret>
		inline array<_Ret> select(const conversion<_Ret> &selector) const {
			array<_Ret> result(this->size());
			select(selector, result);
			return result;
		}

		/// <summary>
		/// Performs a conditional for filtering the list and returns a new list containing those filtered items.
		/// </summary>
		/// <param name="conditional">The lambda which determines if an item is to be added to the new list.</param>
		/// <returns>The new list of filtered items.</returns>
		array<_Ty> where(const conditional condition) const {
			// Record of elements to keep
			::std::vector<size_t> keep;
			// Loop through elements to see which are being kept and which are to be thrown
			for (::std::size_t c = 0, l = this->size(); c < l; c++) {
				// If we are to keep the element
				if (condition((*this)[c])) {
					// Add the element's index to the 'keep' list
					keep.push_back(c);
				}
			}
			array<_Ty> result(keep.size());
			// We will now take each element we are keeping and move them into sequential
			// order at the begining of our original list
			for (::std::size_t c = 0, last = 0, l = keep.size(); c < l; c++) {
				if (keep[c] != last) {
					// swap elements
					const _Ty &right = (*this)[keep[c]];
					result[c] = ::std::move(right);
				}
				last++;
			}
			// trim the tail off of our original array
			return result;
		}

		/// <summary>
		/// Performs a sort on this list.
		/// </summary>
		/// <param name="pred">Predicate used to determine if the left element should go before the right.</param>
		/// <typeparam name="_Pr">The type of the predicate object, method or lambda</typeparam>
		/// <returns>A reference to this list (used for chaining calls).</returns>
		template<class _Pr>
		inline array<_Ty>& orderby(_Pr pred) {
			::std::sort(this->begin(), this->end(), pred);
			return *this;
		}

		/// <summary>Performs a join on the current list and the provided list. Unlike <see cref="JOIN"/> it requires the <see cref="INTO"/> macro to create a merger of the two paired elements.</summary>
		/// <param name="type">The type contained within the list to be joined.</param>
		/// <param name="merge_type">The new type to be created for each joined pair</param>

		/// <summary>
		/// Performs a join on the current list and the provided list and performs a merge of the paired items.
		/// </summary>
		/// <param name="arr">Array to be joined with this one.</param>
		/// <param name="merge">Lambda that performs the merge.</param>
		/// <param name="on">Conditional lambda that determines if two elements should be paired.</param>
		/// <typeparam name="_Ty2">The type contained in the array being joined.</typeparam>
		/// <typeparam name="_Ret">The type to be returned in the new merged array.</typeparam>
		/// <returns>New array of merged items.</returns>
		template<class _Ty2, class _Ret>
		array<_Ret> join(const array<_Ty2> &arr, const merger<_Ty2, _Ret> merge, const comparison<_Ty2> on) const {
			array<_Ret> merged;
			for (auto first : *this) {
				for (auto second : arr) {
					if (on(first, second)) {
						merged.push_back(merge(first, second));
					}
				}
			}
			return merged;
		}
		/// <summary>
		/// Performs a join on the current list and the provided list and pairs the items into <see cref="linq::merge_pair"/>.
		/// </summary>
		/// <param name="arr">Array to be joined with this one.</param>
		/// <param name="on">Conditional lambda that determines if two elements should be paired.</param>
		/// <typeparam name="_Ty2">The type contained in the array being joined.</typeparam>
		/// <returns>New array of paired items.</returns>
		template<class _Ty2>
		inline array<merge_pair<_Ty, _Ty2>> join(const array<_Ty2> &arr, const comparison<_Ty2> on) const {
			return join<_Ty2, merge_pair<_Ty, _Ty2>>(arr, [](auto left, auto right)->merge_pair<_Ty, _Ty2> { return { left, right }; }, on);
		}
	};

	/// <summary>
	/// Helper function for converting a <see cref="std::vector"/> to an <see cref="linq::array"/>.
	/// </summary>
	template<class _Ty> inline array<_Ty> from(const ::std::vector<_Ty> &vec) { return array<_Ty>(vec); }
	/// <summary>
	/// Helper function for converting a C-style array pointer into an <see cref="linq::array"/>.
	/// </summary>
	template<class _Ty>
	inline array<_Ty> from(const _Ty *c_arr, const size_t &count) {
		return array<_Ty>(c_arr, c_arr + count);
	}

#ifdef LINQ_USE_MACROS
	/// <summary>
	/// dummy function to make things uniform with the <see cref="FROM"/> macro
	/// </summary>
	template<class _Ty> inline array<_Ty> from(const array<_Ty> &arr) { return arr; }

	/// <summary>All LINQ queries must be finished with this macro.</summary>
#define END )

/// <summary>Used with <see cref="ORDERBY"/> as a predefined ascending sort.</summary>
#define ASCENDING ::linq::ascending
/// <summary>Used with <see cref="ORDERBY"/> as a predefined descending sort.</summary>
#define DESCENDING ::linq::descending

// ELEM, LEFT, & RIGHT are simple macros to be used where needed for parameter names.
// This allows for easy name changing without the risk of missing a name.
#define __LINQ_ELEM__ item
#define __LINQ_LEFT__ left
#define __LINQ_RIGHT__ right

/// <summary>Performs an item selection which is expected to transform the data in some way.</summary>
/// <param name="type">The new type being created and returned for the new array.</param>
/// <example>SELECT(int) { return str.size(); }</example>
#define SELECT(type) END.select<type>([](auto __LINQ_ELEM__) -> type

/// <summary>Performs a conditional for filtering the list.</summary>
/// <example>WHERE { return item > 10; }</example>
#define WHERE END.where([](auto __LINQ_ELEM__) -> bool

/// <summary>Performs a sort on the list. Can be given the predefined ASCENDING or DESCENDING for comparable types, or any predicate.</summary>
/// <param name="pred">Predicate used for comparing the two items for which should go first.</param>
/// <example>ORDERBY(ASCENDING)</example>
/// <example>ORDERBY(PREDICATE { return left > right; })</example>
#define ORDERBY(pred) END.orderby(pred

/// <summary>Helper for creating a predicate lambda for the ORDERBY macro.</summary>
/// <see cref="ORDERBY"/>
#define PREDICATE [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> bool

/// <summary>Performs a join on the current list and the provided list. This results in an array of <see cref="linq::merge_pair"/>.</summary>
/// <param name="type">The type contained within the list to be joined.</param>
/// <example>JOIN(int) nums</example>
#define PAIR_JOIN(type) END.join<type>(

/// <summary>Performs a join on the current list and the provided list. Unlike <see cref="JOIN"/> it requires the <see cref="INTO"/> macro to create a merger of the two paired elements.</summary>
/// <param name="type">The type contained within the list to be joined.</param>
/// <param name="merge_type">The new type to be created for each joined pair</param>
/// <example>MERGE_JOIN(int, myPair) nums INTO { return { left, right }; }</example>
#define MERGE_JOIN(type, merge_type) END.join<type, merge_type>(

/// <summary>
/// (Optional) Performs a conditional check between the left and right during either a <see cref="JOIN"/> or <see cref="MERGE_JOIN"/>.
/// Note: For <see cref="MERGE_JOIN"/>, must come after the <see cref="INTO"/> statement
/// </summary>
/// <example>JOIN(obj) myObjects ON { return left.ref_id == right.id; }</example>
#define ON , [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> bool

/// <summary>Used with <see cref="MERGE_JOIN"/> to define how the paired items are merged into a new type.</summary>
/// <param name="type">The type to be created from the merged items.</param>
/// <example>MERGE_JOIN(int, myPair) nums INTO { return { left, right }; }</example>
#define INTO(type) , [](auto __LINQ_LEFT__, auto __LINQ_RIGHT__) -> type

/// <summary>Simple helper macro for calling <see cref="linq::from"/> and needed for starting a LINQ query when using the defined macros.</summary>
/// <example>FROM(vecOfItems)</example>
/// <example>FROM(cArray, count)</example>
#define FROM(param, ...) ::linq::from(param,##__VA_ARGS__


/*** Custom lambda expression alternatives ***/

/// <summary>Performs an item selection which is expected to transform the data in some way, using a fully defined lambda</summary>
/// <param name="type">The new type being created and returned for the new array.</param>
/// <param name="lambda_item_type">Lambda of the form: _Ret(_Ty item)</param>
/// <example>SELECT_L(int) [](auto item) -> int { return str.size(); }</example>
#define SELECT_L(type) END.select<type>(

/// <summary>Performs a conditional for filtering the list using a fully defined lambda.</summary>
/// <example>WHERE_L [](auto item) -> bool { return item > 10; }</example>
#define WHERE_L END.where(

/// <summary>
/// (Optional) Performs a conditional check between the left and right during either a <see cref="JOIN"/> or <see cref="MERGE_JOIN"/> using a fully defined lambda..
/// Note: For <see cref="MERGE_JOIN"/>, must come after the <see cref="INTO"/> statement
/// </summary>
/// <example>JOIN(obj) myObjects ON_L [](auto left, auto right) -> bool { return left.ref_id == right.id; }</example>
#define ON_L , 

/// <summary>Used with <see cref="MERGE_JOIN"/> to define how the paired items are merged into a new type using a fully defined lambda.</summary>
/// <param name="type">The type to be created from the merged items.</param>
/// <example>MERGE_JOIN(int, myPair) nums INTO_L [](auto left, auto right) -> myPair { return { left, right }; }</example>
#define INTO_L(type) , 

#endif
}

#endif
