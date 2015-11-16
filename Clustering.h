//
// Created by zealot on 9/20/15.
//

#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include <forward_list>
#include <sstream>
#include "Point.h"


namespace Clustering
{

    static const char DELIM=',';

//    //forward declarations for friend functions
    template <typename T, int dim> class Cluster;
    template <typename T,int dim> class Point;
    template <typename T, int dim> unsigned int getInterClusterEdges(Cluster<T,dim>&, Cluster<T,dim>&);
    template <typename T, int dim> double interClusterDistance(Cluster<T,dim> &lhs,Cluster<T,dim> &rhs);
    template <typename T, int dim> std::ostream &operator<<(std::ostream &, const Cluster<T,dim> &);
    template <typename T, int dim> std::istream &operator>>(std::istream &, Clustering::Cluster<T,dim> &);
    template <typename T, int dim> bool operator==(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);
    template <typename T, int dim> const Cluster<T,dim> operator+(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);
    template <typename T, int dim> const Cluster<T,dim> operator-(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);
    template <typename T, int dim> const Cluster<T,dim> operator+(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs);
    template <typename T, int dim> const Cluster<T,dim> operator-(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs);


    template <typename T, int dim>class Cluster
    {
        template <int in,int valid>friend class KMeans;
        unsigned int size;
        unsigned int id;
        std::forward_list<Point<T,dim>> points;
        Point<T,dim>* __centroid;
        bool centroidValidity;


    public:
        Cluster();//default ctor.
        // The big three: cpy ctor, overloaded operator=, dtor
        Cluster(const Cluster &);
        Cluster &operator=(const Cluster &);
        ~Cluster();

        // Set functions
        void add(const Point<T,dim> &);
        void remove(const Point<T,dim> &);
        void setCentroid(const Point<T,dim>&);
        void setCentroidValidity(bool isValid){ centroidValidity =isValid;}
        class Move
        {
        public:
            Move(const Point<T,dim> &target,Cluster<T,dim> *from,Cluster<T,dim> *to){perform(target,from,to);};
        private:
            void perform(const Point<T,dim> &target,Cluster<T,dim> *from,Cluster<T,dim> *to)
            {
                to->add(target);
                from->remove(target);
            }
        };


        //getters
        const unsigned int getID(){return id;}
        const Point<T,dim>& getCentroid(){return *__centroid;}
        const bool centrodValid(){return centroidValidity;}
        const unsigned long int getSize(){return size;}
        unsigned int getClusterEdges(){return size * (size - 1) / 2;}
        friend unsigned int getInterClusterEdges <>(Cluster<T,dim>&, Cluster<T,dim>&);

        //computation functions
        void computeCentroid();
        double intraClusterDistance();
        friend double interClusterDistance <>(Cluster<T,dim> &lhs,Cluster<T,dim> &rhs);
        void pickPoints(int k, Point<T,dim>** pointArray);

        // Overloaded operators
        // IO
        friend std::ostream &operator<< <>(std::ostream &, const Cluster<T,dim> &);
        friend std::istream &operator>> <>(std::istream &, Cluster<T,dim> &);

        // Set-preserving operators (do not duplicate points in the space)
        // - Friends
        friend bool operator== <>(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);

        // - Members
        Cluster<T,dim> &operator+=(const Cluster<T,dim> &rhs); // union
        Cluster<T,dim> &operator-=(const Cluster<T,dim> &rhs); // (asymmetric) difference

        Cluster<T,dim> &operator+=(const Point<T,dim> &rhs); // add point
        Cluster<T,dim> &operator-=(const Point<T,dim> &rhs); // remove point

        // Set-destructive operators (duplicate points in the space)
        // - Friends
        friend const Cluster<T,dim> operator+ <>(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);
        friend const Cluster<T,dim> operator- <>(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs);

        friend const Cluster<T,dim> operator+ <>(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs);
        friend const Cluster<T,dim> operator- <>(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs);

    private:
        unsigned int newID(); //returns a unique ID each time it's called




    };

    template <typename T, int dim>
    Cluster<T,dim>::Cluster(const Cluster<T,dim> &inCluster) {
        size = 0;
        __centroid = nullptr;
        *this = inCluster;
        id = newID();
    }

    template <typename T, int dim>
    Cluster<T,dim>::Cluster()
    {
        size = 0;
        id = newID();
        __centroid = nullptr;
        centroidValidity = false;
    }

    template <typename T, int dim>
    Cluster<T,dim> &Cluster<T,dim>::operator=(const Cluster<T,dim> &inCluster)
    {
        points=inCluster.points;//copy in the other cluster's points

        if (inCluster.__centroid == nullptr)//if the target doesn't have a centroid,
        {
            if (__centroid != nullptr)//but the current cluster does,
                delete __centroid;//delete our centroid.
        }
        else//if the target does have a centroid,
        {
            if (__centroid == nullptr)//and we don't,
                __centroid = new Point<T,dim>(*(inCluster.__centroid));//add one.
            else//but if we do already have a centroid,
                *__centroid = *(inCluster.__centroid);//make ours equal to the target's
        }
        return *this;
    }

    template <typename T, int dim>
    Cluster<T,dim>::~Cluster()
    {
        if (__centroid != nullptr)//delete centroid if it exists
            delete __centroid;
    }

    template <typename T, int dim>
    void Cluster<T,dim>::add(const Point<T,dim> &inPoint)
    {
        auto seeker=points.begin();
        auto trailer=points.before_begin();
        if(size==0)
        {
            points.push_front(inPoint);
            ++size;
            return;
        }
        for(;seeker!=points.end();trailer=seeker++)
        {
            if(inPoint<*seeker)
            {
                points.insert_after(trailer,inPoint);
                ++size;
                return;
            }
        }
        points.insert_after(trailer,inPoint);//if the code made it this far, it goes at the end
        ++size;
    }

    template <typename T, int dim>
    void Cluster<T,dim>::remove(const Point<T,dim> &target)
    {
        if (size == 0)
            return; //return target if cluster empty
        auto seeker=points.begin();
        auto trailer=points.before_begin();
        for(;seeker!=points.end();trailer=seeker++)
        {
            if(*seeker==target)
            {
                points.erase_after(trailer);
                --size;
                return;
            }
        }
        return;
    }

    template <typename T, int dim>
    std::ostream &operator<<(std::ostream &os, const Cluster<T,dim> &cluster) {
        if (cluster.size == 0) {
            return os;
        }

        for(auto seeker=cluster.points.begin();seeker!=cluster.points.end();++seeker)
        {
            os << *seeker << " : " << cluster.id << std::endl;
        }
        return os;
    }

    template <typename T, int dim>
    std::istream &operator>>(std::istream &istream, Clustering::Cluster<T,dim> &cluster) {
        int dims = 1;
        std::string worker;


        getline(istream, worker, '\n');//get first line
        istream.seekg(istream.beg);//return to beginning of string after reading in the string to count dims
        std::stringstream workStream(worker);//put it into a stream for sending to the point >>operator

//        for (int x = 0; x < worker.size(); x++)//count number of delimiters to determine how many dimentions point has
//            if (worker[x] == Clustering::DELIM)
//                dims++;
        Clustering::Point<T,dim> newPoint;

        while (getline(istream, worker, '\n'))//gets lines from the stream, puts them into a string
        {
            workStream.str(worker);//makes the string into a stream to send to point
            workStream >> newPoint;//have it set data using the stream
            cluster.add(newPoint);
            workStream.clear();
        }

        return istream;
    }

    template <typename T, int dim>
    bool operator==(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs) {
        if (lhs.size == 0 && rhs.size == 0)
            return true;//clusters equal if they're both empty
        if(lhs.size!=rhs.size)
            return false;//clusters not equal if they're different sizes
        auto left=lhs.points.begin();
        auto right=rhs.points.begin();
        for(;left!=lhs.points.end();++left,++right)
        {
            if(*left!=*right)
                return false;//if one of the points don't match, then the clusters don't
        }
        return true;//otherwise they must be the same
    }

    template <typename T, int dim>
    Cluster<T,dim> &Cluster<T,dim>::operator+=(const Cluster<T,dim> &rhs)
    {
        std::forward_list<Point<T,dim>> temp=rhs.points;
        points.merge(temp);

        return *this;
    }

    template <typename T, int dim>
    Cluster<T,dim> &Cluster<T,dim>::operator-=(const Cluster<T,dim> &rhs)
    {
        auto remote=rhs.points.begin();
        auto local=points.begin();
        for(;remote!=rhs.points.end();++remote)
        {
            for(local=points.begin();local!=points.end();++local)
            {
                if(*local==*remote)//if we have it, get rid of it it
                {
                    remove(*local);
                    break;
                }
                if(*local<*remote)//if we're past the point where it should be, then we must not have it
                {
                    break;
                }
            }
        }

        return *this;
    }

    template <typename T, int dim>
    Cluster<T,dim> &Cluster<T,dim>::operator+=(const Point<T,dim> &rhs)
    {
        add(rhs);
        return *this;
    }

    template <typename T, int dim>
    Cluster<T,dim> &Cluster<T,dim>::operator-=(const Point<T,dim> &rhs)
    {
        remove(rhs);
        return *this;
    }

    template <typename T, int dim>
    const Cluster<T,dim> operator+(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs)
    {
        Cluster<T,dim> output(lhs);
        output += rhs;
        return output;
    }

    template <typename T, int dim>
    const Cluster<T,dim> operator-(const Cluster<T,dim> &lhs, const Cluster<T,dim> &rhs)
    {
        Cluster<T,dim> output(lhs);
        output -= rhs;
        return output;
    }

    template <typename T, int dim>
    const Cluster<T,dim> operator+(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        Cluster<T,dim> output(lhs);
        output.add(rhs);
        return lhs;
    }

    template <typename T, int dim>
    const Cluster<T,dim> operator-(const Cluster<T,dim> &lhs, const Point<T,dim> &rhs)
    {
        Cluster<T,dim> output(lhs);
        output.remove(rhs);
        return lhs;
    }

    template <typename T, int dim>
    unsigned int Cluster<T,dim>::newID()
    {
        static unsigned int id = 0;
        id++;
        return id;
    }

    template <typename T, int dim>
    void Cluster<T,dim>::setCentroid(const Point<T,dim> &location)
    {
        if (__centroid != nullptr)//if we already have a centroid,
            *__centroid = location;//set it to the target location.
        else//if we don't,
            __centroid = new Point<T,dim>(location);//make one with the target's location
    }

    template <typename T, int dim>
    void Cluster<T,dim>::computeCentroid()
    {
        if (size == 0) {
            std::cout << "Cluster empty, cannot compute" << std::endl;
            return;
        }
        if (__centroid == nullptr)//if centroid hasn't been built yet,
        {
            __centroid=new Point<T,dim>();//make one based on existing point dimentionality.
        }
        else
            for (int x = 0; x < dim; x++)//zero out the centroid
            {
                __centroid->setValue(x, 0);
            }
        auto seeker=points.begin();
        while(seeker!=points.end())
        {
            *__centroid+=*seeker/size;
            ++seeker;
        }
        centroidValidity = true;
    }

    template <typename T, int dim>
    double Cluster<T,dim>::intraClusterDistance()
    {
        auto outer=points.begin(), inner=points.begin();

        if (size < 2)//case for 0 or 1 points
            return 0.0;
        if (size == 2)//case for 2 points
        {
            ++inner;
            return outer->distanceTo(*inner);
        }

        double distance = 0;
        unsigned int edges=getClusterEdges();
        while(outer!=points.end())//case for 3 or more
        {
            inner=outer;
            ++inner;
            while(inner!=points.end())
            {
                distance += outer->distanceTo(*inner) / edges;
                ++inner;
            }
            ++outer;
        }
        return distance;
    }

    template <typename T, int dim>
    double interClusterDistance(Cluster<T,dim> &lhs, Cluster<T,dim> &rhs)
    {
        unsigned int edges=getInterClusterEdges(lhs,rhs);
        auto left=lhs.points.begin(), right=rhs.points.begin();
        double distance=0;

        while(left!=lhs.points.end())
        {
            while(right!=rhs.points.end())
            {
                distance += (left->distanceTo(*right)) / edges;
                ++right;
            }
            ++left;
        }
        return distance;
    }

    template <typename T, int dim>
    void Cluster<T,dim>::pickPoints(int k, Point<T,dim>* pointArray[])
    {
        unsigned int spread=size/k,x,index=0;
        auto seeker=points.begin();
        for(x=0;seeker!= points.end(),index<k;x++,++seeker)//iterate through nodes
        {
            if(x%spread==0)//if the index of the node we're on is a multiple of the spread, add it's point to the array
                pointArray[index++]=&(*seeker);
        }
        for(;index<k;index++)//add dummy points in case k>size
            pointArray[index]=&*(points.end());
    }


    template <typename T, int dim>
    unsigned int getInterClusterEdges(Cluster<T,dim> &c1, Cluster<T,dim> &c2)
    {
        unsigned int totalSize=c1.size+c2.size;
        return (totalSize*(totalSize-1)/2);
    }

}
#endif //CLUSTERING_CLUSTER_H
