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
}