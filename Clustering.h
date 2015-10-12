//
// Created by zealot on 9/20/15.
//

#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include "Point.h"

namespace Clustering {

    static const char DELIM=',';
    typedef Point *PointPtr;
    typedef struct LNode *LNodePtr;


    struct LNode {
        PointPtr p;
        LNodePtr next;
    };

    class Cluster {
        int size;
        LNodePtr head;
        unsigned int id;
        bool releasePoints;
        PointPtr __centroid;
        bool centroidValidity;

    public:
        Cluster() : Cluster(false) {};//default ctor.

        Cluster(bool);

        // The big three: cpy ctor, overloaded operator=, dtor
        Cluster(const Cluster &);
        Cluster &operator=(const Cluster &);
        ~Cluster();

        // Set functions: They allow calling c1.add(c2.remove(p));
        void add(const PointPtr &);
        const PointPtr &remove(const PointPtr &);
        void setCentroid(const Point&);
        void setCentroidValidity(bool isValid){ centroidValidity =isValid;}


        //getters
        const unsigned int getID(){return id;}
        const Point getCentroid(){return *__centroid;}
        const bool centrodValid(){return centroidValidity;}
        const int getSize(){return size;}
        int getClusterEdges(){return size * (size - 1) / 2;}

        //computation functions
        void computeCentroid();
        double intraClusterDistance()const;
        friend double interClusterDistance(Cluster &lhs,Cluster &rhs);

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

        Cluster &operator+=(const Point &rhs); // add point
        Cluster &operator-=(const Point &rhs); // remove point

        // Set-destructive operators (duplicate points in the space)
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

        friend const Cluster operator+(const Cluster &lhs, const PointPtr &rhs);
        friend const Cluster operator-(const Cluster &lhs, const PointPtr &rhs);

    private:
        unsigned int setID(); //returns a unique ID for this instance of cluster




    };

}
#endif //CLUSTERING_CLUSTER_H
