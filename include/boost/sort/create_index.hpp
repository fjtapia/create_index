//----------------------------------------------------------------------------
/// @file create_index.hpp
/// @brief test program of the creation and sorting of index
///
/// @author Copyright (c) 2017 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_CREATE_INDEX_HPP
#define __BOOST_SORT_CREATE_INDEX_HPP

#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include <boost/sort/common/util/traits.hpp>

namespace srtc = boost::sort::common;
namespace srt = boost::sort;
namespace srtu = srtc::util;

namespace boost
{
namespace sort
{
//
//---------------------------------------------------------------------------
/// @struct comp_index
///
/// @remarks this is the comparison object for pointers. Compare the objects
///          pointed by the iterators
//---------------------------------------------------------------------------
template<class Iter_t, class Compare = srtu::compare_iter<Iter_t> >
struct comp_index
{
    //----------------------------- Variables -----------------------
    Compare comp; // comparison object of the elements pointed by Iter_t

    //------------------------------------------------------------------------
    //  function : less_ptr_no_null
    /// @brief constructor from a Compare object
    /// @param C1 : comparison object
    //-----------------------------------------------------------------------
    comp_index (Compare C1 = Compare()): comp(C1) { };

    //------------------------------------------------------------------------
    //  function : operator ( )
    /// @brief Make the comparison of the objects pointed by T1 and T2, using
    //         the internal comp
    //
    /// @param  T1 : first iterator
    /// @param  T2 : second iterator
    /// @return bool result of the comparison
    //-----------------------------------------------------------------------
    bool operator( )(Iter_t T1, Iter_t T2) const
    {
        return comp(*T1, *T2);
    };
};
template<class iter_t>
using iter_index = typename std::vector < iter_t >::iterator;

//----------------------------------------------------------------------------
/// @fn void create_index (method, index, it_begin, end, comp)
/// @brief : this function permit to create an index of a set of data
///
/// @param[in] method Sorting algorithm used to create the index. This
///                   algorithm can be any of the Boost Sort library, single
///                   thread or parallel, or any of the provided by the
///                   compilers. The only one requisite is to have 2 template
///                   parameters, first the iterator and second the comparison
///                   operator
/// @param[out] index Is merely a vector of iterators to the data set. The
///                   function return this vctor sorted according with the
///                   comparison opoerator provided
/// @param[in] it_begin iterator to the first element of the tada to index
/// @param[in] it_end after the last iterator of the data
/// @param[in] comp comparison operator
///
/// @remarks : the method is a template parameter of the sorting algorithm. But
/// this algorithm don't sort the elements pointed by iter_t, sort iterators.
/// Due this, their template parameters can't be iter and compare. Must be
/// iter_index <iter_t>, and comp_index <iter_t, compare>.
/// By example :
///
///     typedef std::vector <data>::iterator iter_t;
///     typedef std::less <data>			 compare;
///
///		std::vector <data> vdata;

/// If you sort directly the data
///
///  std::stable_sort <iter_index <iter_t>, comp_index <iter_t, compare>>
//-----------------------------------------------------------------------------
template  <class func, class iter_t, class compare = srtu::compare_iter<iter_t>>
void create_index (func method,std::vector <iter_t> & index,
		           iter_t it_begin, iter_t it_end, compare comp = compare())
{
	auto nelem = it_end - it_begin;
	assert (nelem >= 0);
	if (nelem < 2) return;

	// filling the index with the itertors
	index.clear();
	index.reserve (nelem);
	for (auto it = it_begin; it != it_end; ++it)
		index.emplace_back (it);

	// sorting the index with the method algorithm
	comp_index <iter_t, compare> cmpx (comp);
	method (index.begin(), index.end(), cmpx);
};
//----------------------------------------------------------------------------
}; // end namespace sort
}; // end namespace sort
//----------------------------------------------------------------------------
#endif

