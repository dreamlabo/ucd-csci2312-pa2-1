//
// Created by Owner on 2/15/2016.
//

#include <cmath>
#include "Point.h"
#include <fstream>
#include <sstream>
#include <string>
using std::string;
using std::stringstream;

#include <cstdlib>

namespace Clustering {


unsigned int Point::__idGen = 0; // id generator

//Constructor #1
//Precondition: User has entered amount of dimensions for their points
//PostCondition: *__values (array) is initialized to 0.0
   Point::Point(int dimensions){
        __dim = dimensions;
        __values = new double[__dim];
        __id = __idGen++;

        //Initialize m_Value array to 0.0
        for (int index = 0; index < __dim; index++) {
            __values[index] = 0.0;
        }
    }

//copy constructor
  Point::Point(const Point &rightSide) {

        __dim = rightSide.__dim;
        __values = new double[__dim];

        for (int index = 0; index < __dim; index++) {
            __values[index] = rightSide.__values[index];
        }
        __id = rightSide.__id;
    }


//Assignment Operator
    Point &Point::operator=(const Point &rightSide) {
        if (*this == rightSide)
        { return *this; }
        else
        {
            delete[] __values;

            __dim = rightSide.__dim;
            __values = new double[__dim];
            for (int index = 0; index < __dim; index++)
            {
                __values[index] = rightSide.__values[index];
            }
           this-> __id = rightSide.__id;
            return *this;
        }
    }
// Destuctor
    Point::~Point() {

        delete [] __values;

    }



//accesors

// setValue function
// sets the value of a point
// Precondition:
// Postcondition: the values (point) of an index is set
    void Point::setValue(int index, double axisPoint) {
        __values[index] = axisPoint;
    }



// getValue function
// returns the point value
// Precondition:
// Postcondition: the values of the point are returned
    double Point::getValue(int index) const {
                return __values[index];

    }


// getId function
// returns the ID of the point
// Precondition:
// Postcondition: the ID of the point is returned
    int Point::getId() const {
        return __id;
    }

// getDims function
// returns the dimensions of the point
// Precondition:
// Postcondition: the dimensions of the point are returned
    int Point::getDims() const {
        return __dim;;
    }


// Functions

// distanceTo function
    double Point::distanceTo(const Point &array) const {
        if (__dim != array.__dim)
            return 0;
        else {
            double sum = 0;
            double distance = 0;

            for (int index = 0; index < __dim; index++) {
                distance = (pow(__values[index] - array.__values[index], 2));
                sum += distance;
            }
            return sqrt(sum);
        }
    }

// Overloaded operators
//Members

//overloaded *=
    Point &Point::operator*=(double d) {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] * d;
        }
        return *this;
    }


// Overloaded /=
    Point &Point::operator/=(double d) {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] / d;
        }
        return *this;
    }




// overloaded operator *
    const Point Point::operator*(double d) const {

//        if( d == 0) {
//            std::cout << "Sorry, division by 0 is not allowed\n";
//            return *this;
//        }
//        Point temp(this->__dim);
//        int dim = this->__dim;
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] * d;
        }
        return *this;
    }

//Overloaded operator /
    const Point Point::operator/(double d) const {
        for (int index = 0; index < this->__dim; index++){
            this->__values[index] = this->__values[index] / d;
        }
        return *this;
    }

// Overloaded operator []
    double &Point::operator[](int index) {
         return __values[index];
    }

// friend functions

//Overloaded +=
    Point &operator+=(Point &leftSide, const Point &rightSide) {
        int dim = leftSide.__dim;
        for (int index = 0; index < dim; index++) {
            leftSide.__values[index] += rightSide.__values[index];
        }
        return leftSide;
    }

// Overloaded -=
    Point &operator-=(Point &leftSide, const Point &rightSide) {
        int dim = leftSide.__dim;
        for (int index = 0; index < dim; index++) {
            leftSide.__values[index] -= rightSide.__values[index];
        }
        return leftSide;
    }

// Overloaded +
    const Point operator+(const Point &leftSide, const Point &rightSide) {

        Point temp(leftSide.__dim);

        for (int index = 0; index < temp.__dim; index++) {
            temp.__values[index] = leftSide.__values[index] + rightSide.__values[index];
        }
        return temp;
    }

// Overloaded +
    const Point operator-(const Point &leftSide, const Point &rightSide) {
        Point temp(leftSide.__dim);

        for (int index = 0; index < temp.__dim; index++) {
            temp.__values[index] = leftSide.__values[index] - rightSide.__values[index];
        }
        return temp;
    }


// Overloaded ==
    bool operator==(const Point &leftSide, const Point &rightSide) {
         // check that the points are not the same points or their dimensions are different
        if (&leftSide == &rightSide || leftSide.__dim != rightSide.__dim) {
        return false;
    }

        bool equal = true;
        for (int index = 0; index < rightSide.__dim; index++) {
            if (leftSide.__values[index] != rightSide.__values[index]) {
                equal = false;
                break;
            }
        }
        return equal && (leftSide.__id == rightSide.__id);
    }

// Overloaded !=
    bool operator!=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide == rightSide);
    }



// Overloaded <
    bool operator<(const Point &leftSide, const Point &rightSide) {
         //check that the points are not the same points or their dimensions are different
        if (&leftSide == &rightSide || leftSide.__dim != rightSide.__dim) {
            return false;
        }

        bool less_Than = false;
        for (int index = 0; index < rightSide.__dim; index++) {
             if (leftSide.__values[index] > rightSide.__values[index]) {
                break;
            }
            else if (leftSide.__values[index] < rightSide.__values[index]) {
                less_Than = true;
                break;
            }
        }
        return less_Than;
    }

// Overloaded >
    bool operator>(const Point &leftSide, const Point &rightSide) {
        // check that the points are not the same points or their dimensions are different
        return rightSide < leftSide;
    }

// Overloaded <=
    bool operator<=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide > rightSide);
    }

// Overloaded >=
    bool operator>=(const Point &leftSide, const Point &rightSide) {
        return !(leftSide < rightSide);
    }

// Overloaded <<
    std::ostream &operator<<(std::ostream &ostream, const Point &point) {
            ostream <<  point.__values[0];
            for (int  index = 1; index < point.__dim ; index++) {
                ostream << ", " << point.__values[index];
            }
            return ostream;
        }


    std::istream &operator>>(std::istream &istream, Point &point) {
        double* newPoint = new double[point.__dim];
        char pointValue[50];

        if (istream.good()){
        for(int index = 0; index < point.__dim; index++)
        {istream.getline(pointValue, 50, ',');
            newPoint[index] = atof( pointValue);
        }
        }
        else { std::cerr << "The file not open" << std::endl;}
         point.__values = newPoint;
        return istream;
    }


} //end of Clustering namespace bracket


