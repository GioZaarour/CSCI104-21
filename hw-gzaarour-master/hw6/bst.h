#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree(); 
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); 
    virtual void remove(const Key& key); 
    void clear(); 
    bool isBalanced() const;
    void print() const;
    bool empty() const;
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator 
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; 
    Node<Key, Value> *getSmallestNode() const;  
    static Node<Key, Value>* predecessor(Node<Key, Value>* current);
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void deleteTree(Node<Key, Value>* nde); //deletes the nodes. used in clear()
    bool returnBalance(Node<Key, Value>* l, Node<Key, Value>* r) const; //returns balance of any node given left and right subtrees. used in isBalanced()
    int getHeight(Node<Key, Value>* nde) const;
    static Node<Key, Value>* successor(Node<Key, Value>* nde); //used in iterator in-order sequencing

protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr) : current_(ptr) {}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() : current_(nullptr) {}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==( const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=( const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->current_ != rhs.current_;

}


/**
* Advances the iterator's location using an in-order sequencing
*/
//in-order sequencing means the next node in our iterator's traversal is the SUCESSOR
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
    current_ = successor(current_);
    return *this;

}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(nullptr) {}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();

}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

//return successor. used for iterator operator++ function, since we are doing in-order sequencing through the BST
//recall successor is the smallest value in the right subtree. 
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::successor(Node<Key, Value>* current) {

    if (current->getRight() != nullptr) {  //if right subtree exists
        current = current->getRight(); //go to it

        while (current->getLeft() != nullptr) { //then go as far left as possible
            current = current->getLeft();
        }
        return current;
    }
    else { 
        while (current->getParent() != nullptr) { //if there's no right subtree, return parent with left child
            if (current->getParent()->getLeft() == current) {
                return current->getParent();
            }
            current = current->getParent();
        }
    }
    return nullptr; //no successor for the input node

}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    Node<Key, Value>* current = root_;

    while(current != nullptr) { //checking if the tree is empty or not

        //if the input node is already in the tree we just update the value
        if (current->getKey() == keyValuePair.first) {
            current->setValue(keyValuePair.second);
            return;
        }
        //if the input node is greater than the current node, we move down the tree and to the right
        else if (current->getKey() < keyValuePair.first){
            //if there's no right child, set input node there and return
            if(current->getRight() == nullptr) {
                current->setRight( new Node<Key, Value>(keyValuePair.first, keyValuePair.second, current) );
                return;
            }

            //otherwise if there's a right child, set current node to that and repeat the loop
            current = current->getRight();
        }
        //if input node is smaller than the current, move down and LEFT
        else if (current->getKey() > keyValuePair.first) {
            //if there's no left child, put the input node there
            if(current->getLeft() == nullptr) {
                current->setLeft( new Node<Key, Value>(keyValuePair.first, keyValuePair.second, current) );
                return;
            }

            //set current as the left child and repeat loop
            current = current->getLeft();
        }
    }

    //if we haven't returned yet, while loop never ran and tree is empty
    //so set the input node as the root
    root_ = new Node<Key, Value>( keyValuePair.first, keyValuePair.second, nullptr );
    return;
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* The tree may not remain balanced after removal.
*/
//swap with PREDECESSOR (largest node  in the left subtree)
//if node only has one child, promote the child
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    //first we have to see if the node we're looking for is even there
    Node<Key, Value>* nde = internalFind(key);

    //function will do nothing if the node wasn't found
    if(nde != nullptr) {
        /*set an integer to check if we need to repeat removal or not(need to repeat when swapping node with 
        predecessor since we will need to then remove it from the leaf location we put it in) */
        int checkRepeat = 0;
        
        do {
            Node<Key, Value>* l = nde->getLeft();
            Node<Key, Value>* r = nde->getRight();
            Node<Key, Value>* p = nde->getParent();

            if (l == nullptr && r != nullptr) {  //node has only right child, so remove the node and promote the child

                //link the lone child to the parent
                r->setParent(p);

                if (p != nullptr) {
                    if (p->getLeft() == nde) {
                        p->setLeft(r);
                    }
                    else {
                        p->setRight(r);
                    }
                }
                else {
                    root_ = r;  //we removed the root so we promote the child up to the root
                }

                checkRepeat = 0;
            }
            else if (l != nullptr && r == nullptr) {  //node has only left child, so remove the node and promote the child

                //link the lone child to the parent
                l->setParent(p);

                if (p != nullptr) {
                    if (p->getLeft() == nde) { 
                        p->setLeft(l);
                    }
                    else {
                        p->setRight(l);
                    }
                }
                else {
                    root_ = l; //we removed the root so we promote the child up to the root
                }

                checkRepeat = 0;                       
            }
            else if (l == nullptr && r == nullptr) { // node is a leaf, so just remove it

                if (p != nullptr) {
                    if (p->getLeft() == nde) {
                        p->setLeft(nullptr);
                    }
                    else {
                        p->setRight(nullptr);
                    }
                } 
                else {
                    root_ = nullptr; //we removed the root and there were no children, so now we have an empty tree
                }
            }
            else if (l != nullptr && r != nullptr) { //node has two children, so swap with predecessor then repeat removal steps
                nodeSwap(predecessor(nde), nde);
                checkRepeat == 1;
            }

        }while(checkRepeat == 1);

        delete nde;
    }
    return;
}


//left then all the way right. return largest value in the left subtree
//if there's no left subtree, return the parent that has a right child
template<class Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{   
    if (current->getLeft() != nullptr) {  //if left subtree exists
        current = current->getLeft(); //go to it

        while (current->getRight() != nullptr) { //then go as far right as possible
            current = current->getRight();
        }
        return current;
    }
    else { 
        while (current->getParent() != nullptr) { //if there's no left subtree, return parent with right child
            if (current->getParent()->getRight() == current) {
                return current->getParent();
            }
            current = current->getParent();
        }
    }
    return nullptr; //no predecessr for the input node
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    deleteTree(root_);
    root_ = nullptr;
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::deleteTree(Node<Key, Value>* nde) {
    if (nde == nullptr) {
        return;
    }
    //recursiveley delete the subtrees of the node, then node after. deletes from the bottom up
    deleteTree(nde->getLeft());
    deleteTree(nde->getRight());
    delete nde;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    if (root_ == nullptr) {
        return nullptr;
    }

    Node<Key, Value>* nde = root_;

    while(nde != nullptr) {

        if(nde->getLeft() == nullptr) {
            return nde;
        }
        else {
            nde = nde->getLeft();
        }
    }
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    //traverse through tree starting at the root
    Node<Key, Value>* nde = root_;
    
    while (nde != nullptr) { 
        
        //if we get to the key, return it. otherwise make a comparison to go left or right
        if (nde->getKey() == key) {
            return nde;
        }
        else if (nde->getKey() < key) {
            nde = nde->getRight();
        }
        else if (nde->getKey() > key) {
            nde = nde->getLeft();
        }
    }
    return nde;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    if(root_ == nullptr) { //if tree is empty it's balanced
        return true;
    }
    else {
        return returnBalance(root_->getLeft(), root_->getRight());
    }
}

//helper function that returns if a node is balanced given its left and right subtrees
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::returnBalance(Node<Key, Value>* l, Node<Key, Value>* r) const {

    //balanced if both subtrees are empty
    if (l == nullptr && r == nullptr) {  
        return true;
    } 
    /*if one subtree is empty we just have to check that the other is height of at most 1
    since empty subtrees have a height of -1, as per properties of AVL trees*/
    else if (l != nullptr && r == nullptr) { 
        if (getHeight(l) <= 1) {
            return true;
        }
    } 
    // " for the reverse
    else if (l == nullptr && r != nullptr) { 
        if (getHeight(r) <= 1) {
            return true;
        }
    } 
    //where it gets fun
    else if (l != nullptr && r != nullptr) {
        //recursively check if both subtrees are balanced, and if they are compare their heights
        if ( returnBalance(l->getLeft(), l->getRight()) && returnBalance( r->getLeft(), r->getRight())) {
            
            //check the variance to make sure the left and right subtrees don't differ in height by more than 1
            if ( (getHeight(l) - getHeight(r)) <= 1 && (getHeight(l) - getHeight(r)) >= -1) {
                return true;
            }
        }
    }
    //if we haven't returned yet, return false
    return false;  
}



template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}


//getheight of a node
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* nde) const {

    if (nde) {  //check if there's a node at that pointer
        

        Node<Key, Value>* l = nde->getLeft();
        Node<Key, Value>* r = nde->getRight();

        //leaf node
        if (l == nullptr && r == nullptr) {  
            return 1;
        }
        //if only one child under input node (right)
        else if (l == nullptr && r != nullptr) { // if start only has right child, height is right child's height +1
            return getHeight(r) + 1;
        }
        //if only one child under input node (left)
        else if (l != nullptr && r == nullptr) {
            return getHeight(l) + 1;
        }
        //two children means that it's height will be the max of each subtree, or the LONGEST distance to a leaf
        else if (l != nullptr && r != nullptr) {
            return std::max(getHeight(r), getHeight(l)) + 1;
        }
    }
    else { //if there isn't a node at that pointer
        return 0;
    }
}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
