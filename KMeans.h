#ifndef CLUSTERING_KMEANS_H
#define CLUSTERING_KMEANS_H

#include <iostream>
#include "Clustering.h"

namespace Clustering
{
    class KMeans
    {
    public:
        static constexpr double SCORE_DIFF_THRESHOLD=2;
    private:
        Cluster *clusterArray;
        double score,scorediff;
        int k;
    public:
        KMeans(int k,std::istream &is);
        double computeClusteringScore();
        void iterateOnce();
    };
}

#endif//CLUSTERING_KMEANS_H