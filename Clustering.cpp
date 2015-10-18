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
        id = setID();
        releasePoints = false;


    }

    Cluster::Cluster(bool release) {
        size = 0;
        head = nullptr;
        id = setID();
        releasePoints = release;
        __centroid = nullptr;
        centroidValidity = false;
    }

    Cluster &Cluster::operator=(const Cluster &inCluster) {
        LNodePtr remote = inCluster.head;
        while (size > 0) {
            remove(head->p);
        }
        if (remote != nullptr) {
            add(remote->p);
            while (remote->next != nullptr) {
                remote = remote->next;
                add(remote->p);
            }
        }

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

    Cluster::~Cluster() {
        PointPtr temp;
        while (size > 0) {
            if (releasePoints)//if relasePoints set, delete point and node
            {
                temp = head->p;
                remove(head->p);
                delete temp;
            }
            else
                remove(head->p);//otherwise just remove the node
        }
        if (__centroid != nullptr)//delete centroid if it exists
            delete __centroid;
    }

    void Cluster::add(const PointPtr &inPoint) {
        LNodePtr seeker = head, trailer = head, newNode = new LNode;

        newNode->next = nullptr;//initialize new node
        newNode->p = inPoint;
        centroidValidity = false;
        if (size == 0)//if cluster currently empty, newNode is head
        {
            head = newNode;
            size++;
            return;
        }
        if (head->next == nullptr)//if cluster has 1 point
        {
            if (*(head->p) < *(inPoint))//put new point at end if it's bigger
            {
                head->next = newNode;
                size++;
                return;
            }
            else//put it in front if it's not
            {
                newNode->next = head->next;
                head = newNode;
                size++;
                return;
            }
        }

        if (*(head->p) > *(inPoint))//if the new element should be 1st
        {
            newNode->next = head;
            head = newNode;
            size++;
            return;
        }
        seeker=head->next;
        while (seeker != nullptr)//seek to appropriate spot if more than 1
        {
            if (*(seeker->p) >= *(inPoint))//if it's smaller or the same, add it in front
            {
                newNode->next = trailer->next;
                trailer->next = newNode;
                size++;
                return;
            }
            trailer = seeker;
            seeker = seeker->next;
        }
        trailer->next = newNode;//if it hasn't been added yet, it must go at the end
        size++;
    }

    const PointPtr &Cluster::remove(const PointPtr &target) {
        LNodePtr seeker = head, trailer = head;
        if (size == 0)
            return target; //return target if cluster empty
        while (seeker->p != target && seeker->next != nullptr)//look for target point
        {
            trailer = seeker;
            seeker = seeker->next;
        }
        if (seeker->p == target)//if it was found, kill it
        {
            trailer->next = seeker->next;
            if (seeker == head)
                head = seeker->next;
            delete seeker;
            size--;
            centroidValidity = false;
        }
        return target;
    }

    std::ostream &operator<<(std::ostream &os, const Cluster &cluster) {
        if (cluster.size == 0) {
            return os;
        }
        LNodePtr seeker = cluster.head;
        do {
            os << *(seeker->p) << " : " << cluster.id << std::endl;
            seeker = seeker->next;
        } while (seeker != nullptr);
        return os;
    }

    std::istream &operator>>(std::istream &istream, Cluster &cluster) {
        int dims = 1;
        std::string worker;
        PointPtr newPoint;

        getline(istream, worker, '\n');//get first line
        istream.seekg(istream.beg);//return to beginning of string after reading in the string to count dims
        std::stringstream workStream(worker);//put it into a stream for sending to the point >>operator

        for (int x = 0; x < worker.size(); x++)//count number of delimiters to determine how many dimentions point has
            if (worker[x] == Clustering::DELIM)
                dims++;


        while (getline(istream, worker, '\n'))//gets lines from the stream, puts them into a string
        {
            workStream.str(worker);//makes the string into a stream to send to point
            newPoint = new Point(dims);//create point
            workStream >> *newPoint;//have it set data using the stream
            cluster.add(newPoint);
            workStream.clear();
        }

        return istream;
    }

    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if (lhs.size == 0 && rhs.size == 0)
            return true;//clusters equal if they're both empty
        LNodePtr lSeeker = lhs.head;
        LNodePtr rSeeker = rhs.head;
        unsigned long int counter = 0;
        while (lSeeker != nullptr && rSeeker != nullptr)
        {
            if(*(lSeeker->p)!=*(rSeeker->p))//if the points aren't equal, the clusters aren't equal
                return false;
            lSeeker = lSeeker->next;
            rSeeker = rSeeker->next;
            counter++;
        }
        if (counter == lhs.size && counter == rhs.size &&
            lSeeker->p == rSeeker->p)//if testing final element and final element is the same
            return true;//then it's equal
        return false;//otherwise it's not
    }

    Cluster &Cluster::operator+=(const Cluster &rhs) {
        LNodePtr lSeeker = head;
        LNodePtr rSeeker = rhs.head;
        unsigned long int y;
        for (unsigned long int x = 0; x < rhs.size; x++)//iterate through the rhs's points
        {
            for (y = 0; y < size; y++)//compare against all the lhs's points
            {
                if (lSeeker->p == rSeeker->p)//if we found the current point in the lhs already, break out of the loop
                    break;
                lSeeker = lSeeker->next;
            }
            if (y == size)//if we didn't find it, add it.
                add(rSeeker->p);
            rSeeker = rSeeker->next;//check the next one for matches
            lSeeker = head;
        }
        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &rhs) {
        LNodePtr lSeeker = head;
        LNodePtr rSeeker = rhs.head;
        for (unsigned long int x = 0; x < rhs.size; x++)//iterate through the rhs's points
        {
            for (unsigned long int y = 0; y < size; y++)//compare against all the lhs's points
            {
                if (lSeeker->p ==
                    rSeeker->p)//if we found the current point in the lhs and rhs, pull it from the lhs and break out of the loop
                {
                    remove(lSeeker->p);
                    break;
                }
                lSeeker = lSeeker->next;
            }
            rSeeker = rSeeker->next;//check the next one for matches
            lSeeker = head;
        }
        return *this;
    }

    Cluster &Cluster::operator+=(const Point &rhs)
    {
        PointPtr inPoint = new Point(rhs);
        add(inPoint);
        return *this;
    }

    Cluster &Cluster::operator-=(const Point &rhs)
    {
        LNodePtr seeker = head;
        for (; seeker!= nullptr; seeker = seeker->next)
        {
            if (*(seeker->p) == rhs)
            {
                remove(seeker->p);
                break;
            }
        }

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

    const Cluster operator+(const Cluster &lhs, const PointPtr &rhs)
    {
        Cluster output(lhs);
        output.add(rhs);
        return lhs;
    }

    const Cluster operator-(const Cluster &lhs, const PointPtr &rhs)
    {
        Cluster output(lhs);
        output.remove(rhs);
        return lhs;
    }


    unsigned int Cluster::setID()
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
        LNodePtr seeker;
        if (__centroid == nullptr)//if centroid hasn't been built yet,
        {
            __centroid = new Point(head->p->getDims());//make one based on existing point dimentionality.
        }
        else
            for (int x = 0; x < __centroid->getDims(); x++)//zero out the centroid
            {
                __centroid->setValue(x, 0);
            }
        for (seeker = head; seeker != nullptr; seeker = seeker->next)
        {
            *__centroid += (*(seeker->p) / size);
        }
        centroidValidity = true;
    }

    double Cluster::intraClusterDistance() const
    {
        LNodePtr outer, inner;
        double distance = 0;

        if (size < 2)//case for 0 or 1 points
            return 0.0;
        if (size == 2)//case for 2 points
            return head->p->distanceTo(*(head->next->p));

        for (outer = head; outer->next->next != nullptr; outer = outer->next)//case for 3 or more
        {
            for (inner = outer->next; inner != nullptr; inner = inner->next)//start at outer's current location to prevent counting the same pairs twice
            {
                distance += (outer->p->distanceTo(*(inner->p))) / size;
            }
        }
        return distance;
    }


    double interClusterDistance(Cluster &lhs, Cluster &rhs)
    {
        if(lhs.__centroid== nullptr||rhs.__centroid== nullptr)
        {
            std::cout<<"Error, one or both target clusters lack centroids"<<std::endl;
            return 0.0;
        }
        return lhs.__centroid->distanceTo(*(rhs.__centroid));
    }

    void Cluster::pickPoints(int k, PointPtr *pointArray)
    {
        unsigned long int spread=size/k,x,index=0;
        LNodePtr seeker;
        for(x=0,seeker=head;seeker!= nullptr,index<k;x++,seeker=seeker->next)//iterate through nodes
        {
            if(x%spread==0)//if the index of the node we're on is a multiple of the spread, add it's point to the array
                pointArray[index++]=seeker->p;
        }
        for(;index<k;index++)//add dummy points in case k>size
            pointArray[index]=head->p;
    }


    const PointPtr Cluster::operator[](unsigned long int index)
    {

        LNodePtr seeker=head;
        unsigned long int x;
        for(x=0;seeker!= nullptr; x++)
        {
            if (x == index)
                return seeker->p;
            seeker = seeker->next;
        }
        return nullptr;
    }
}