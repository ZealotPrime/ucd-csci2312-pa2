//
// Created by zealot on 9/19/15.
//

#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H

#include <iostream>
#include <vector>
#include <math.h>
#include "Clustering.h"

namespace Clustering {
    template <typename T,int dim> class Point;

    template <typename T,int dim> class Point {
        int dim;        // number of dimensions of the point
         std::vector<double> values; // values of the point's dimensions
        unsigned int id;//this point's id number

    public:
        Point();
        Point(int, double *);

        // Big three: cpy ctor, overloaded operator=, dtor
        Point(const Point &);
        Point &operator=(const Point &);
        ~Point();

        // Accessors & mutators
        int getDims() const { return dim; }
        void setValue(int, double);
        double getValue(int) const;

        // Functions
        double distanceTo(const Point &) const;

        // Overloaded operators

        // Members
        Point &operator*=(double);
        Point &operator/=(double);
        const Point operator*(double) const; // prevent (p1*2) = p2;
        const Point operator/(double) const;

        double &operator[](int index) { return values[index]; } // I got rid of the -1 here because I felt it was silly as only a programmer would ever use this

        // Friends
        friend Point &operator+=(Point &, const Point &);
        friend Point &operator-=(Point &, const Point &);
        friend const Point operator+(const Point &, const Point &);
        friend const Point operator-(const Point &, const Point &);

        friend bool operator==(const Point &, const Point &);
        friend bool operator!=(const Point &, const Point &);

        friend bool operator<(const Point &, const Point &);
        friend bool operator>(const Point &, const Point &);
        friend bool operator<=(const Point &, const Point &);
        friend bool operator>=(const Point &, const Point &);

        friend std::ostream &operator<<(std::ostream &, const Point &);
        friend std::istream &operator>>(std::istream &, Point &);

    private:
        static unsigned int newID();
    };
    template <typename T,int dim>
    Clustering::Point::Point()
    {
        this->dim=dim;
        values.resize(dim);
        id=newID();
        for(int x=0;x<dim;x++)
            values[x]=0;
    }
    template <typename T,int dim>
    Clustering::Point::Point(int dimSize, double *inArray)
    {
        dim=dimSize;
        id=newID();
        for(int x=0;x<dim;x++)
            values[x]=inArray[x];
    }
    template <typename T,int dim>
    Clustering::Point::Point(const Clustering::Point &inPoint)
    {
        *this=inPoint;
    }
    template <typename T,int dim>
    Clustering::Point &Clustering::Point::operator=(const Clustering::Point &inPoint)
    {
        //values.resize(dim);
        values=inPoint.values;
        id=inPoint.id;
        return *this;
    }
    template <typename T,int dim>
    Clustering::Point::~Point()
    {
    }
    template <typename T,int dim>
    void Clustering::Point::setValue(int index, double newVal)
    {
        values[index]=newVal;
    }
    template <typename T,int dim>
    double Clustering::Point::getValue(int index) const
    {
        return values[index];
    }
    template <typename T,int dim>
    double Clustering::Point::distanceTo(const Clustering::Point &inPoint) const
    {
        double sum=0, diff=0;
        for(int x=0;x<dim;x++)
        {
            diff=values[x]-inPoint.values[x];
            sum += diff * diff;
        }
        return sqrt(sum);
    }
    template <typename T,int dim>
    Clustering::Point &Clustering::Point::operator*=(double modifier)
    {
        for(int x=0;x<dim;x++)
            values[x]*=modifier;
        return *this;
    }
    template <typename T,int dim>
    Clustering::Point &Clustering::Point::operator/=(double modifier)
    {
        for(int x=0;x<dim;x++)
            values[x]/=modifier;
        return *this;
    }
    template <typename T,int dim>
    const Clustering::Point Clustering::Point::operator*(double modifier) const
    {
        Clustering::Point outPoint(*this);
        outPoint*=modifier;
        return outPoint;
    }
    template <typename T,int dim>
    const Clustering::Point Clustering::Point::operator/(double modifier) const
    {
        Clustering::Point outPoint(*this);
        outPoint/=modifier;
        return outPoint;
    }

    template <typename T,int dim>
    Clustering::Point &operator+=(Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            lhs.values[x] += rhs.values[x];
        return lhs;
    }
    template <typename T,int dim>
    Clustering::Point &operator-=(Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            lhs.values[x] -= rhs.values[x];
        return lhs;
    }
    template <typename T,int dim>
    const Clustering::Point operator+(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        Clustering::Point outPoint(lhs);
        for (int x = 0; x < rhs.dim; x++)
            outPoint.values[x] += rhs.values[x];
        return outPoint;
    }
    template <typename T,int dim>
    const Clustering::Point operator-(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        Clustering::Point outPoint(lhs);
        for (int x = 0; x < rhs.dim; x++)
            outPoint.values[x] -= rhs.values[x];
        return outPoint;
    }

    bool operator==(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            if (lhs.values[x] != rhs.values[x])
                return false;
        return true;
    }

    bool operator!=(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            if (lhs.values[x] != rhs.values[x])
                return true;
        return false;
    }

    bool operator<(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
        {
            if (lhs.values[x] < rhs.values[x])
                return true;
            if (lhs.values[x] > rhs.values[x])
                return false;
        }
        return false;
    }

    bool operator>(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
        {
            if (lhs.values[x] > rhs.values[x])
                return true;
            if (lhs.values[x] < rhs.values[x])
                return false;
        }
        return false;
    }

    bool operator<=(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
        {
            if (lhs.values[x] < rhs.values[x])
                return true;
            if (lhs.values[x] > rhs.values[x])
                return false;
        }
        return true;
    }

    bool operator>=(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
        {
            if (lhs.values[x] > rhs.values[x])
                return true;
            if (lhs.values[x] <rhs.values[x])
                return false;
        }
        return true;
    }

    std::ostream &operator<<(std::ostream &os, const Clustering::Point &inPoint)
    {
        for (int x = 0; x < inPoint.dim; x++)
        {
            os<<std::showpoint;
            os << inPoint.values[x];
            if(x<inPoint.dim-1)
                os<<",";
        }
        return os;
    }

    std::istream &operator>>(std::istream &is, Clustering::Point &inPoint)
    {
        std::string worker;

        for(int x=0;getline(is,worker,',');x++)
        {
            inPoint.values[x] = stod(worker);
        }

        return is;
    }

    template <typename T,int dim>
    unsigned int Point::newID()
    {
        static unsigned int id = 0;
        id++;
        return id;
    }
}
#endif //CLUSTERING_POINT_H