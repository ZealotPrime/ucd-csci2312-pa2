//
// Created by zealot on 9/20/15.
//

#include "Clustering.h"

namespace Clustering
{

   Cluster::Cluster(const Cluster &inCluster)
    {
        LNodePtr local,remote;
        size=inCluster.size;
        if(size>0)
        {
            remote = inCluster.head;
            head= new LNode;
            head->p=remote->p;
            local = head->next;
            remote=remote->next;
            while (remote->next != nullptr)
            {
                local->next=new LNode;
                local->p=remote->p;
                local=local->next;
                remote=remote->next;
            }
            local=new LNode;
            local->p=remote->p;
            local->next= nullptr;

        }
    }


    Cluster &Cluster::operator=(const Cluster &inCluster)
    {
        LNodePtr local,remote;
        size=inCluster.size;
        if(size>0)
        {
            remote = inCluster.head;
            head= new LNode;
            head->p=remote->p;
            local = head->next;
            remote=remote->next;
            while (remote->next != nullptr)
            {
                local->next=new LNode;
                local->p=remote->p;
                local=local->next;
                remote=remote->next;
            }
            local=new LNode;
            local->p=remote->p;
            local->next= nullptr;

        }
    return *this;
    }

    Cluster::~Cluster()
    {
        LNodePtr current,subsequent;
        current=head;
        subsequent=current->next;
        while(subsequent->next!= nullptr)
        {
            delete current;
            current=subsequent->next;
            subsequent=current->next;
        }
        delete subsequent;
    }

    void Cluster::add(const PointPtr &inPoint)
    {
        LNodePtr seeker=head, trailer=head;
        if(size==0)//if cluster not currently empty
            while(seeker->p<inPoint&&seeker->next!= nullptr)//seek to appropriate spot
            {
            trailer =seeker;
            seeker=seeker->next;
            }
        trailer->next=new LNode;
        trailer = trailer->next;
        trailer->next=seeker;
        size++;

    }

    const PointPtr &Cluster::remove(const PointPtr &target)
    {
        LNodePtr seeker=head, trailer;
        if(size==0)
            return target; //return target if cluster empty
        while(seeker->p!=target&&seeker->next!= nullptr)
        {
            trailer =seeker;
            seeker=seeker->next;
        }
        if(seeker->p==target)
        {
            trailer->next=seeker->next;
            delete seeker;
            size--;
        }
        return target;
    }

    std::ostream &operator<<(std::ostream &os, const Cluster &cluster)
    {
        if(cluster.size==0)
        {
            os<<"Cluster "<<cluster<<" is empty"<<std::endl;
            return os;
        }
        LNodePtr seeker=cluster.head;
        os<<"Cluster "<<cluster<<" contains: ";
        while(seeker->next!= nullptr)
        {
            os<<seeker->p;
        }
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