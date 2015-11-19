//
// Created by zealot on 9/19/15.
//

#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H

#include <iostream>
#include <vector>
#include <math.h>

namespace Clustering {
    //forward declarations of friend functions
    template <typename T,int dim> class Point;

//    template <typename T, int dim> struct mapKey;
//    template <typename T, int dim> struct keyHash;

    template <typename T,int dim>Point<T,dim> &operator+=(Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>Point<T,dim> &operator+=(Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>Point<T,dim> &operator-=(Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>const Point<T,dim> operator+(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>const Point<T,dim> operator-(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator==(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator!=(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator<(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator>(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator<=(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>bool operator>=(const Point<T,dim> &, const Point<T,dim> &);
    template <typename T,int dim>std::ostream &operator<<(std::ostream &, const Point<T,dim> &);
    template <typename T,int dim>std::istream &operator>>(std::istream &, Clustering::Point<T,dim> &);

    template <typename T,int dim> class Point {
         std::vector<T> values; // values of the point's dimensions
        unsigned int id;//this point's id number

    public:
        Point();
        Point(T *);

        // Big three: cpy ctor, overloaded operator=, dtor
        Point(const Point<T,dim> &);
        Point &operator=(const Point<T,dim> &);
        ~Point();

        // Accessors & mutators
        int getDims() const { return dim; }
        void setValue(int, T);
        T getValue(int) const;
        unsigned int getID(){return id;}
        void setID(){id=newID();} //changes the ID to allow a single temp point to be used for loading in the file


        // Functions
        double distanceTo(const Point<T,dim> &) const;

        // Overloaded operators

        // Members
        Point<T,dim> &operator*=(double);
        Point<T,dim> &operator/=(double);
        const Point<T,dim> operator*(double) const; // prevent (p1*2) = p2;
        const Point<T,dim> operator/(double) const;

        double &operator[](int index) { return values[index]; } //0 based array operator

        // Friends
        friend Point<T,dim> &operator+= <>(Point<T,dim> &, const Point<T,dim> &);
        friend Point<T,dim> &operator-= <>(Point<T,dim> &, const Point<T,dim> &);
        friend const Point<T,dim> operator+ <>(const Point<T,dim> &, const Point<T,dim> &);
        friend const Point<T,dim> operator- <>(const Point<T,dim> &, const Point<T,dim> &);

        friend bool operator== <>(const Point<T,dim> &, const Point<T,dim> &);
        friend bool operator!= <>(const Point<T,dim> &, const Point<T,dim> &);

        friend bool operator< <>(const Point<T,dim> &, const Point<T,dim> &);
        friend bool operator> <>(const Point<T,dim> &, const Point<T,dim> &);
        friend bool operator<= <>(const Point<T,dim> &, const Point<T,dim> &);
        friend bool operator>= <>(const Point<T,dim> &, const Point<T,dim> &);

        friend std::ostream &operator<< <>(std::ostream &, const Point<T,dim> &);
        friend std::istream &operator>> <>(std::istream &, Point<T,dim> &);

    private:
        static unsigned int newID();

    };

    //structure and functors for hash map
    struct mapKey
    {
        unsigned int p1,p2;
        mapKey(const unsigned int in1, const unsigned int in2)
        {
            p1=in1;
            p2=in2;
        }
    };

    struct mapKeyHash
    {
        std::size_t operator ()(const mapKey& key)const
        {
            unsigned int id1=key.p1;
            unsigned int id2=key.p2;

            if(id1>id2)
                std::swap(id1,id2);
            return std::hash<std::size_t>()((id1 + id2) * (id1 + id2 + 1) / 2 + id2);
        }
    };

    struct mapKeyEquality
    {
        bool operator()(const mapKey& lhs,const mapKey& rhs)const
        {
            return (lhs.p1 == rhs.p1 && lhs.p2 == rhs.p2)||(lhs.p1 == rhs.p2 && lhs.p2 == rhs.p1);
        }
    };



    template <typename T,int dim>
    Clustering::Point<T,dim>::Point()
    {
        //this->dim=dim;
        values.resize(dim);
        id=newID();
        for(int x=0;x<dim;x++)
            values[x]=0;
    }
    template <typename T,int dim>
    Clustering::Point<T,dim>::Point(T *inArray)
    {
        id=newID();
        for(int x=0;x<dim;x++)
            values[x]=inArray[x];
    }
    template <typename T,int dim>
    Clustering::Point<T,dim>::Point(const Clustering::Point<T,dim> &inPoint)
    {
        *this=inPoint;
    }
    template <typename T,int dim>
    Clustering::Point<T,dim>& Clustering::Point<T,dim>::operator=(const Clustering::Point<T,dim> &inPoint)
    {
        //values.resize(dim);
        values=inPoint.values;
        id=inPoint.id;
        return *this;
    }
    template <typename T,int dim>
    Clustering::Point<T,dim>::~Point()
    {
    }
    template <typename T,int dim>
    void Clustering::Point<T,dim>::setValue(int index, T newVal)
    {
        values[index]=newVal;
    }
    template <typename T,int dim>
    T Clustering::Point<T,dim>::getValue(int index) const
    {
        return values[index];
    }
    template <typename T,int dim>
    double Clustering::Point<T,dim>::distanceTo(const Clustering::Point<T,dim> &inPoint) const
    {
        T sum=0, diff=0;
        for(int x=0;x<dim;x++)
        {
            diff=values[x]-inPoint.values[x];
            sum += diff * diff;
        }
        return sqrt(sum);
    }
    template <typename T,int dim>
    Clustering::Point<T,dim> &Clustering::Point<T,dim>::operator*=(double modifier)
    {
        for(int x=0;x<dim;x++)
            values[x]*=modifier;
        return *this;
    }
    template <typename T,int dim>
    Clustering::Point<T,dim> &Clustering::Point<T,dim>::operator/=(double modifier)
    {
        for(int x=0;x<dim;x++)
            values[x]/=modifier;
        return *this;
    }
    template <typename T,int dim>
    const Clustering::Point<T,dim> Clustering::Point<T,dim>::operator*(double modifier) const
    {
        Clustering::Point<T,dim> outPoint(*this);
        outPoint*=modifier;
        return outPoint;
    }
    template <typename T,int dim>
    const Clustering::Point<T,dim> Clustering::Point<T,dim>::operator/(double modifier) const
    {
        Clustering::Point<T,dim> outPoint(*this);
        outPoint/=modifier;
        return outPoint;
    }

    template <typename T,int dim>
    Point<T,dim> &operator+=(Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
            lhs.values[x] += rhs.values[x];
        return lhs;
    }
    template <typename T,int dim>
    Clustering::Point<T,dim> &operator-=(Clustering::Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
            lhs.values[x] -= rhs.values[x];
        return lhs;
    }
    template <typename T,int dim>
    const Clustering::Point<T,dim> operator+(const Clustering::Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        Clustering::Point<T,dim> outPoint(lhs);
        for (int x = 0; x < dim; x++)
            outPoint.values[x] += rhs.values[x];
        return outPoint;
    }
    template <typename T,int dim>
    const Clustering::Point<T,dim> operator-(const Clustering::Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        Clustering::Point<T,dim> outPoint(lhs);
        for (int x = 0; x <dim; x++)
            outPoint.values[x] -= rhs.values[x];
        return outPoint;
    }
    template <typename T,int dim>
    bool operator==(const Clustering::Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
            if (lhs.values[x] != rhs.values[x])
                return false;
        return true;
    }
    template <typename T,int dim>
    bool operator!=(const Clustering::Point<T,dim> &lhs, const Clustering::Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
            if (lhs.values[x] != rhs.values[x])
                return true;
        return false;
    }
    template <typename T,int dim>
    bool operator<(const Point<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
        {
            if (lhs.values[x] < rhs.values[x])
                return true;
            if (lhs.values[x] > rhs.values[x])
                return false;
        }
        return false;
    }
    template <typename T,int dim>
    bool operator>(const Point<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
        {
            if (lhs.values[x] > rhs.values[x])
                return true;
            if (lhs.values[x] < rhs.values[x])
                return false;
        }
        return false;
    }
    template <typename T,int dim>
    bool operator<=(const Point<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
        {
            if (lhs.values[x] < rhs.values[x])
                return true;
            if (lhs.values[x] > rhs.values[x])
                return false;
        }
        return true;
    }
    template <typename T,int dim>
    bool operator>=(const Point<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        for (int x = 0; x < dim; x++)
        {
            if (lhs.values[x] > rhs.values[x])
                return true;
            if (lhs.values[x] <rhs.values[x])
                return false;
        }
        return true;
    }
    template <typename T,int dim>
    std::ostream &operator<<(std::ostream &os, const Point<T,dim> &inPoint)
    {
        for (int x = 0; x < dim; x++)
        {
            os<<std::showpoint;
            os << inPoint.values[x];
            if(x<dim-1)
                os<<",";
        }
        return os;
    }
    template <typename T,int dim>
    std::istream &operator>>(std::istream &is, Clustering::Point<T,dim> &inPoint)
    {
        std::string worker;

        for(int x=0;getline(is,worker,',');x++)
        {
            inPoint.values[x] = stod(worker);
        }

        return is;
    }

    template <typename T,int dim>
    unsigned int Point<T,dim>::newID()
    {
        static unsigned int id = 0;
        id++;
        return id;
    }
}
#endif //CLUSTERING_POINT_H