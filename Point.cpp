//
// Created by zealot on 9/19/15.
//

#include "Point.h"
#include <cmath> //needed for distanceTo func
#include "Clustering.h"
#include <vector>
#include <iostream>
#include <string>

Clustering::Point::Point(int dimSize)
{
    dim=dimSize;
    values.resize(dim);
    id=newID();
    for(int x=0;x<dim;x++)
        values[x]=0;
}

Clustering::Point::Point(int dimSize, double *inArray)
{
    dim=dimSize;
    id=newID();
    for(int x=0;x<dim;x++)
        values[x]=inArray[x];
}

Clustering::Point::Point(const Clustering::Point &inPoint)
{
    *this=inPoint;
}

Clustering::Point &Clustering::Point::operator=(const Clustering::Point &inPoint)
{
    dim=inPoint.dim;
    //values.resize(dim);
    values=inPoint.values;
    id=inPoint.id;
    return *this;
}

Clustering::Point::~Point()
{
}

void Clustering::Point::setValue(int index, double newVal)
{
    values[index]=newVal;
}

double Clustering::Point::getValue(int index) const
{
    return values[index];
}

double Clustering::Point::distanceTo(const Clustering::Point &inPoint) const
{
    double sum=0, diff=0;
    for(int x=0;x<dim;x++)
    {
        diff=fabs(values[x]-inPoint.values[x]);
        sum += diff * diff;
    }
    return sqrt(sum);
}

Clustering::Point &Clustering::Point::operator*=(double modifier)
{
    for(int x=0;x<dim;x++)
        values[x]*=modifier;
    return *this;
}

Clustering::Point &Clustering::Point::operator/=(double modifier)
{
    for(int x=0;x<dim;x++)
        values[x]/=modifier;
    return *this;
}

const Clustering::Point Clustering::Point::operator*(double modifier) const
{
    Clustering::Point outPoint(*this);
    outPoint*=modifier;
    return outPoint;
}

const Clustering::Point Clustering::Point::operator/(double modifier) const
{
    Clustering::Point outPoint(*this);
    outPoint/=modifier;
    return outPoint;
}

namespace Clustering { // I discovered the need to add this after I had written all the functions, thus the needless scope operators
    Clustering::Point &operator+=(Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            lhs.values[x] += rhs.values[x];
        return lhs;
    }

    Clustering::Point &operator-=(Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        for (int x = 0; x < rhs.dim; x++)
            lhs.values[x] -= rhs.values[x];
        return lhs;
    }

    const Clustering::Point operator+(const Clustering::Point &lhs, const Clustering::Point &rhs)
    {
        Clustering::Point outPoint(lhs);
        for (int x = 0; x < rhs.dim; x++)
            outPoint.values[x] += rhs.values[x];
        return outPoint;
    }

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
                os<<Clustering::DELIM;
        }
        return os;
    }

    std::istream &operator>>(std::istream &is, Clustering::Point &inPoint)
    {
        std::string worker;

        for(int x=0;getline(is,worker,Clustering::DELIM);x++)
        {
            inPoint.values[x] = stod(worker);
        }

        return is;
    }

    unsigned int Point::newID()
    {
        static unsigned int id = 0;
        id++;
        return id;
    }
}