#include "bst.h"
#include "avlbst.h"
#include "print_bst.h"

int main (){
    AVLTree<int, std::string> avl;

    std::pair<int, std::string> node1;
    node1.first = 100;
    node1.second = "damn";

    avl.insert(node1);

    std::pair<int, std::string> node2;
    node2.first = 65;
    node2.second = "okay";

    avl.insert(node2);

    std::pair<int, std::string> node3;
    node3.first = 103;
    node3.second = "bet";

    avl.insert(node3);

    avl.print();

    std::cout << "Is it balanced?: " << std::boolalpha << avl.isBalanced() << std::endl;

    //second print

    std::pair<int, std::string> insertNode;

    avl.insert( insertNode = std::make_pair(50, "shit") );

    avl.print();

    //should always be balanced because AVL tree
    std::cout << "Is it balanced?: " << std::boolalpha << avl.isBalanced() << std::endl;

    avl.insert( insertNode = std::make_pair(700, "shit") );
    avl.insert( insertNode = std::make_pair(45, "shit") );
    avl.insert( insertNode = std::make_pair(37, "shit") );
    avl.insert( insertNode = std::make_pair(103, "FUCK") );
    avl.insert( insertNode = std::make_pair(52, "shit") );
    avl.insert( insertNode = std::make_pair(22, "shit") );
    avl.remove(37);
    avl.remove(700);
    avl.print();

    std::cout << "Is it balanced?: " << std::boolalpha << avl.isBalanced() << std::endl;

}