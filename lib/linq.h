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

#include <exception>
#include <stdexcept>

namespace linq {
	namespace core {
		// TEMPLATE STRUCT more
		template<class _Ty = void>
		struct more
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			/// <summary>
			/// Performs a comparison of the two objects to see if the left is greater than the right.
			/// </summary>
			/// <param name="_Left">Left of the greater comparison.</param>
			/// <param name="_Right">Right of the greater comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is greater than the <paramref name="_Right"/>; otherwise false.</returns>
			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator> to operands
				return (_Left > _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION more
		template<>
		struct more<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			/// <summary>
			/// Performs a generic comparison of the two objects to see if the left is greater than the right.
			/// </summary>
			/// <param name="_Left">Left of the greater comparison.</param>
			/// <param name="_Right">Right of the greater comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is greater than the <paramref name="_Right"/>; otherwise false.</returns>
			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator> to operands
				return (static_cast<_Ty1&&>(_Left) > static_cast<_Ty2&&>(_Right));
			}
		};
		// TEMPLATE STRUCT less
		template<class _Ty = void>
		struct less
		{	// functor for operator<
			typedef _Ty first_argument_type;
			typedef _Ty second_argument_type;
			typedef bool result_type;

			/// <summary>
			/// Performs a comparison of the two objects to see if the left is lesser than the right.
			/// </summary>
			/// <param name="_Left">Left of the lesser comparison.</param>
			/// <param name="_Right">Right of the lesser comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is lesser than the <paramref name="_Right"/>; otherwise false.</returns>
			constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
			{	// apply operator< to operands
				return (_Left < _Right);
			}
		};
		// TEMPLATE STRUCT SPECIALIZATION less
		template<>
		struct less<void>
		{	// transparent functor for operator<
			typedef int is_transparent;

			/// <summary>
			/// Performs a generic comparison of the two objects to see if the left is lesser than the right.
			/// </summary>
			/// <param name="_Left">Left of the lesser comparison.</param>
			/// <param name="_Right">Right of the lesser comparison.</param>
			/// <returns>True if the <paramref name="_Left"/> is lesser than the <paramref name="_Right"/>; otherwise false.</returns>
			template<class _Ty1, class _Ty2>
			constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
				-> decltype(static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right))
			{	// transparently apply operator< to operands
				return (static_cast<_Ty1&&>(_Left) < static_cast<_Ty2&&>(_Right));
			}
		};

		/// <summary>
		/// Basic lightweight structure which holds two objects of differing types.
		/// </summary>
		/// <typeparam name="_Left">The first object</typeparam>
		/// <typeparam name="_Right">The second object</typeparam>
		template<class _Left, class _Right>
		struct merge_pair {
			_Left left;
			_Right right;
		};
	}

	/// <summary>predicate used for sorting objects in ascending order</summary>
	core::less<> ascending;
	/// <summary>predicate used for sorting objects in descending order</summary>
	core::more<> descending;

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
		/// <summary>
		/// Delegate used for transforming the provided data into a new state which is returned by the call.
		/// </summary>
		template<class _Ret>
		using conversion = ::std::function<_Ret(const _Ty&)>;
		/// <summary>
		/// Delegate for applying conditions on the provided item and returning a bool result.
		/// </summary>
		typedef ::std::function<bool(const _Ty&)> conditional;
		/// <summary>
		/// Delegate for comparing two items and returning a bool result.
		/// </summary>
		template<class _Ty2>
		using comparison = ::std::function<bool(const _Ty&, const _Ty2&)>;
		/// <summary>
		/// Delegate that is expected to merge two items into a new item which is then returned by the call.
		/// </summary>
		template<class _Ty2, class _Ret>
		using merger = ::std::function<_Ret(const _Ty&, const _Ty2&)>;

	public:
		/// <summary>
		/// Performs an item selection which is expected to transform the data in some way and return a new array.
		/// </summary>
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
		array<_Ty> where(const conditional &condition) const {
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
		inline array<_Ty>& orderby(const _Pr &pred) {
			::std::sort(this->begin(), this->end(), pred);
			return *this;
		}

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
		array<_Ret> join(const array<_Ty2> &arr, const merger<_Ty2, _Ret> &merge, const comparison<_Ty2> &on) const {
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
		/// Performs a join on the current list and the provided list and pairs the items into <see cref="linq::core::merge_pair"/>.
		/// </summary>
		/// <param name="arr">Array to be joined with this one.</param>
		/// <param name="on">Conditional lambda that determines if two elements should be paired.</param>
		/// <typeparam name="_Ty2">The type contained in the array being joined.</typeparam>
		/// <returns>New array of paired items.</returns>
		template<class _Ty2>
		inline array<core::merge_pair<_Ty, _Ty2>> join(const array<_Ty2> &arr, const comparison<_Ty2> &on) const {
			return join<_Ty2, core::merge_pair<_Ty, _Ty2>>(arr, [](auto left, auto right)->core::merge_pair<_Ty, _Ty2> { return { left, right }; }, on);
		}

		/// <summary>
		/// Returns the first element in the array or throws <see cref="std::logic_error"/> if the array is empty.
		/// </summary>
		/// <returns>The first element of the array; otherwise an error is thrown.</returns>
		_Ty first() const {
			if (this->size() > 0) {
				return *(this->begin());
			}
			throw ::std::logic_error("array is empty");
		}
		/// <summary>
		/// Searches the array for the first item which satisfies the provided condition.
		/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
		/// <returns>The first item found which satisfies the condition; otherwise an error is thrown.</returns>
		_Ty first(const conditional &condition) const {
			for (auto it = this->begin(), end = this->end(); it != end; it++) {
				if (condition(*it)) {
					return *it;
				}
			}
			throw ::std::logic_error("no elements match the given conditional");
		}
		/// <summary>
		/// Searches the array for the first item which satisfies the provided condition.
		/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
		/// <returns>The first item found which satisfies the condition; otherwise the provided <paramref name="defaultValue"/>.</returns>
		inline _Ty first_or_default(_Ty defaultValue, const conditional &condition) const {
			try { return first(condition); }
			catch (...) { return defaultValue; }
		}

		/// <summary>
		/// Returns the last element in the array or throws <see cref="std::logic_error"/> if the array is empty.
		/// </summary>
		/// <returns>The last element of the array; otherwise an error is thrown.</returns>
		_Ty last() const {
			size_t pos = this->size() - 1;
			if (pos >= 0) {
				return this->at(pos);
			}
			throw ::std::logic_error("array is empty");
		}
		/// <summary>
		/// Searches the array for the last item which satisfies the provided condition.
		/// Throws <see cref="std::logic_error"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <exception cref="std::logic_error">Thrown if no items satisfy the condition.</exception>
		/// <returns>The last item found which satisfies the condition; otherwise an error is thrown.</returns>
		_Ty last(const conditional &condition) const {
			for (auto it = this->rbegin(), end = this->rend(); it != end; it++) {
				if (condition(*it)) {
					return *it;
				}
			}
			throw ::std::logic_error("no elements match the given conditional");
		}
		/// <summary>
		/// Searches the array for the last item which satisfies the provided condition.
		/// Returns the provided <paramref name="defaultValue"/> if the condition is never satisfied.
		/// </summary>
		/// <param name="condition">Condition to be satisfied.</param>
		/// <param name="defaultValue">Default value to be returned if no items satisfy the condition.</param>
		/// <returns>The last item found which satisfies the condition; otherwise the provided <paramref name="defaultValue"/>.</returns>
		_Ty last_or_default(_Ty defaultValue, const conditional &condition) const {
			try { return last(condition); }
			catch (...) { return defaultValue; }
		}

		/// <summary>
		/// Helper method which converts this <see cref="linq::array"/> into a <see cref="std::vector"/>.
		/// </summary>
		/// <returns><see cref="std::vector"/> containing a copy of the elements of this <see cref="linq::array"/>.</returns>
		inline ::std::vector<_Ty> to_vector() const {
			return ::std::vector<_Ty>(*this);
		}
	};

	/// <summary>
	/// Helper function for converting a <see cref="std::vector"/> to an <see cref="linq::array"/>.
	/// </summary>
	/// <param name="vec">The <see cref="std::vector"/> to be converted to a <see cref="linq::array"/>.</param>
	/// <returns><see cref="linq::array"/> containing a copy of the elements from the provided <paramref name="vec"/>.</returns>
	template<class _Ty>
	inline array<_Ty> from(const ::std::vector<_Ty> &vec) { return array<_Ty>(vec); }
	/// <summary>
	/// Helper function for converting a C-style array pointer into an <see cref="linq::array"/>.
	/// </summary>
	/// <param name="c_arr">The pointer to a C-Style array to be converted to a <see cref="linq::array"/>.</param>
	/// <param name="size">The size of the C-Style array.</param>
	/// <returns><see cref="linq::array"/> containing a copy of the elements from the provided <paramref name="c_arr"/>.</returns>
	template<class _Ty>
	inline array<_Ty> from(const _Ty *c_arr, const size_t &size) {
		return array<_Ty>(c_arr, c_arr + size);
	}
}


#ifdef LINQ_USE_MACROS
#ifndef FROM
#include "linq-macros.h"
namespace linq {
	/// <summary>
	/// Pass-through function to make things uniform with the <see cref="FROM"/> macro.
	/// </summary>
	/// <param name="arr">The array to be passed through as a result.</param>
	/// <returns>The same provided <see cref="arr"/>.</returns>
	template<class _Ty>
	inline array<_Ty> from(const array<_Ty> &arr) { return arr; }
}
#endif
#endif

#endif
