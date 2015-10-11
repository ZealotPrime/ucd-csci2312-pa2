//
// Created by zealot on 9/20/15.
//

#include "Clustering.h"
#include <string>
#include <sstream>

namespace Clustering
{
   Cluster::Cluster(const Cluster &inCluster)
    {
        size=0;
        *this=inCluster;
        id=getId();
        releasePoints=false;

    }

    Cluster::Cluster(bool release)
    {
        size=0;
        head=nullptr;
        id=getId();
        releasePoints=release;
    }

    Cluster &Cluster::operator=(const Cluster &inCluster)
    {
        LNodePtr remote=inCluster.head;
        while(size>0)
        {
            remove(head->p);
        }
        if(remote!= nullptr) {
            add(remote->p);
            while (remote->next != nullptr) {
                remote = remote->next;
                add(remote->p);
            }
        }

    return *this;
    }

    Cluster::~Cluster()
    {
        PointPtr temp;
        while(size>0)
        {
            if(releasePoints)//if relasePoints set, delete point and node
            {
                temp=head->p;
                remove(head->p);
                delete temp;
            }
            else
                remove(head->p);//otherwise just remove the node
        }
    }

    void Cluster::add(const PointPtr &inPoint)
    {
        LNodePtr seeker=head, trailer=head, newNode=new LNode;

        newNode->next= nullptr;//initialize new node
        newNode->p=inPoint;

        if(size==0)//if cluster currently empty, newNode is head
        {
            head=newNode;
            size++;
            return;
        }
        if(seeker->next== nullptr)//if cluster has 1 point
        {
            if (seeker->p < inPoint)//put new point at end if it's bigger
            {
                seeker->next = newNode;
                size++;
                return;
            }
            else//put it in front if it's not
            {
                newNode->next=head->next;
                head=newNode;
                return;
            }
        }
        while(seeker->next!= nullptr)//seek to appropriate spot if more than 1
        {
            if(seeker->p >= inPoint)//if it's smaller or the same, add it in front
            {
                newNode->next=trailer->next;
                trailer->next=newNode;
                size++;
                return;
            }
            trailer=seeker;
            seeker=seeker->next;
        }
        if(seeker->p >= inPoint)//if it's smaller or the same as the last point(which the loop didn't test, add it in front
        {
            newNode->next=trailer->next;
            trailer->next=newNode;
            size++;
            return;
        }
        seeker->next=newNode;//if it hasn't been added yet, it must go at the end
        size++;
    }

    const PointPtr& Cluster::remove(const PointPtr &target)
    {
        LNodePtr seeker=head, trailer=head;
        if(size==0)
            return target; //return target if cluster empty
        while(seeker->p!=target&&seeker->next!= nullptr)//look for target point
        {
            trailer =seeker;
            seeker=seeker->next;
        }
        if(seeker->p==target)//if it was found, kill it
        {
            trailer->next=seeker->next;
            if(seeker==head)
               head=seeker->next;
            delete seeker;
            size--;
        }
        return target;
    }

    std::ostream &operator<<(std::ostream &os, const Cluster &cluster)
    {
        if(cluster.size==0)
        {
            return os;
        }
        LNodePtr seeker=cluster.head;
        do
        {
            os<< *(seeker->p)<<" : "<<cluster.id<<std::endl;
            seeker=seeker->next;
        }while(seeker!= nullptr);
        return os;
    }

    std::istream &operator>>(std::istream &istream, Cluster &cluster)
    {
        int dims=1;
        std::string worker;
        PointPtr newPoint;

        getline(istream,worker,'\n');//get first line
        istream.seekg(istream.beg);//return to beginning of string after reading in the string to count dims
        std::stringstream workStream(worker);//put it into a stream for sending to the point >>operator

        for(int x=0;x<worker.size();x++)//count number of delimiters to determine how many dimentions point has
            if(worker[x]==Clustering::DELIM)
                dims++;



        while(getline(istream,worker,'\n'))//gets lines from the stream, puts them into a string
        {
            std::cout<<"Worker: "<<worker<<std::endl;
            workStream.str(worker);//makes the string into a stream to send to point
            newPoint= new Point(dims);//create point
            workStream>>*newPoint;//have it set data using the stream
            cluster.add(newPoint);
            workStream.clear();
        }

        return istream;
    }

    bool operator==(const Cluster &lhs, const Cluster &rhs)
    {
        if(lhs.size==0&&rhs.size==0)
            return true;//clusters equal if they're both empty
        LNodePtr lSeeker=lhs.head;
        LNodePtr rSeeker=rhs.head;
        int counter=0;
        while(lSeeker->next!= nullptr&&rSeeker->next!= nullptr)
        {
            lSeeker=lSeeker->next;
            rSeeker=rSeeker->next;
            counter++;
        }
        if(counter==lhs.size&&counter==rhs.size&&lSeeker->p==rSeeker->p)//if testing final element and final element is the same
            return true;//then it's equal
        return false;//otherwise it's not
    }

    Cluster &Cluster::operator+=(const Cluster &rhs)
    {
        LNodePtr lSeeker=head;
        LNodePtr rSeeker=rhs.head;
        int y;
        for(int x=0;x<rhs.size;x++)//iterate through the rhs's points
        {
            for(y=0;y<size;y++)//compare against all the lhs's points
            {
                if(lSeeker->p==rSeeker->p)//if we found the current point in the lhs already, break out of the loop
                    break;
                lSeeker=lSeeker->next;
            }
            if(y==size)//if we didn't find it, add it.
                add(rSeeker->p);
            rSeeker=rSeeker->next;//check the next one for matches
            lSeeker=head;
        }
        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &rhs)
    {
        LNodePtr lSeeker=head;
        LNodePtr rSeeker=rhs.head;
        for(int x=0;x<rhs.size;x++)//iterate through the rhs's points
        {
            for(int y=0;y<size;y++)//compare against all the lhs's points
            {
                if(lSeeker->p==rSeeker->p)//if we found the current point in the lhs and rhs, pull it from the lhs and break out of the loop
                {
                    remove(lSeeker->p);
                    break;
                }
                lSeeker=lSeeker->next;
            }
            rSeeker=rSeeker->next;//check the next one for matches
            lSeeker=head;
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
        LNodePtr seeker=head;
        for(int x=0;x<size;x++)
        {
            if(*(seeker->p)==rhs)
            {
                remove(seeker->p);
                break;
            }

            seeker=seeker->next;
        }

        return *this;
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs)
    {
        Cluster output(lhs);
        output+=rhs;
        return output;
    }

    const Cluster operator-(const Cluster &lhs, const Cluster &rhs)
    {
        Cluster output(lhs);
        output-=rhs;
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


    unsigned int Cluster::getId()
    {
        static unsigned int id=0;
        id++;
        return id;
    }



}