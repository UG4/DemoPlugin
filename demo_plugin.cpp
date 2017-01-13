/*
 * Copyright (c) 2011-2017:  G-CSC, Goethe University Frankfurt
 * Author: Sebastian Reiter
 * 
 * This file is part of UG4.
 * 
 * UG4 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 3 (as published by the
 * Free Software Foundation) with the following additional attribution
 * requirements (according to LGPL/GPL v3 §7):
 * 
 * (1) The following notice must be displayed in the Appropriate Legal Notices
 * of covered and combined works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (2) The following notice must be displayed at a prominent place in the
 * terminal output of covered works: "Based on UG4 (www.ug4.org/license)".
 * 
 * (3) The following bibliography is recommended for citation and must be
 * preserved in all covered files:
 * "Reiter, S., Vogel, A., Heppner, I., Rupp, M., and Wittum, G. A massively
 *   parallel geometric multigrid solver on hierarchically distributed grids.
 *   Computing and visualization in science 16, 4 (2013), 151-164"
 * "Vogel, A., Reiter, S., Rupp, M., Nägel, A., and Wittum, G. UG4 -- a novel
 *   flexible software system for simulating pde based models on high performance
 *   computers. Computing and visualization in science 16, 4 (2013), 165-179"
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 */

#include "bridge/util.h"
#include "lib_algebra/cpu_algebra_types.h"

using namespace std;
using namespace ug::bridge;

namespace ug{
namespace DemoPlugin{

///	Sparse matrix type as defined in lib_algebra/cpu_algebra/sparsematrix.h
typedef CPUAlgebra::matrix_type matrix_type;
///	Vector type as defined in lib_algebra/cpu_algebra/vector.h
typedef CPUAlgebra::vector_type vector_type;


/// Prints a welcome message
void DemoPluginSaysHi()
{
	cout << "Hi from DemoPlugin" << endl;
}


///	Calculates the sum of all coefficients of the given vector and returns it
number DemoVecCompSum(vector_type& v)
{
	number sum = 0;
	for(size_t i = 0; i < v.size(); ++i){
		sum += v[i];
	}
	return sum;
}

///	Calculates the sum of all coefficients of the given matrix and returns it.
/** This implementation has O(n^2), even if the underlying matrix has a sparse
 * structure. Have a look at DemoMatCompSumFast for a O(n) variant (for sparse matrices).
 */
number DemoMatCompSumSlow(matrix_type& m)
{
//	see DemoMatCompSumFast for a more efficient implementation
	number sum = 0;
	for(size_t irow = 0; irow < m.num_rows(); ++irow){
		for(size_t icol = 0; icol < m.num_cols(); ++icol){
			sum += m(irow, icol);
		}
	}
	return sum;
}


//	Calculates the sum of all coefficients of the given matrix and returns it.
/** This implementation has O(n) if the underlying matrix has a sparse structure.
 */
number DemoMatCompSumFast(matrix_type& m)
{
//	here we iterate over all rows and for each row over all non-zero entries
//	of that row, using the row_iterator type.
	number sum = 0;
	for(size_t irow = 0; irow < m.num_rows(); ++irow){
		for(matrix_type::row_iterator icur = m.begin_row(irow);
			icur != m.end_row(irow); ++icur)
		{
			sum += icur.value();
		}
	}
	return sum;
}


///	A basic class used to demonstrate registration of classes for scripting
class DemoClass {
public:
	DemoClass()	: m_value(0)		{}
	void add_to_value(number a)		{m_value += a;}
	void set_value(number v)		{m_value = v;}
	number get_value() const		{return m_value;}
private:
	number m_value;
};


} // end namespace DemoPlugin


/**
 * This function is called when the plugin is loaded.
 * Make sure that it's name is 'InitUGPlugin_YOURPLUGINNAME',
 * where YOURPLUGINNAME has to match the name you chose for your plugin
 * in the plugin's CMakeLists.txt file.
 */
extern "C" void
InitUGPlugin_DemoPlugin(Registry* reg, string grp)
{
	using namespace DemoPlugin;

	grp.append("DemoPlugin");

	try{
	//	register some functions
		reg->add_function("DemoPluginSaysHi", &DemoPluginSaysHi, grp);
		reg->add_function("DemoVecCompSum", &DemoVecCompSum, grp);
		reg->add_function("DemoMatCompSumSlow", &DemoMatCompSumSlow, grp);
		reg->add_function("DemoMatCompSumFast", &DemoMatCompSumFast, grp);

	//	register a class
		reg->add_class_<DemoClass>("DemoClass", grp, "Class to demonstrate class-registration and scripting.")
			.add_method("add_to_value", &DemoClass::add_to_value)
			.add_method("set_value", &DemoClass::set_value)
			.add_method("get_value", &DemoClass::get_value)
			.add_constructor()
			.set_construct_as_smart_pointer(true);
	}
	UG_REGISTRY_CATCH_THROW(grp);
}

}// namespace ug
