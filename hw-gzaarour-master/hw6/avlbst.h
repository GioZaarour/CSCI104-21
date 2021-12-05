#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the height of a AVLNode.
*/
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

/**
* A setter for the height of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void postInsertionBalance(AVLNode<Key, Value>* child, AVLNode<Key, Value>* parent);
    void postRemovalBalance(AVLNode<Key, Value>* parent);
    void rotation(char direction, AVLNode<Key, Value>* y);

};

//should get O(logn) runtime
//handle duplicates by overwriting old value with new value
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    //very similar to bst insertion but now we have to worry about updating heights and rebalancing

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);

    while (current != nullptr) {
        if (current->getKey() == new_item.first) {
            current->setValue(new_item.second); //update value if the key is already in the tree
            return;
        }
        else if (current->getKey() < new_item.first) {
            if (current->getRight() == nullptr) {
                current->setRight( new AVLNode<Key, Value>(new_item.first, new_item.second, current) );

                /*since we just inserted a leaf node under current, current now has to get set to at least height 
                of 2, if it isn't already (due to the left subtree maybe having greater height) */
                if (current->getHeight() == 1) {  
                    current->setHeight(2);

                    //rebalance
                    postInsertionBalance(current->getRight(), current);  
                }
                //if we didn't update any heights, tree is balanced and we don't need any post-insertion procedures
                return;
            }

            //keep moving down and right, and repeat loop
            current = current->getRight();
        }
        else if (current->getKey() > new_item.first) {
            if (current->getLeft() == nullptr) {
                current->setLeft( new AVLNode<Key, Value>(new_item.first, new_item.second, current) );

                /*since we just inserted a leaf node under current, current now has to get set to at least height 
                of 2, if it isn't already (due to the right subtree maybe having greater height) */
                if (current->getHeight() == 1) {
                    current->setHeight(2);

                    //rebalance
                    postInsertionBalance(current->getLeft(), current);
                } 

                //if you didn't need to update any heights, the tree won't be unbalanced. we dont need to post-insert balance
                return;
            }

            //keep moving down and left, then repeat loop
            current = current->getLeft();
        }
    }
    //otherwise the tree was an empty tree so we just insert the new node as root
    this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    return;
}

//should get O(logn) runtime
//swap nodes with two children with their predecessor
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{

    //first we have to see if the node is even there
    AVLNode<Key, Value>* nde = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (nde != nullptr) {
        if (nde->getKey() == key) {
            break;
        }
        else if (nde->getKey() < key) {
            nde = nde->getRight();
        }
        else if (nde->getKey() > key) {
            nde = nde->getLeft();
        }

    }

    if (nde != nullptr) {
        AVLNode<Key, Value>* p;

        for (int i = 0; i < 2; i++) {
            AVLNode<Key, Value>* l = nde->getLeft();
            AVLNode<Key, Value>* r = nde->getRight();
            p = nde->getParent();

            //node has one child (right)
            if (l == nullptr && r != nullptr) {
                //attach its child to its parent since we're deleting it
                r->setParent(p);
                if (p != nullptr) {
                    if (p->getLeft() == nde) {
                        p->setLeft(r);
                    }
                    else {
                        p->setRight(r);
                    }
                } 
                //otherwise the node was the root and we have to make its child the new root
                else if (p == nullptr) {
                    this->root_ = r;
                }
                break;
            }
            //node has no children
            else if (l == nullptr && r == nullptr) {
                
                if (p != nullptr) {
                    if (p->getLeft() == nde) {
                        p->setLeft(nullptr);
                    }
                    else {
                        p->setRight(nullptr);
                    }
                } 
                //node has no children, AND no parent, so tree is empty
                else if (p == nullptr) {
                    this->root_ = nullptr;
                }
                break;
            } 
            //node has one child (left)
            else if (l != nullptr && r == nullptr) {
                //attach its child to its parent since we're deleting it
                l->setParent(p);
                if (p != nullptr) {
                    if (p->getLeft() == nde) {
                        p->setLeft(l);
                    }
                    else {
                        p->setRight(l);
                    }
                }
                //otherwise the node was the root and we have to make its child the new root
                else if (p == nullptr) {
                    this->root_ = l;
                }
                break;
            }
            //if node has two children we need to swap with predecessor 
            else if (l != nullptr && r != nullptr) {
                nodeSwap(static_cast<AVLNode<Key, Value>*>(this->predecessor(nde)), nde);
            }
        }
        delete nde;

        //now we need to fix heights and rebalance the tree with rotations
        postRemovalBalance(p);
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::postRemovalBalance(AVLNode<Key, Value>* parent) {

    if (parent == nullptr) { // if Node does not have a parent, there is no need to balance
        return;
    }

    //height's of left and right subtrees of parent
    int leftHght, rightHght;
    leftHght = 0; 
    rightHght = 0;

    //if those subtrees exist, get their heights
    if (parent->getRight() != nullptr) { 
        rightHght = parent->getRight()->getHeight();
    }
    if (parent->getLeft() != nullptr) { 
        leftHght = parent->getLeft()->getHeight();
    }

    //check if parent node is out of balance
    if (leftHght - rightHght > 1 || leftHght - rightHght < -1) {

        //child and grandchild will be set (HIGHER/TALLER child and grandchild)
        //**recall from theory/lecture that z is unbalanced node, y is its taller child, and x is y's taller child
        //so parent = z, child = y, and grandkid = x
        //this will make the foundation of our three-node rotation if we need one
        AVLNode<Key, Value>* child;
        AVLNode<Key, Value>* grandkid;

        //left and right subtree heights of child
        int childLeftHght, childRightHght;
        childLeftHght = 0;
        childRightHght = 0;

        /*if left is our taller child, get the heights of ITS subtrees
        then check if zigzags exist and do the necessary rotations */
        if (leftHght > rightHght) { 

            child = parent->getLeft();
            if (child->getLeft() != nullptr) {
                childLeftHght = child->getLeft()->getHeight(); 
            }
            if (child->getRight() != nullptr) { 
                childRightHght = child->getRight()->getHeight();
            }

            //if no zigzag exists, do one right rotation with child = y (center node in the three-node rotation)
            if (childLeftHght >= childRightHght) {

                grandkid = child->getLeft();  
                rotation('r', child);

                //now we need to edit heights accordingly, and we're done
                //right rotation changed parent's (z's) left subtree so recalculate its height as max of its subtrees plus one
                parent->setHeight( std::max( childRightHght, rightHght) + 1 );
                //right rotation changed the child's (y's) right subtree to parent (z)
                child->setHeight( std::max(childLeftHght, parent->getHeight())+ 1 );
                //go up the tree and continue fixing
                postRemovalBalance(child->getParent()); 
                return;
            } 
            /*if we have a zigzag, do a double rotation with grandkid = y (center node). left rotation will un-zigzag it, then a 
            right rotation will fix the unbalanced subtree */
            else { 
                
                //make grandkid and itialize it's heights
                grandkid = child->getRight();
                int gcLeftHght = 0;
                int gcRightHght = 0; 
                if (grandkid->getRight() != nullptr) { 
                    gcRightHght = grandkid->getRight()->getHeight();
                }
                if (grandkid->getLeft() != nullptr) { 
                    gcLeftHght = grandkid->getLeft()->getHeight(); 
                }

                //left rotation on grandchild as middle node to undo the zigzag
                rotation('l', grandkid);
                //right rotation with middle node = grandchild and bottom node being its FORMER parent
                rotation('r', grandkid);

                //fix more heights after rotating, similar to above
                child->setHeight( std::max(childLeftHght, gcLeftHght) + 1); 
                parent->setHeight( std::max(gcRightHght, rightHght) + 1); 
                grandkid->setHeight( std::max(child->getHeight(), parent->getHeight()) + 1);  

                //go up one node in the tree and continue fixing, if needed
                postRemovalBalance(grandkid->getParent()); 
                return;
            }
        } 
        //otherwise right child is the taller child. so do the same operations as above but inversed
        else {  

            child = parent->getRight(); 
            if (child->getLeft() != nullptr) {
                childLeftHght = child->getLeft()->getHeight(); 
            }
            if (child->getRight() != nullptr) {  
                childRightHght = child->getRight()->getHeight(); 
            }

            //no zigzag, single rotation
            if (childRightHght >= childLeftHght) { 
                grandkid = child->getRight();   
                rotation('l', child); 

                //reset heights after rotation
                parent->setHeight( std::max(childLeftHght, leftHght) + 1);  
                child->setHeight(  std::max(childRightHght, parent->getHeight()) + 1);  

                //go up one level and recurse
                postRemovalBalance(child->getParent());
                return;
            } 
            //zigzag exists, so do a double rotation on grandkid
            else {  
                
                grandkid = child->getLeft();  
                int gcLeftHght = 0;  
                int gcRightHght = 0;   
                if (grandkid->getLeft() != nullptr) { 
                    gcLeftHght = grandkid->getLeft()->getHeight();
                }
                if (grandkid->getRight() != nullptr) {   
                    gcRightHght = grandkid->getRight()->getHeight(); 
                }
                
                //double rotation
                rotation('r', grandkid); 
                rotation('l', grandkid);

                //reset heights after rotation
                child->setHeight( std::max(childRightHght, gcRightHght) + 1);
                parent->setHeight( std::max(gcLeftHght, leftHght) + 1);
                grandkid->setHeight( std::max(child->getHeight(), parent->getHeight())  + 1);  

                //go up one level and recurse
                postRemovalBalance(grandkid->getParent()); 
                return;
            }
        }
    }
    //node we passed in wasn't unbalanced
    //if height didn't change after removal, we have nothing to do
    if (parent->getHeight() == std::max(leftHght, rightHght) + 1) {
        return;
    }
    //otherwise the height did change, so we fix it's height and recursively go up the tree doing the same 
    else if ( parent->getHeight() != std::max(leftHght, rightHght) + 1 ){

        parent->setHeight(std::max(leftHght, rightHght) + 1);
        postRemovalBalance(parent->getParent());
        return;

    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::postInsertionBalance(AVLNode<Key, Value>* child, AVLNode<Key, Value>* parent) {

    if (parent == nullptr) { return; } //if our new insertion has no parent, the tree is definitely balanced
    if (parent->getParent() == nullptr) { return; } // same with grandparent

    //grandparent node and it's left and right subtrees
    AVLNode<Key, Value>* grandpa = parent->getParent();  
    AVLNode<Key, Value>* r = grandpa->getRight(); 
    AVLNode<Key, Value>* l = grandpa->getLeft();  

    //initialize heights of gp node and its subtrees
    int gheight = 1;
    int lheight = 0;
    int rheight = 0;

    if (l != nullptr && r != nullptr) {  //if two subtrees exist under gp then set new heights for everything
        lheight = l->getHeight();
        rheight = r->getHeight();
        gheight = std::max(lheight, rheight) + 1;  //height is longest distance to the leaf node, so max of the subtree heights +1
    } 
    else if (l == nullptr && r != nullptr) {  // gp only has one subtree, get it's height and set gp height
        rheight = r->getHeight();
        gheight = rheight + 1;
    } 
    else if (l != nullptr && r == nullptr) {  // "
        lheight = l->getHeight();
        gheight = lheight + 1;
    }
    if (grandpa->getHeight() == gheight) {
        //if after updating heights, the gp height never changed from insertion, then tree is balanced
        return;
    }
    //actually update the gp height
    grandpa->setHeight(gheight);  

    if (lheight - rheight <= 1 && lheight - rheight >= -1) {  //check balance of gp node. by checking the variance in its subtrees

        //gp height has changed so we have to move up the tree and recursively check the higher nodes
        postInsertionBalance(parent, grandpa); 
        return;
    }
    else if (l == parent) { //left subtree is the unbalanced
        if (child == parent->getLeft()) { //no zigzag

            //right rotation with parent node as "y" (middle node)
            rotation('r', parent); 
            int hleft = 0; //height of gp's left subtree

            if (grandpa->getLeft() != nullptr) {
                hleft = grandpa->getLeft()->getHeight();
            } 

            //set grandpa height again..
            grandpa->setHeight( std::max(hleft, rheight)+ 1); 

            //and since the right subtree of parent is now gp, parent height = max of child/gp height
            parent->setHeight( std::max(child->getHeight(), grandpa->getHeight()) + 1);
        } 
        else {  //now we have a left zigzag, so we do a double rotation
            
            //left rotation with child node as "y" (middle node)
            rotation('l', child); 
            //right rotation with child node as "y" again, but this time it's old parent is now below it as "x"           
            rotation('r', child);

            //get heights again (see above)
            int hleft = 0;
            if (grandpa->getLeft() != nullptr) {
                hleft = grandpa->getLeft()->getHeight();
            }
            grandpa->setHeight( std::max(hleft, rheight) + 1);

            //now we need heights of the parents' subtrees, so we initialize those and then set their values
            int pleft, pright;
            pright = 0;
            pleft = 0;
            if (parent->getRight() != nullptr) { 
                pright = parent->getRight()->getHeight(); 
            }
            if (parent->getLeft() != nullptr) { 
                pleft = parent->getLeft()->getHeight();
            }  
            
            //since its subtree heights changed, we need to set parent's height according to those
            parent->setHeight(std::max(pleft, pright) + 1); 
            //after rotating, the child's subtrees are now parent and grandparent, so we set child's height according to those
            child->setHeight( std::max(parent->getHeight(), grandpa->getHeight()) + 1 ); 
        }
    } 
    else {  //right subtree is the imbalanced one. so below we just repeat the same 
            //operations as with left unbalanced subtree, but reversed
        if (child == parent->getRight()) {  //no zigzag
            rotation('l', parent);

            int hright = 0; 
            if (grandpa->getRight() != nullptr) {  
                hright = grandpa->getRight()->getHeight();
            }  
            grandpa->setHeight( std::max(hright, lheight) + 1);
            parent->setHeight( std::max(child->getHeight(), grandpa->getHeight()) + 1);
        } 
        else {  // if right zig-zag
            rotation('r', child);
            rotation('l', child);

            int hright = 0; 
            if (grandpa->getRight() != nullptr) {
                hright = grandpa->getRight()->getHeight();
            } 

            grandpa->setHeight( std::max(hright, lheight) + 1);

            int pleft, pright;
            pright = 0;
            pleft = 0;  
            if (parent->getRight() != nullptr) {  
                pright = parent->getRight()->getHeight();
            }
            if (parent->getLeft() != nullptr) {
                pleft = parent->getLeft()->getHeight();
            }  

            parent->setHeight(std::max(pleft, pright) + 1); 
            child->setHeight( std::max(parent->getHeight(), grandpa->getHeight()) + 1); 
        }
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotation( char direction, AVLNode<Key, Value>* y ) {  
    //in rotation -- z: top node (unbalanced node). y: middle node. x: bottom node

    AVLNode<Key, Value>* z = y->getParent(); 

    //left rotation
    if (direction == 'l') { 

        // update z's right subtree pointer,
        z->setRight(y->getLeft());

        //update the parent of the left child we detached after rotating
        if (z->getRight() != nullptr) {
            z->getRight()->setParent(z);
        }

        //update y's parent pointer,
        y->setParent(z->getParent());

        //set z's parent as y
        z->setParent(y);

        //y's left child set to z
        y->setLeft(z);
    
    //right rotation
    } else if (direction == 'r') { 
        
        //same operations as above but inverted
        z->setLeft(y->getRight());
        if (z->getLeft() != nullptr) { z->getLeft()->setParent(z); }
        y->setParent(z->getParent());
        z->setParent(y);
        y->setRight(z);
    }

    // if z had a parent, set that parent's child to point to the new parent y
    if (y->getParent() != nullptr) {  
        if (y->getParent()->getLeft() == z) { 
            y->getParent()->setLeft(y); 
        }
        else { 
            y->getParent()->setRight(y); 
        }
    }
    else {
        //if z was the root, set y as the new root
        this->root_ = y;
    }
    return;
}


#endif
