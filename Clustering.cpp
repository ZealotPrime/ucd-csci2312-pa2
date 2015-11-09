//
// Created by zealot on 9/20/15.
//

#include "Clustering.h"
#include <string>
#include <sstream>

namespace Clustering {
    Cluster::Cluster(const Cluster &inCluster) {
        size = 0;
        __centroid = nullptr;
        *this = inCluster;
        id = newID();
    }

    Cluster::Cluster()
    {
        size = 0;
        id = newID();
        __centroid = nullptr;
        centroidValidity = false;
    }

    Cluster &Cluster::operator=(const Cluster &inCluster)
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
                __centroid = new Point(*(inCluster.__centroid));//add one.
            else//but if we do already have a centroid,
                *__centroid = *(inCluster.__centroid);//make ours equal to the target's
        }
        return *this;
    }

    Cluster::~Cluster()
    {
        if (__centroid != nullptr)//delete centroid if it exists
            delete __centroid;
    }

    void Cluster::add(const Point &inPoint)
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

    void Cluster::remove(const Point &target)
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

    std::ostream &operator<<(std::ostream &os, const Cluster &cluster) {
        if (cluster.size == 0) {
            return os;
        }

        for(auto seeker=cluster.points.begin();seeker!=cluster.points.end();++seeker)
        {
            os << *seeker << " : " << cluster.id << std::endl;
        }
        return os;
    }

    std::istream &operator>>(std::istream &istream, Cluster &cluster) {
        int dims = 1;
        std::string worker;


        getline(istream, worker, '\n');//get first line
        istream.seekg(istream.beg);//return to beginning of string after reading in the string to count dims
        std::stringstream workStream(worker);//put it into a stream for sending to the point >>operator

        for (int x = 0; x < worker.size(); x++)//count number of delimiters to determine how many dimentions point has
            if (worker[x] == Clustering::DELIM)
                dims++;
        Point newPoint(dims);

        while (getline(istream, worker, '\n'))//gets lines from the stream, puts them into a string
        {
            workStream.str(worker);//makes the string into a stream to send to point
            workStream >> newPoint;//have it set data using the stream
            cluster.add(newPoint);
            workStream.clear();
        }

        return istream;
    }

    bool operator==(const Cluster &lhs, const Cluster &rhs) {
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

    Cluster &Cluster::operator+=(const Cluster &rhs)
    {
        std::forward_list<Point> temp=rhs.points;
        points.merge(temp);

        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &rhs)
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

    Cluster &Cluster::operator+=(const Point &rhs)
    {
        add(rhs);
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &rhs)
    {
        remove(rhs);
        return *this;
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs)
    {
        Cluster output(lhs);
        output += rhs;
        return output;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs)
    {
        Cluster output(lhs);
        output -= rhs;
        return output;
    }

    const Cluster operator+(const Cluster &lhs, const Point &rhs)
    {
        Cluster output(lhs);
        output.add(rhs);
        return lhs;
    }

    const Cluster operator-(const Cluster &lhs, const Point &rhs)
    {
        Cluster output(lhs);
        output.remove(rhs);
        return lhs;
    }


    unsigned int Cluster::newID()
    {
        static unsigned int id = 0;
        id++;
        return id;
    }


    void Cluster::setCentroid(const Point &location)
    {
        if (__centroid != nullptr)//if we already have a centroid,
            *__centroid = location;//set it to the target location.
        else//if we don't,
            __centroid = new Point(location);//make one with the target's location
    }

    void Cluster::computeCentroid()
    {
        if (size == 0) {
            std::cout << "Cluster empty, cannot compute" << std::endl;
            return;
        }
        if (__centroid == nullptr)//if centroid hasn't been built yet,
        {
            __centroid=new Point(points.begin()->getDims());//make one based on existing point dimentionality.
        }
        else
            for (int x = 0; x < __centroid->getDims(); x++)//zero out the centroid
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

    double Cluster::intraClusterDistance()
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


    double interClusterDistance(Cluster &lhs, Cluster &rhs)
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

    void Cluster::pickPoints(int k, PointPtr pointArray[])
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


    unsigned int getInterClusterEdges(Cluster &c1, Cluster &c2)
    {
        unsigned int totalSize=c1.size+c2.size;
        return (totalSize*(totalSize-1)/2);
    }
}