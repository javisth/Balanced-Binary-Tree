#ifndef SYMTAB_H
#define SYMTAB_H

/* DO NOT CHANGE:  This file is used in evaluation 
 * Changing function signatures will result in a 25% deduction.
 * */

#include <iostream>
#include "Tree.h"
using namespace std;

template <class Whatever>
class SymTab : private Tree<Whatever> {
public:
       	using Tree<Whatever> :: Insert;
        using Tree<Whatever> :: Lookup;
        using Tree<Whatever> :: Remove;
        using Tree<Whatever> :: Write;
        using Tree<Whatever> :: Set_Debug_On;
        using Tree<Whatever> :: Set_Debug_Off;
};

#endif
