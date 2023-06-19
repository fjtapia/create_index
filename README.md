
<h1>Boost Sort create_index
<h3> <a href="https://github.com/fjtapia/sort_parallel">https://github.com/fjtapia/sort_parallel</a> </h3>

<h3>PRELIMINARY</h3>

The goal of this function is to provide the **Boost Sort library** with the ability to create indexes on a data set. These indices allow us to have a data set ordered by different concepts simultaneously, without modifying said data set.

| Name | Age | Weight |
| --- | --- | --- |
| John | 45 | 86.4 |
| Anna | 37 | 57.2|
| Peter| 32 | 74.3 |
| William | 52 | 76.1 |

For example, in this case, we can create an index of name, another of age, and another of weight, which allows us do binary searches by the three concepts.



<h3>DESCRIPTION</h3>

An index is a vector of iterators. The index can be created with any sorting algorithm, single thread or parallel, being able to use not only the Boost library algorithms but also those provided by libraries and compilers. These algorithms must have the following format


    template <class iter_t, class compare>
    void algorithm (iter_t it_begin, iter_t it_end, compare comp)

<h3>USAGE</h3>

The create_index function has the following format

    template <class func, class iter_t, class compare = srtu::compare_iter< iter_t >>
    void create_index (func method, std::vector <iter_t> & index,
		            iter_t it_begin, iter_t it_end, compare comp = compare ())


<h5>PARAMETERS</h5>

**func :** The first parameter is the sorting algorithm to use. Usually, such algorithms are template functions, so the type of such template parameters must be specified, to pass it as a parameter.

This sorting algorithm don't sort iter_t elements. Sort iterators to the iter_t  elements, and the comparison object don't compare the pointed by two iter_t elements, compare the pointed by the iterators to the iter_t.

Due this the template parameters of the sorting algorithms are no iter_t and compare. The correct template parameters are obtained form iter_t and compare with two templates


The correct types are obtained by two functions **iter_index <iter_t>** and **comp_index <iter_t, compare>**. Therefore, the format of the said first parameter would be, if you want to use the std::sort algorithm

    std::sort <iter_index <iter_t>, comp_index <iter_t, compare>

**index :** This is a vector if  iter_t elements which is the index, where we will obtain the ordered iterators,

***The other 3 parameters are identical to those used in direct sorting.***

**it_begin :** The third parameter the iter_t to the first element of the data set.

**it_end :** The fourth parameter is the iter_t to the element after the last.

**comp :** The fifth parameter is the object with which we are going to compare


<h3>EXAMPLE</h3>

The description may seem confusing, but its use is extremely simple.

We see it with the following example

    #include <boost/sort/create_index.hpp>
    #include <boost/sort/sort.hpp>

    namespace srt = boost::sort;

    //------------------------------------------------ --------------------------
    // We have a data type with 3 elements, number, name, and weight
    //------------------------------------------------ --------------------------
    structure member
    {   uint32_t    num;
        std::string name;
        float       weight;
    };
    //------------------------------------------------ ------------------------
    // object to compare by name
    //------------------------------------------------ ------------------------
    struct_comp_name
    {   bool operator() (const member &m1, const member &m2) const
        {   return (m1.name < m2.name);
        };
    };
    //------------------------------------------------ ------------------------
    // Object to compare by weight
    //------------------------------------------------ -----------------------
    struct_comp_weight
    {   bool operator() (const member &m1, const member &m2) const
        {   return (m1.weight < m2.weight);
        };
    };
    //------------------------------------------------ -----------------------
    // printing operator
    //------------------------------------------------ -----------------------
    std::ostream &operator<< (std::ostream &out, const member &m)
    {   out << m.num << " - " << m.name << " - " << m.weight << std::endl;
        return out;
    };

    int main(void)
    {
        //----------------------- definitions ----------------------- ----------
        typedef typename std::vector <member>::iterator 	iter_t;

        //------------------------------------------------ ----------------------
        // Type of the first template parameter of the algorithm. It is the same
        // in the two indexes
        //------------------------------------------------ ----------------------
        typedef srt::iter_index <iter_t> it_idx;

        //------------------------------------------------------ ----------------
        // The data set is sorted by number. We want to create indexes to sort by
        // name and weight
        //------------------------------------------------ -----------------------
        std::vector <member> VM = { {1, "Peter", 85.6},
                                    {2, "Hanna", 63.4},
                                    {3, "John", 83.6},
                                    {4, "Elsa", 56.6}};

        //------------------------------------------------ -----------------------
        // Creation of the two vectors of iter_t. Ix_name to be ordered by name
        // and Ix_weight ordered by weight
        //------------------------------------------------ ----------------------
        std::vector <iter_t> Ix_name, Ix_weight;

        //------------------------------------------------ -----------------------
        // type of the second template parameter of the algorithm, in the index
        // Ix_name, sorted by name
        //------------------------------------------------ -----------------------
        typedef srt::comp_index <iter_t, comp_name>     comp_name_idx;

        //------------------------------------------------ ----------------------
        // creation of the index Ix_name
        //------------------------------------------------ ---------------------
        srt::create_index (srt::spinsort <it_idx, comp_name_idx>, Ix_name,
    		        VM.begin(), VM.end(), comp_name());

        //------------------------------------------------ -----------------------
        // type of the second template parameter of the algorithm, in the index
        // Ix_weight, sorted by weight
        //------------------------------------------------ -----------------------
        typedef srt::comp_index <iter_t, comp_weight> 	comp_weight_idx;

        //------------------------------------------------ -----------------------
        // Creation of the index Ix_weight
        //------------------------------------------------ -----------------------
        srt::create_index   (std::stable_sort <it_idx, comp_weight_idx>, Ix_weight,
    		        VM.begin(), VM.end(), comp_weight());


        std::cout << "Printing sorted by number \n";
        for (auto it = VM.begin(); it != VM.end(); ++it)
        std::cout << (*it);
        std::cout<<std::endl;

        std::cout << "Printing sorted by name \n";
        for (auto it = Ix_name.begin(); it != Ix_name.end(); ++it)
        std::cout << (*(*it));
        std::cout<<std::endl;

        std::cout << "Printing sorted by weight\n";
        for (auto it = Ix_weight.begin(); it != Ix_weight.end(); ++it)
        std::cout << (*(*it));
        std::cout<<std::endl;

        return 0;
    };

The result that appears on the screen is

	Printing sorted by number
	1 - Peter - 85.6
	2 - Hanna - 63.4
	3 - John - 83.6
	4 - Elsa - 56.6

	Printing sorted by name
	4 - Elsa - 56.6
	2 - Hanna - 63.4
	3 - John - 83.6
	1 - Peter - 85.6

	Printing sorted by weight
	4 - Elsa - 56.6
	2 - Hanna - 63.4
	3 - John - 83.6
	1 - Peter - 85.6



<h2>Author and Copyright</h2>

Copyright 2023  [Francisco Tapia *(fjtapia@gmail.com)* ](mail:fjtapia@gmail.com)
Distributed under the [Boost Software License, Version 1.0. ](http://www.boost.org/LICENSE_1_0.txt)  (See http://www.boost.org/LICENSE_1_0.txt)
