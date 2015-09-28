//
// Created by zealot on 9/20/15.
//

#include "Clustering.h"

namespace Clustering
{

   Cluster::Cluster(const Cluster &inCluster)
    {
        size=0;
        *this=inCluster;
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
        LNodePtr current,subsequent;
        current=head;
        subsequent=current->next;
        while(current->next!= nullptr)
        {
            delete current;
            current=subsequent->next;
            subsequent=current->next;
        }
        delete subsequent;
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
            os<<"Cluster "<<(&cluster)<<" is empty"<<std::endl;
            return os;
        }
        LNodePtr seeker=cluster.head;
        os<<"Cluster "<<(&cluster)<<" contains: ";
        do
        {
            os<< *(seeker->p)<<" ";
            seeker=seeker->next;
        }while(seeker!= nullptr);
        os<<std::endl;
        return os;
    }


    bool operator==(const Cluster &lhs, const Cluster &rhs)
    {
        if(lhs.size==0&&rhs.size==0)
            return true;//clusters equal if they're both empty
        LNodePtr lSeeker=lhs.head;
        LNodePtr rSeeker=rhs.head;
        int counter=0;
        while(lSeeker->p!= nullptr&&rSeeker!= nullptr)
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
        }
        return *this;
    }

    Cluster &Cluster::operator-=(const Cluster &rhs)
    {
        LNodePtr lSeeker=head;
        LNodePtr rSeeker=rhs.head;
        int y;
        for(int x=0;x<rhs.size;x++)//iterate through the rhs's points
        {
            for(y=0;y<size;y++)//compare against all the lhs's points
            {
                if(lSeeker->p==rSeeker->p)//if we found the current point in the lhs and rhs, pull it from the lhs and break out of the loop
                {
                    remove(lSeeker->p);
                    break;
                }
                lSeeker=lSeeker->next;
            }
            rSeeker=rSeeker->next;//check the next one for matches
        }
        return *this;
    }

    Cluster &Cluster::operator+=(const PointPtr &rhs)
    {
        add(rhs);
        return *this;
    }

    Cluster &Cluster::operator-=(const PointPtr &rhs)
    {
        remove(rhs);
        return *this;
    }

    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) //as per the header file, this will duplicate points, unlike the += operator
    {
        Cluster output;
        LNodePtr seeker=lhs.head;
        for(int x=0;x<lhs.size;x++)//add all points from lhs
        {
            output.add(seeker->p);
            seeker=seeker->next;
        }
        seeker=rhs.head;
        for(int x=0;x<rhs.size;x++)//add all points from rhs
        {
            output.add(seeker->p);
            seeker=seeker->next;
        }
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
}