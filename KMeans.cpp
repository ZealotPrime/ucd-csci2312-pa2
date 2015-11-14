//
// Created by zealot on 10/12/15.
//

#include <cmath>
#include "KMeans.h"

namespace Clustering
{
    KMeans::KMeans(int k, std::istream &is)
    {
        clock_t timer;
        PointPtr initialCentroids[k];
        clusterArray= new Cluster[k];//create array of k clusters
        this->k=k;
        scoreDiff =SCORE_DIFF_THRESHOLD+1;
        score=1000000.0;
        numberOfIterations=0;
        std::cout<<"Loading points..."<<std::flush;
        timer=clock();//get clock to time operation
        is>>clusterArray[0];//load teh dataz into master cluster
        timer=clock()-timer;
        std::cout<<"done in "<<(double)timer/CLOCKS_PER_SEC<<" seconds"<<std::endl<<"Successfully loaded "<<clusterArray[0].getSize()<<" points"<<std::endl;
        //std::cout<<"Loaded the following points: "<<std::endl<<clusterArray[0]<<std::endl;
        clusterArray[0].pickPoints(k,initialCentroids);//get initial centroids from the dataset
        for(int x=0;x<k;x++)
        {
            clusterArray[x].setCentroid(*(initialCentroids[x]));//set each cluster's respective centroid from the generated centroids
        }
    }

    KMeans::~KMeans()
    {
        delete[] clusterArray;
    }

    void KMeans::computeClusteringScore()
    {
        double intra=0,inter=0,oldscore;
        for(int x=0;x<k;x++)
        {
            intra+=(clusterArray[x].intraClusterDistance());//calc mean intracluster distance
        }
        for(int x=0;x<k-1;x++)
            for(int y=x+1;y<k;y++)
                inter+=interClusterDistance(clusterArray[x],clusterArray[y]);//calc mean intercluster distance


        oldscore=score;
        score=intra/inter;//set score to the ratio of the two
        scoreDiff =oldscore-score;//computer scoreDiff
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
        std::forward_list<Point>::iterator seeker,trailer;
        clock_t timer=clock();
            for (int containingCluster = 0; containingCluster < k; containingCluster++)//go through each cluster
            {
                if(STATLEVEL>2)
                    clusterStats();
                for (seeker=clusterArray[containingCluster].points.begin(); seeker!=clusterArray[containingCluster].points.end();trailer=seeker++)//and each of it's points
                {
                    tempDistance = seeker->distanceTo(clusterArray[containingCluster].getCentroid());//get distance to current centroid
                    for (int targetCluster = ((numberOfIterations==0)?(containingCluster+1):(containingCluster==0?1:0)); targetCluster < k; (++targetCluster==containingCluster?targetCluster++:1))//test against the other clusters
                    {// the question mark operator here is to make the system use a more efficient 1 way version of the algorithm if it's the first loop
                        if (seeker->distanceTo(clusterArray[targetCluster].getCentroid()) < tempDistance)//if it's closer to the target cluster
                        {
                            Cluster::Move(*seeker, &(clusterArray[containingCluster]), &(clusterArray[targetCluster]));//move it
                            seeker=trailer;
                            break;
                        }
                    }
                }
            }
        recalculateInvalidCentroids();
        computeClusteringScore();
        timer=clock()-timer;
        std::cout<<"Iteration: "<<numberOfIterations++<<" Score: "<<score<<"("<< scoreDiff<<") ("<<(double)timer/CLOCKS_PER_SEC<<" Seconds)" <<std::endl;
        if(STATLEVEL>1)
            clusterStats();
    }


    void KMeans::iterateUntill(unsigned long maxIterations)
    {
        bool limit;
        unsigned long int startIteration=numberOfIterations;
        clock_t timer=clock();
        maxIterations==0?(limit=false):(limit=true);//determine if the function is supposed to stop after a number of iterations
        while(scoreDiff>SCORE_DIFF_THRESHOLD)
        {
            iterateOnce();
            if(limit&&numberOfIterations-startIteration>maxIterations)//end before threshold if at max iterations
            {
                break;
            }
        }
        timer=clock()-timer;
        std::cout<<((double)timer/CLOCKS_PER_SEC)<<" seconds for all iterations, "<<((double)timer/CLOCKS_PER_SEC)/(numberOfIterations)<<" second average time per iteration"<<std::endl;
    }

    void KMeans::outputPoints(std::ostream &os)
    {
        for(int x=0;x<k;x++)
            os<<(clusterArray[x]);
    }

    void KMeans::clusterStats()
    {
        std::cout<<"Points per cluster: ";
        for(unsigned int x=0;x<k;x++)
        {
            std::cout<<clusterArray[x].getID()<<":"<<clusterArray[x].getSize()<<" ";
        }
        std::cout<<std::endl;
    }


}