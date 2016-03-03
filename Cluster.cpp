//
// Created by Todd Labo on 2/15/2016.
//



#include <cassert>
#include <string>
#include "Point.h"
#include "Cluster.h"
#include <cstdlib>    //for "atof" in operator >>
#include <fstream>
#include <sstream>
using std::stringstream;
using std::string;
namespace Clustering {

    LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n) {
    }


// Constructor
    Cluster::Cluster() : __size(0), __points(nullptr) { }

//Copy Constructor
    Cluster::Cluster(const Cluster &rightSide) {
        __size = 0;
        __points = nullptr;
        if (rightSide.__points == nullptr) {
            assert(rightSide.__size == 0);
            __points = nullptr;
        }
        else {
            LNodePtr current = rightSide.__points;

            while (current != nullptr) {
                add(current->point);
                current = current->next;
            }
        }
    }


// Assignment operator=
    Cluster &Cluster::operator=(const Cluster &rightSide) {
        if (this == &rightSide) {
            return *this;
        }//prevent self assignment

        LNodePtr nodePtr = __points; //to traverse the list
        LNodePtr nextNode; //to point to next node

        while (nodePtr != nullptr) {
            nextNode = nodePtr->next; //save pointer to next node
            delete nodePtr;
            nodePtr = nextNode; //position ptr at the next node
            __size--;
        }
        __size = 0;
        __points = nullptr;

        LNodePtr rightSidePtr = rightSide.__points;
        for (int i = 0; i < rightSide.getSize(); i++) {
            add(rightSidePtr->point);

            rightSidePtr = rightSidePtr->next;

        }
        return *this;
    }

    Cluster::~Cluster() {
        LNodePtr currentNode = __points;
        LNodePtr nextNode;

        while (currentNode != nullptr) {
            nextNode = currentNode->next;
            delete currentNode;
            currentNode = nextNode;
        }
        __size = 0;
        __points = nullptr;

    }

// getSize function
// returns the size of the cluster
// Precondition:
// Postcondition: the size of the cluster is returned
    int Cluster::getSize() const {
        return __size;
    }

// add function
// adds a point to linked list in lexographical order
// Precondition:
// Postcondition: a point is added to the linked list, retaining the lexigraphical order
    void Cluster::add(const Point &addPoint) {
        LNodePtr newNode = new LNode(addPoint, nullptr);

        // If there are no nodes in the list
        // Make newNode the first node.
        if (!__points) {  // insert into empty list
            __points = newNode;
        }
        else if (addPoint < __points->point) { //insert at beginning of the list
            newNode->next = __points;
            __points = newNode;
        }
        else {
            LNodePtr currentNode = __points->next, previousNode = __points; //set pointer to current and previous nodes

            while (true) {
                if (currentNode == nullptr || currentNode->point > addPoint) {
                    newNode->next = currentNode;
                    previousNode->next = newNode;
                    break;
                }
                else
                    previousNode = currentNode;
                currentNode = currentNode->next;
            }
        }
        __size++;
    }


// remove function
// removes a point from the cluster
// Precondition:
// Postcondition: a point is removed from the cluster
    const Point &Cluster::remove(const Point &removePoint) {

        LNodePtr nodePtr; //To traverse the list
        LNodePtr previousNode; //To point to the previous node

        //If list is empty
        if (!__points)
            return removePoint;

        //If point to be removed is at head of list
        if (__points->point == removePoint) {
            nodePtr = __points->next;
            delete __points;
            __points = nodePtr;
        }
        else {
            nodePtr = __points;
            while (nodePtr != nullptr && nodePtr->point != removePoint) {
                previousNode = nodePtr;
                nodePtr = nodePtr->next;
            }
            if (nodePtr) {
                previousNode->next = nodePtr->next;
                delete nodePtr;
            }
        }

        __size--;
        return removePoint;
    }

// contains Function
// returns true/false whether a point is found within a cluster
// Precondition:
// Postcondition: a bool value (true/false) is returned
    bool Cluster::contains(const Point &point) {
        LNodePtr curser = __points; // to traverse the list
        bool contains = false;

        // Traverse the list, checking to see if the list contains the requested point
        while (curser != nullptr) {
            if (curser->point == point) {
                contains = true;
                break;
            }
            curser = curser->next; // move curser
        }
        return contains;
    }




// Overloaded operators

// Overloaded [] operator
// returns a reference to the point at the requested index
// Precondition
// Postcondition: a reference to the point at the requested index is returned
    const Point &Cluster::operator[](unsigned int index) const {
        assert (index < __size);
        LNodePtr cursor = __points;

        for (unsigned int i = 0; i < index; i++) {
            assert(cursor);
            cursor = cursor->next;
        }
        return cursor->point;
    }

// Members: Compound assignment (Point argument)

// operator +=  (Point argument)
// Adds a point to an existing cluster.
// Precondition:
// Postcondition: A new point is added to an existing Cluster.
    Cluster &Cluster::operator+=(const Point &point) {
        add(point);
        return *this;
    }

// operator -=  (Point argument)
// Removes a point from existing cluster.
// Precondition:
// Postcondition: A new point is removed from an existing Cluster.
    Cluster &Cluster::operator-=(const Point &point) {
        remove(point);
        return *this;
    }


    // Members: Compound assignment (Cluster argument)
// operator +=   (Cluster)
// creates a union of two cluster
    Cluster &Cluster::operator+=(const Cluster &cluster) {
        Cluster tempCluster;  // create new cluster to store the union of two cluster

        while (__points != nullptr) {
            tempCluster.add(__points->point);
            __points = __points->next;
        }
        LNodePtr cursor = cluster.__points; // to traverse the list
        while (cursor != nullptr) {
            if (!tempCluster.contains(cursor->point)) {
                tempCluster.add(cursor->point);
            }
            cursor = cursor->next;
        }
        *this = tempCluster;
        return *this;
    }

// operator -=   (Cluster)
// creates a new cluster created from the (asymmetric) difference of two cluster
// Precondition:
// Postcondition: a new cluster created and returned from the (asymmetric) difference of two cluster.
    Cluster &Cluster::operator-=(const Cluster &cluster) {
        // Cluster tempCluster;
        LNodePtr cursor = cluster.__points;

        while (cursor != nullptr) {
            if (contains(cursor->point)) {
                remove(cursor->point);
            }
            cursor = cursor->next;
        }

        return *this;
    }







//friends IO


// Overloaded << operator
    std::ostream &operator<<(std::ostream &ostream, const Cluster &cluster) {
        assert(cluster.__points != nullptr);
        LNodePtr cursor = cluster.__points;

        while (cursor != nullptr) {    // print the rest of the points
            ostream << cursor->point << std::endl;
            ostream << std::endl;
            cursor = cursor->next;
        }
        return ostream;
    }


// Overloaded >> operator
// Reads cluster from a file
// Precondition:
// Postcondition: A cluster is read from a file, and a new cluster is created form that file.
    std::istream &operator>>(std::istream &istream, Cluster &cluster) {
        return istream;

    }



// Friends: Comparison


// Overloaded == operator
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        //Check to see if clusters are of same size, if not return false
        //assert (lhs.__size == rhs.__size);
        // check that the points are not the same points or their dimensions are different
        if (lhs.__size != rhs.__size) {
             return false;
             }

        LNodePtr targetRight = rhs.__points; //set rhs curser to head
        LNodePtr targetLeft = lhs.__points;  //set lhs curser head

            //  while the end of the list hasn't been reached
            while ( targetLeft != nullptr  ) {
                // check to see if nodes don't match, if they don't, return false
                if (targetLeft->point != targetRight->point) {
                    return false;
                }
                else  // otherwise move to the  next node
                    targetRight = targetRight->next;
                    targetLeft = targetLeft->next;
            } //return to beg of while loop

            //if we reach here, all points are the same and we return true
            return true;
        }


// Overloaded != operator
    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        //Check to see if clusters are of same size, if not return false
        //assert (lhs.__size == rhs.__size);
        // check that the points are not the same points or their dimensions are different
//        if (lhs.__size != rhs.__size) {
//            return false;
//        }

        LNodePtr targetRight = rhs.__points; //set rhs curser to head
        LNodePtr targetLeft = lhs.__points;  //set lhs curser head

        //  while the end of the list hasn't been reached
        while ( targetLeft != nullptr && targetRight!= nullptr  ) {
            // check to see if nodes don't match, if they don't, return false
            if (targetLeft->point == targetRight->point) {
                return false;
            }
            else  // otherwise move to the  next node
                targetRight = targetRight->next;
                targetLeft = targetLeft->next;
        } //return to beg of while loop

        //if we reach here, all points are the same and we return true
        return true;
    }


// Friends: Arithmetic (Cluster and Point)

// operator +   (Cluster,Point)
// A new cluster is created by adding a point to an existing cluster. (using copy constuctor and add function of cluster.cpp)
// Precondition:
// Postcondition: a new cluster has been created by adding a point from to an existing cluster.
    const Cluster operator+(const Cluster &cluster, const Point &pointToAdd) {
        Cluster tempCluster(cluster);
          tempCluster += pointToAdd;

        return tempCluster;
    }

// operator -   (Cluster,Point)
// A new cluster is created by removing a point from an existing cluster.  (using copy constuctor and remove function of cluster.cpp)
// Precondition:
// Postcondition: a new cluster has been created by removing a point from an existing cluster.
    const Cluster operator-(const Cluster &cluster, const Point &pointToSubtract) {
        Cluster tempCluster(cluster);
        tempCluster -= pointToSubtract;

        return tempCluster;
    }


    // Friends: Arithmetic (two Clusters)

// operator + (cluster,cluster)
// union of two clusters.
// Precondition:
// Postcondition: The union of two clusters is returned
    const Cluster operator+(const Cluster &leftCluster, const Cluster &rightCluster) {   //FFFIIIXXXXXXXXX!!!!!!!!!!!!!!!!!!!!!!!!!
        Cluster plusCluster(leftCluster);


       // LNodePtr  cursorLeft = leftCluster.__points;
        LNodePtr cursorRight = rightCluster.__points;
        while (cursorRight != nullptr){
            if(!plusCluster.contains(cursorRight->point)){
                plusCluster.add(cursorRight->point);
            }
            cursorRight = cursorRight->next;
        }
        return plusCluster;
    }

// operator - (cluster,cluster)
// (asymmetric) difference of two clusters.
// Precondition:
// Postcondition: The (asymmetric) difference of two clusters is returned
    const Cluster operator-(const Cluster &leftCluster, const Cluster &rightCluster) {
       // Cluster newCluster;
        Cluster tempCluster(leftCluster);

        LNodePtr cursorRight = rightCluster.__points;
        while (cursorRight != nullptr){
            if(tempCluster.contains(cursorRight->point)){
                tempCluster.remove(cursorRight->point);
                cursorRight = cursorRight->next;
            } else{
            cursorRight = cursorRight->next;
            }
        }
        return tempCluster;
    }


// operator >>
//        std::ifstream csv("points.csv");
//        std::string line;
//
//        if (csv.is_open()) {
//
//            while (getline(csv,line)) {
//
//                std::cout << "Line: " << line << std::endl;
//
//                std::stringstream lineStream(line);
//                std::string value;
//                double d;
//                Point p(5);
//
//                int i = 1;
//                while (std::getline(lineStream, value, ',')) {
//                  //  d = std::stod(value);
//
//                    std::cout << "Value: " << d << std::endl;
//
//                    p.setValue(i++, d);
//                }
//                std::cout << "Point: " << p << std::endl;
//            }
//        }
//        csv.close();

//        return istream;
//    }










}// end of namespace Clustering



