#ifndef CLUSTERING_KMEANS_H
#define CLUSTERING_KMEANS_H

#include <iostream>
#include "Clustering.h"

namespace Clustering
{
    class KMeans
    {
    public:
        static constexpr double SCORE_DIFF_THRESHOLD=.0001;
        static constexpr int STATLEVEL = 2;
    private:
        Cluster *clusterArray;
        double score, scoreDiff;
        int k;
        unsigned long int numberOfIterations;
    public:
        KMeans(int k,std::istream &is);
        ~KMeans();
        void computeClusteringScore();
        void recalculateInvalidCentroids();
        void iterateOnce();
        void iterateUntill(unsigned long int);//iterates until scorediff below threshold, or x times(0 for infinite iterations)
        void outputPoints(std::ostream& os);
        void clusterStats();
    };
}

#endif//CLUSTERING_KMEANS_H