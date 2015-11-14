//
// Created by zealot on 9/20/15.
//

#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include <forward_list>
#include "Point.h"


namespace Clustering
{

    static const char DELIM=',';
    typedef Point<Double> *PointPtr;


    class Cluster
    {
        friend class KMeans;
        unsigned int size;
        unsigned int id;
        std::forward_list<Point<Double>> points;
        PointPtr __centroid;
        bool centroidValidity;


    public:
        Cluster();//default ctor.
        // The big three: cpy ctor, overloaded operator=, dtor
        Cluster(const Cluster &);
        Cluster &operator=(const Cluster &);
        ~Cluster();

        // Set functions
        void add(const Point<Double> &);
        void remove(const Point<Double> &);
        void setCentroid(const Point<Double>&);
        void setCentroidValidity(bool isValid){ centroidValidity =isValid;}
        class Move
        {
        public:
            Move(const Point<Double> &target,Cluster *from,Cluster *to){perform(target,from,to);};
        private:
            void perform(const Point<Double> &target,Cluster *from,Cluster *to)
            {
                to->add(target);
                from->remove(target);
            }
        };


        //getters
        const unsigned int getID(){return id;}
        const Point<Double>& getCentroid(){return *__centroid;}
        const bool centrodValid(){return centroidValidity;}
        const unsigned long int getSize(){return size;}
        unsigned int getClusterEdges(){return size * (size - 1) / 2;}

        friend unsigned int getInterClusterEdges(Cluster&, Cluster&);

        //computation functions
        void computeCentroid();
        double intraClusterDistance();
        friend double interClusterDistance(Cluster &lhs,Cluster &rhs);
        void pickPoints(int k, PointPtr* pointArray);

        // Overloaded operators
        // IO
        friend std::ostream &operator<<(std::ostream &, const Cluster &);
        friend std::istream &operator>>(std::istream &, Cluster &);

        // Set-preserving operators (do not duplicate points in the space)
        // - Friends
        friend bool operator==(const Cluster &lhs, const Cluster &rhs);

        // - Members
        Cluster &operator+=(const Cluster &rhs); // union
        Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

        Cluster &operator+=(const Point<Double> &rhs); // add point
        Cluster &operator-=(const Point<Double> &rhs); // remove point

        // Set-destructive operators (duplicate points in the space)
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

        friend const Cluster operator+(const Cluster &lhs, const Point<Double> &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Point<Double> &rhs);

    private:
        unsigned int newID(); //returns a unique ID each time it's called




    };

}
#endif //CLUSTERING_CLUSTER_H
