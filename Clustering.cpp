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

    void Cluster::add(PointPtr const &inPoint)
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

    const PointPtr &Cluster::remove(PointPtr const &target)
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
        }
        return target;
    }

    std::ostream &operator<<(std::ostream &os, const Cluster &cluster)
    {
        if(cluster.size==0)
            return os;
        LNodePtr seeker=head;
        while(seeker->next!= nullptr)
        {
            os<<seeker->p;
        }
        return <#initializer#>;
    }


}