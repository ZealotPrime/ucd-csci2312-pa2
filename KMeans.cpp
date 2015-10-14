//
// Created by zealot on 10/12/15.
//

#include "KMeans.h"

namespace Clustering
{
    KMeans::KMeans(int k, std::istream &is)
    {
        PointPtr initialCentroids[k];
        clusterArray= new Cluster[k];//create array of k clusters
        clusterArray[0].setReleasePoints(true);//set the master to release it's points
        this->k=k;
        scorediff=SCORE_DIFF_THRESHOLD+1;
        score=1000.0;

        is>>clusterArray[0];//load teh dataz into master cluster
        clusterArray[0].pickPoints(k,initialCentroids);//get initial centroids from the dataset
        for(int x=0;x<k;x++)
        {
            clusterArray[x].setCentroid(*initialCentroids[x]);//set each cluster's respective centroid from the generated centroids
        }
    }

    double KMeans::computeClusteringScore()
    {
        double intra=0,inter=0;
        for(int x=0;x<k;x++)
        {
            intra+=(clusterArray[x].intraClusterDistance())/k;//calc mean intracluster distance
        }
        for(int x=0;x<k-1;x++)
            for(int y=x+1;y<k;y++)
            {
                inter+=(interClusterDistance(clusterArray[x],clusterArray[y])/k);//calc mean intercluster distance
            }
        return intra/inter;//return the ratio of the two
    }

    void KMeans::iterateOnce()
    {
        double tempDistance=0;
        for(unsigned long int x=0;x<k;x++)//go through each cluster
        {
            for(unsigned long int y;y<clusterArray[x].getSize();y++)//test each of it's points
            {
                tempDistance=clusterArray[x][y]->distanceTo(clusterArray[x].getCentroid());
                for(unsigned long int z=x+1;z<k;z++)//against the other points
                {
                    if(clusterArray[x][y]->distanceTo(clusterArray[z].getCentroid())<tempDistance)


                }
            }
        }
    }
}