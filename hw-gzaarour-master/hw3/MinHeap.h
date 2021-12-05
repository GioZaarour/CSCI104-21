// d-ary teemplated min heap implementation with nodes of the tree containing
// some object and a priority value

// entire implementation of the minHeap will be in .h file because it's
// templated
#include <exception>
#include <stdexcept>
#include <vector>

// define a struct that contains both the item and priority
// will be a heap of these structs
template<class T>
struct object {
    T item;
    int priority;
};

template<class T>
class MinHeap {

private:
    // storing the heap
    std::vector<object> list;

    int d;

    // shift a node up to its correct position
    void heapifyUp(int i) {

        // index of the parent of our node
        int parent = (i - 1) / d;

        // Loop should only run till root node in case the
        // element inserted is the minimum  will
        // send it to the root node
        while (parent >= 0) {
            if (list[i].priority < list[parent].priority) {
                swap(list[i], list[parent]);
                i = parent;
                parent = (i - 1) / d;
            }

            // node is in the correct position
            else {
                break;
            }
        }
    }

    // shift a node down to its correct position
    void heapifyDown(int i) {

        // array to store the indexes of all the children nodes
        int child[d];

        while (1) {
            // child[j]=-1 if node is a leaf
            // checks if the potential child node is out of bounds of the vector,
            // if it is sets child node's index to -1
            // and if it isn't sets child node's index to (d*index + 1, d*index+2,
            // etc.)
            for (int j = 0; j < d; j++) {
                child[j] = ((d * i + j + 1) < list.size()) ? (d * i + j + 1) : -1;
            }

            // initialize min child and its index
            // min child starts as the value of the parent node because
            // to start off with we don't know any other value
            int min_child = list[i].priority, min_child_index;

            // loop to find the smallest of all
            // the children of a given node
            for (int j = 0; j < d; j++) {
                if (child[j] != -1 && list[child[j]].priority < min_child) {
                    min_child_index = child[j];
                    min_child = list[child[j]].priority;
                }
            }

            /* if the min child is still equal to what we initialized it as, that
            means
            either no child node is lower than the parent, or we are at a leaf node
            with no children */
            if (min_child == list[i].priority)
                break;

            // swap only if the key of min_child_index
            // is less than the key of node
            if (list[i].priority < list[min_child_index].priority)
                swap(list[i], list[min_child_index]);

            i = min_child_index;
        }
    }

public:
    /* Constructor that builds a d-ary Min Heap
        This should work for any d >= 2,
        but doesn't have to do anything for smaller d.*/
    MinHeap(int d) {
        try {
            if (d < 2) {
                throw invalid_argument("Must use value larger than 2");
            }

            this->d = d;
        } catch (const invalid_argument& ex) {
            std::cout << std::endl << ex.what();
        }
    }

    ~MinHeap();

    /* adds the item to the heap, with the given priority. */
    void add(T item, int priority) {
        // create the new object
        object addition;
        object.item = item;
        object.priority = priority;

        // put it at the back of the vector or the bottom of the heap
        list.push_back(addition);

        // get the index of the newly added node and put it in the right place with
        // heapifyUp
        int index = list.size() - 1;
        heapifyUp(index);
    }

    /* returns the element with smallest priority.
        smallest priority (lowest in value) stored at the ROOT
                    Break ties however you wish.
                    Throws an exception if the heap is empty. */
    const T& peek() const {
        try {
            // exception if heap empty
            if (isEmpty()) {
                throw out_of_range("Heap empty");
            }

            return list[0].item;
        } catch (const out_of_range& oor) {
            std::cout << std::endl << oor.what();
        }
    }

    /* removes the element with smallest priority.
                    Break ties however you wish.
        Throws an exception if the heap is empty. */
    void remove() {
        try {
            // exception if heap empty
            if (isEmpty()) {
                throw out_of_range("Heap empty");
            }

            // replace root node with the bottom and leftmost node
            list[0] = list.back();
            list.pop_back();

            // trickle/bubble down the new root node to position all the nodes
            // properly
            heapifyDown(0);

        } catch (const out_of_range& oor) {
            std::cout << std::endl << oor.what();
        }
    }

    /* returns true iff there are no elements on the heap. */
    bool isEmpty() {
        if (list.size() == 0)
            return true;
        else
            return false;
    }
};