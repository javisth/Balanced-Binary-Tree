/****************************************************************************

                                                Javisth Chabria
                                                CSE 12, Spring
                                                May 30th
                                                cs12xaq
                              Assignment 8

File Name:      Tree.c
Description:    This program implements a balanced binary tree.

****************************************************************************/

#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);
//=========================================================================
// struct TNode
//
// Description: Implements the node of the binary search tree data structure.
//     Each node contains two children, the left child which contains
//     data "less than" the data of the current node, and the right child
//     which contains data "greater than" the data of the current node.
//
// Data Fields:
//     data (Whatever)  - holds the data stored in the current node
//     left (TNode<Whatever> *left)  - the left child
//     right (TNode<Whatever> *right) - the right child
//     balance (long) - stores balance of tree
//     height (long) - stores height of tree
//     occupancy (&long) - stores occupancy of tree
//     tree_count (&long) - stores count of tree
//
// Public functions:
//     delete_AllNodes - deletes all the children of the current node
//     Insert - inserts a data into the current node or to one of the
//              children of the current node
//     ReplaceAndRemoveMax - deletes a node and replaces it
//     Remove - Removes a node
//     setHeightAndBalance - Sets height and balance of tree
//     Write - display the current node
//     Write_AllNodes - display the current node and all its children
//
//==========================================================================


template <class Whatever>
struct TNode {
        long balance;
        Whatever data;
        long height;
        TNode<Whatever> * left;
        long & occupancy;
        TNode<Whatever> * right;
        unsigned long & tree_count;
 /*---------------------------------------------------------------------------
Function Name:                TNode
Purpose:                      This function is a TNode constructor
Description:                  This function is a TNode constructor
Input:                        element, theTree
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/       
        TNode (const Whatever & element, Tree<Whatever> & theTree)
                : balance (0), data (element), height (0), left (0), 
                occupancy (theTree.occupancy), right (0), 
                tree_count (theTree.tree_count) {
                /* incrementing tree's occupancy */
		occupancy ++;
        }
/*---------------------------------------------------------------------------
Function Name:                TNode
Purpose:                      This function is the TNode constructor
Description:                  This function is the TNode constructor
Input:                        element, parentTNode
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/  
      
        TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
                occupancy (parentTNode.occupancy), right (0), 
                tree_count (parentTNode.tree_count) {
                /* incrementing node's occupancy */
		occupancy ++;
        }

/*---------------------------------------------------------------------------
Function Name:                ~TNode
Purpose:                      This function is the TNode destructor
Description:                  This function is the TNode destructor
Input:                        None.
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

        ~TNode (void) {
                /* decrementing node's occupancy */
		occupancy --;
        }

/*---------------------------------------------------------------------------
Function Name:                delete_AllNodes
Purpose:                      This function deletes all nodes in the tree
Description:                  This function deletes all nodes in the tree
Input:                        None.
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

        void delete_AllTNodes (void) {
                /* checking if current node is empty */
		if ( this == NULL )
		{
			delete (this);
			return;
		}
		/* checking if left node is empty */
		if (this->left != NULL)
		{
			this->left -> delete_AllTNodes();
		}
		/* checking if right node is empty */
		if (this->right != NULL)
		{
			this->right -> delete_AllTNodes();
		}
		/* deleting the node */
		delete (this);
        }

/*---------------------------------------------------------------------------
Function Name:                Insert
Purpose:                      This function inserts an element in the tree
Description:                  This function inserts an element in the tree
Input:                        element, PointerInParent
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/



        unsigned long Insert (const Whatever & element, 
                              TNode<Whatever> *& PointerInParent);

        // OPTIONAL TNode :: Lookup => uncomment if implementing recursively
        // unsigned long Lookup(Whatever & element) const;

/*---------------------------------------------------------------------------
Function Name:                ReplaceAndRemoveMax
Purpose:                      This function replaces and removes element in tree
Description:                  This function replaces and removes element in tree
Input:                        targetTNode, PointerInParent
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

        void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
                TNode<Whatever> *& PointerInParent) {
               /* debug messages */
	       if(Tree<Whatever> :: debug)
		cerr << TREE << tree_count << CHECK
		<<(const char *) data << "]\n";

	       /* if on right node go right all the way */
	       if(right){
		right->ReplaceAndRemoveMax(targetTNode, right);

                if(Tree<Whatever> :: debug)
			cerr << TREE << tree_count << UPDATE
			<< (const char *) data <<"]\n";
		SetHeightAndBalance(PointerInParent);
	       }
	       /* if on left node go left all the way */
	       else{
			if(Tree<Whatever> :: debug)
				cerr << TREE << tree_count <<REPLACE
				<<(const char *)data <<"]\n";

			targetTNode.data = data;
			PointerInParent = left;
			delete this;
			return;
	       }
        }

/*---------------------------------------------------------------------------
Function Name:                Remove
Purpose:                      This function removes a node from tree
Description:                  This function removes a node from tree
Input:                        elementTNode, PointerInParent
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

        unsigned long Remove (TNode<Whatever> & elementTNode, 
                TNode<Whatever> *& PointerInParent,
                long fromSHB = FALSE) {
                
		/* checking if element to remove matches the data */
		if ( elementTNode.data == data )
		{

			/* if the node is a leaf set parent pointer to null
			 * before deleting*/
			if ( !left && !right )
			{
				/* set the pointer to the parent to null */
				PointerInParent = NULL;
				elementTNode.data = data;
				delete this;
			}
			/* if the node doesn't have a left child but has right
			 * child set parent pointer to right child before 
			 * deleting it */
			else if ( !left ){
			        
				PointerInParent = right;
				elementTNode.data = data;
				delete this;
			}
                        /* if the node doesn't have a right child but has a left
			 * child set parent pointer to right child before
			 * deleting it */
			else if ( !right ) 
			{
				PointerInParent = left;
				elementTNode.data = data;
				delete this;
			}
			/* if the node has two children call RARM on left */
			else {
				elementTNode.data = data;
				left -> ReplaceAndRemoveMax(*this,left);
			}
			     return 1;
		}
		/* if elements TNode is greater than data */
		else if(elementTNode.data > data){
			/* if no right child found */
			if(!right){
				return 0;
			}
			/* status is set to result of remove called on
			* right child */
			else {
				right -> Remove(elementTNode, right);
			}
		}
		/* if elements TNode is less than data */
		else{
			/* if no left child found */
			if (!left){
				return 0;
			}
			/* status is set to result of remove called on
			 * left child */
			else{
				left->Remove(elementTNode, left);
			}
		}
		/* SHAB called if there's a PointerInParent and balance_track
		 * and status is true */
		if ( !fromSHB )
		{
			SetHeightAndBalance(PointerInParent);
		}
		/* returning the  status */
		return 1;
        }

/*---------------------------------------------------------------------------
Function Name:                SetHeightAndBalance
Purpose:                      This function sets height and balance of tree
Description:                  This function sets height and balance of tree
Input:                        PointerInParent
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

        void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {
                    /* initializing values */ 
		    long left_height = 0;   /* storing left height of tree */
		    long right_height = 0;  /* storing right height of tree */
		    /* checking if left is null */
		    if(left == NULL)
			left_height = -1;
		    else
			left_height = left->height;
		    /* checking if right is null */
		    if(right == NULL)
			right_height = -1;
		    else
			right_height = right->height;
                    /* updating height */
		    this->height = 1+max(left_height, right_height);
		    /* updating balance */
		    this->balance = left_height - right_height;
		    /* rebalancing the tree */
		    if(abs(this->balance) > THRESHOLD){
                        TNode<Whatever> tmp =  TNode<Whatever>(data, *this);
			/* debug message */
			if(Tree<Whatever> :: debug)
				cerr << TREE << tree_count << COMPARE
				<< (const char *) tmp.data << AND
				<< (const char *) tmp.data << "]\n";
			this->Remove(*this, PointerInParent, 1);
			PointerInParent->Insert(tmp.data, PointerInParent);
		    }

        }

        ostream & Write_AllTNodes (ostream & stream) const {
                if (left)
                        left->Write_AllTNodes (stream);
                stream << *this;
                if (right)
                        right->Write_AllTNodes (stream);

                return stream;
        }
};


/*---------------------------------------------------------------------------
Function Name:                Set_Debug_On
Purpose:                      This function sets the debug to on
Description:                  This function sets the debug to on
Input:                        None.
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/

template <class Whatever>
 void  Tree<Whatever> ::  Set_Debug_On (void) {

	/* setting debug on */
	debug = 1;
}

/*---------------------------------------------------------------------------
Function Name:                Set_Debug_Off
Purpose:                      This function sets debug to off
Description:                  This function sets debug to off
Input:                        None.
Output:                       None.
Result:                       Nothing returned.
Side Effects:                 None.
---------------------------------------------------------------------------*/



template <class Whatever>
 void  Tree<Whatever> ::  Set_Debug_Off (void) {

	/* setting debug off */
	debug = 0;
}

/*---------------------------------------------------------------------------
Function Name:                Insert
Purpose:                      This function inserts an element in the tree
Description:                  This function inserts an element in the tree
Input:                        element, PointerInParent
Output:                       None.
Result:                       If element has been inserted.
Side Effects:                 None.
---------------------------------------------------------------------------*/

template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent) {

	/* debug message */
	if ( Tree<Whatever> :: debug)
		cerr << TREE << tree_count << COMPARE
		<< (const char *) element << AND
		<< (const char *) (data) << "]\n";
	/* inserting element in the tree */
	/* duplicate check */
	if (element == data ) {
		data = element;
		return 1;
	}
	else if (element > data ) {
		/* check to go right */
		/* go right */
		if(right == NULL)
		{
			right = new TNode <Whatever>(element, *this);
			if ( Tree<Whatever> :: debug )
				cerr << TREE << tree_count << INSERT
				<< (const char *) element << "]\n";
		}
		else
			/* insert somewhere down to right */
			right -> Insert(element, right); 
	}
	/* go left */
	else {
		if(left == NULL)
		{
			left = new TNode <Whatever>(element, *this);
			if (Tree<Whatever> :: debug)
				cerr << TREE << tree_count << INSERT
				<< (const char *) element << "]\n";
		}
		else
			/* insert somewhere down to left */
			left -> Insert(element, left);
	}
	if( Tree <Whatever> :: debug)
		cerr << TREE << tree_count << UPDATE << (const char *) data
		<< "]\n";
	SetHeightAndBalance(PointerInParent);
	return 1;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

/*---------------------------------------------------------------------------
Function Name:                Insert
Purpose:                      This function inserts root in tree
Description:                  This function inserts root in tree
Input:                        element
Output:                       None.
Result:                       If element has been inserted.
Side Effects:                 None.
---------------------------------------------------------------------------*/

template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {
        long status = 0; /* variable to check status */
	/* check for first item upon insertion */
	if(root == NULL){
		/* insert at root */
		root = new TNode <Whatever> (element, * this);
		status = 1;

		if(debug)
			cerr << TREE << tree_count << INSERT
			<< (const char *) element << "]\n";
	}
	else{
		/* insert somewhere down the tree */
		status = root -> Insert(element,root);
        }
	return status;
}

/*---------------------------------------------------------------------------
Function Name:                Lookup
Purpose:                      This function looks up an element in the tree
Description:                  This function looks up an element in the tree
Input:                        element
Output:                       None.
Result:                       If element was found.
Side Effects:                 None.
---------------------------------------------------------------------------*/

template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {

        
        TNode <Whatever>* working = root;    /* variable to traverse the tree */
	if(root == NULL)
		return NULL;
	/* if the element is not the root, go down the tree */
	   else while(1){
			/* printing debug messages */
			if ( debug == 1){
				cerr<<TREE<<tree_count<<COMPARE<<
				(const char*)(element)<<AND<<(const char*)
				(working->data)<<"]\n";
			}
			/* checking if element is the first element */
			if(element==working->data){
                                element = working->data;
				return 1;
			}
			/* checking if element is on the right */
			else if (element > working->data) {
				/* checking if right element is null */
				if (working->right == NULL)
					return 0;
				else	
					working = working->right;
			}
			/* checking if element is on the left */
			else {
				/* checking if left element is null */
				if ( working->left == NULL)
					return 0;
				else
					working = working->left;
			}
		 }
}


/*---------------------------------------------------------------------------
Function Name:                Remove
Purpose:                      This function removes an element from the tree
Description:                  This function removes an element from the tree
Input:                        element
Output:                       None.
Result:                       If element has been removed.
Side Effects:                 None.
---------------------------------------------------------------------------*/


template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
        /* remove element if root is not null */
	  if( root != NULL )
	{ 
		TNode<Whatever> elementTNode(element, *this);
		/* variable to traverse tree */
	        long status = root -> Remove(elementTNode, root);
		/* variable to store removes result */
		element = elementTNode.data;
		return status;
	}
	return 0;
}
                                
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
% 
% Description :  Guarantee initialization of occupancy and root. It also 
%                initializes the tree_count using a static counter.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL) 

{                
	static long counter; 
	tree_count = ++counter;
	if ( debug)
		cerr << TREE << tree_count << ALLOCATE;
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
        /* deleting all nodes in the tree */
        if(debug)
		cerr << TREE << tree_count << DEALLOCATE;
	root->delete_AllTNodes();
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
