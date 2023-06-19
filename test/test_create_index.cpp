//----------------------------------------------------------------------------
/// @file test_create_index.cpp
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
#include <boost/sort/create_index.hpp>
#include <boost/sort/sort.hpp>

namespace srtc = boost::sort::common;
namespace srt = boost::sort;
namespace srtu = srtc::util;

using boost::sort::create_index;
using boost::sort::spinsort;

//---------------------------------------------------------------------------
//        F U N C T I O N    T E S T 1
//---------------------------------------------------------------------------
int test1 (void)
{
	//--------------------------------- definitions --------------------------
	typedef typename std::vector <uint64_t>::iterator 	iter_t;
	typedef std::less <uint64_t>						compare;
	typedef srt::comp_index <iter_t, compare>			cmp_idx;
	typedef srt::iter_index <iter_t>					it_idx;
	const size_t NELEM = 100;


    std::mt19937_64 my_rand (0);
    std::vector <uint64_t> vnumbers;
    for (size_t i =0 ; i < NELEM; ++i)
    	vnumbers.emplace_back (my_rand() % 1000);

    std::vector <iter_t> index;

    srt::create_index (spinsort <it_idx, cmp_idx>, index,
    		           vnumbers.begin(), vnumbers.end());

    for ( uint32_t i =0 ; i < index.size() ;++i)
    {
    	if ((i % 5) == 0) std::cout<<std::endl;
    	std::cout<<*(index[i])<<" , ";
    };
    std::cout<<std::endl<<std::endl;
    return 0;
};
//---------------------------------------------------------------------------
//        F U N C T I O N    T E S T 2
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
// We have a data type with 3 elements, number, name and weight
//--------------------------------------------------------------------------
struct member
{   uint32_t num;
    std::string name;
    float weight;
};
//-------------------------------------------------------------------------
// Object to compare by num
//-------------------------------------------------------------------------
struct comp_num
{   bool operator() (const member &m1, const member &m2) const
    {   return (m1.num < m2.num);
    };
};
//------------------------------------------------------------------------
// object to compare by name
//------------------------------------------------------------------------
struct comp_name
{   bool operator() (const member &m1, const member &m2) const
    {   return (m1.name < m2.name);
    };
};
//------------------------------------------------------------------------
// Object to compare by weight
//-----------------------------------------------------------------------
struct comp_weight
{   bool operator() (const member &m1, const member &m2) const
    {   return (m1.weight < m2.weight);
    };
};
//-----------------------------------------------------------------------
// printing operator
//-----------------------------------------------------------------------
std::ostream &operator<< (std::ostream &out, const member &m)
{   out << m.num << " - " << m.name << " - " << m.weight << std::endl;
    return out;
};

int test2 (void)
{
	//------------------------ definitions ---------------------------------
	typedef typename std::vector <member>::iterator 	iter_t;

	//----------------------------------------------------------------------
	// Type of the first template parameter of the algorithm. It is the same
	// in all the two index
	//----------------------------------------------------------------------
	typedef srt::iter_index <iter_t>					it_idx;

    //------------------- ----- begin ---------------------------------------
    // The data set are sorted by number. We want create index to sort by
	// name and weight
	//-----------------------------------------------------------------------
    std::vector <member> VM = {{1, "Peter", 85.6},
                               {2, "Hanna", 63.4},
                               {3, "John", 83.6},
                               {4, "Elsa", 56.6}};

    //-----------------------------------------------------------------------
    // Creation of the two vector of iter_t. Ix_name to be ordered by name and
    // Ix_weight ordered by weight
    //----------------------------------------------------------------------
    std::vector <iter_t> Ix_name, Ix_weight;

    //-----------------------------------------------------------------------
    // type of the second template parameter of the algorithm, in the index
    // Ix_name, sorted by name
    //-----------------------------------------------------------------------
    typedef srt::comp_index <iter_t, comp_name>	comp_name_idx;

    //----------------------------------------------------------------------
    // creation of the index Ix_name
    //---------------------------------------------------------------------
    srt::create_index (spinsort <it_idx, comp_name_idx>, Ix_name,
    		      VM.begin(), VM.end(), comp_name ());

    //-----------------------------------------------------------------------
    // type of the second template parameter of the algorithm, in the index
    // Ix_weight, sorted by weight
    //-----------------------------------------------------------------------
    typedef srt::comp_index <iter_t, comp_weight>	comp_weight_idx;

    //-----------------------------------------------------------------------
    // Creation of the index Ix_weight
    //-----------------------------------------------------------------------
    srt::create_index (std::stable_sort <it_idx, comp_weight_idx>, Ix_weight,
    		      VM.begin(), VM.end(), comp_weight ());


    std::cout << "Printing sorted by number \n";
    for (auto it = VM.begin (); it != VM.end (); ++it)
        std::cout << (*it);
    std::cout<<std::endl;

    std::cout << "Printing sorted by name \n";
    for (auto it = Ix_name.begin (); it != Ix_name.end (); ++it)
        std::cout << (*(*it));
    std::cout<<std::endl;

    std::cout << "Printing sorted by weight \n";
    for (auto it = Ix_weight.begin (); it != Ix_weight.end (); ++it)
        std::cout << (*(*it));
    std::cout<<std::endl;

    return 0;
};
//---------------------------------------------------------------------------
//        F U N C T I O N     T E S T  3
//
// This function is the test with a a big number of elements
//---------------------------------------------------------------------------

int test3 (void)
{

	typedef std::less<uint64_t> 						Compare;
	typedef typename std::vector <uint64_t>::iterator 	iter_t;
	typedef srt::comp_index <iter_t, Compare>			comp_idx;
	typedef srt::iter_index <iter_t>					it_idx;
	const size_t NELEM = 1000000;

    std::mt19937_64 my_rand (0);

    std::vector<uint64_t> A, B;
    A.reserve (NELEM);

    for (size_t i =0 ; i < NELEM; ++i)
        A.emplace_back (my_rand() % 1000000  ) ;

    //------------------------------------------------------------------------
    // this is due to the order in the compiler. First need the instantiation
    // of the algorithm, and after the substitution of the parameters in the
    // function
    //------------------------------------------------------------------------
    std::vector <iter_t> index;
    index.reserve (NELEM);

    srt::create_index (std::stable_sort <it_idx, comp_idx>, index,
    		           A.begin(), A.end());
    //create_index (spinsort <it_idx, comp_idx>, A.begin(), A.end(),
    //		      Compare(), index);
    B = A;
    std::sort (B.begin(), B.end());

    for ( size_t i =0 ; i < A.size() ; ++i)
        assert (B[i] == *(index[i]));

    return 0 ;
};

int main (void)
{
    test1 ();
    test2 ();
    test3 ();
    std::cout<<"------------------------- end -----------------------------\n";
    return 0;
};


