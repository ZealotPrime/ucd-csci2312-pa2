//
// Created by zealot on 10/12/15.
//

#include <cmath>
#include "KMeans.h"

namespace Clustering
{
    KMeans::KMeans(int k, std::istream &is)
    {
        PointPtr initialCentroids[k];
        clusterArray= new Cluster[k];//create array of k clusters
        clusterArray[0].setReleasePoints(true);//set the master to release it's points
        this->k=k;
        scoreDiff =SCORE_DIFF_THRESHOLD+1;
        score=1000.0;
        numberOfIterations=0;

        is>>clusterArray[0];//load teh dataz into master cluster
        std::cout<<"Successfully loaded "<<clusterArray[0].getSize()<<" points"<<std::endl;
        std::cout<<"Initial cluster: "<<std::endl<<clusterArray[0];
        clusterArray[0].pickPoints(k,initialCentroids);//get initial centroids from the dataset
        for(int x=0;x<k;x++)
        {
            clusterArray[x].setCentroid(*(initialCentroids[x]));//set each cluster's respective centroid from the generated centroids
        }
    }

    void KMeans::computeClusteringScore()
    {
        double intra=0,inter=0,oldscore;
        for(int x=0;x<k;x++)
        {
            intra+=(clusterArray[x].intraClusterDistance())/k;//calc mean intracluster distance
        }
        for(int x=0;x<k-1;x++)
            for(int y=x+1;y<k;y++)
                inter+=(interClusterDistance(clusterArray[x],clusterArray[y])/k);//calc mean intercluster distance

        oldscore=score;
        score= intra/inter;//set score to the ratio of the two
        scoreDiff =std::fabs(score-oldscore);
    }

    void KMeans::recalculateInvalidCentroids()
    {
        for(int x=0;x<k;x++)
        {
            if(!clusterArray[x].centrodValid())
                clusterArray[x].computeCentroid();
        }
    }
    void KMeans::iterateOnce()
    {
        double tempDistance=0;
        bool nextPoint=false;
        for(int containingCluster =0; containingCluster <k; containingCluster++)//go through each cluster
        {
            for(unsigned long int currentPoint=0; currentPoint <clusterArray[containingCluster].getSize(); currentPoint++)//and each of it's points
            {
                tempDistance=clusterArray[containingCluster][currentPoint]->distanceTo(clusterArray[containingCluster].getCentroid());//get distance to current centroid
                for(int targetCluster = containingCluster +1; targetCluster <k; targetCluster++)//test against the other clusters
                {
                    if(clusterArray[containingCluster][currentPoint]->distanceTo(clusterArray[targetCluster].getCentroid())<tempDistance)//if it's closer to the target cluster
                    {
                        Cluster::Move(clusterArray[containingCluster][currentPoint], &(clusterArray[containingCluster]), &(clusterArray[targetCluster]));//move it
                        currentPoint--;
                        break;
                    }
                }
            }
        }
        recalculateInvalidCentroids();
        computeClusteringScore();
        std::cout<<"Iteration: "<<numberOfIterations++<<" Score: "<<score<<" ScoreDiff: "<< scoreDiff <<std::endl;
    }


    void KMeans::iterateUntill(unsigned long maxIterations)
    {
        bool limit;
        unsigned long int startIteration=numberOfIterations;
        maxIterations==0?(limit=false):(limit=true);//determine if the function is supposed to stop after a number of iterations
        while(scoreDiff>SCORE_DIFF_THRESHOLD)
        {
            iterateOnce();
            if(limit&&numberOfIterations-startIteration>maxIterations)//end before threshold if at max iterations
                return;
        }
    }

    void KMeans::outputPoints(std::ostream &os)
    {
        for(int x=0;x<k;x++)
            os<<(clusterArray[x]);
    }
}