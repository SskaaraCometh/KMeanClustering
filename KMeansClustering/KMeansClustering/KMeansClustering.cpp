// KMeansClustering.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <valarray>
#include <iterator>
#include <cmath>


class KMeansClustering
{
public:
	KMeansClustering() {};
	~KMeansClustering() {};

	KMeansClustering(int k, std::vector<std::pair< double, double>> & data_) : m_k(k), m_features(k), m_data(k)
	{
		m_data[0] = data_; //this just assigns the first label to all data
	}

	void ClusterData(std::valarray<std::pair< double, double>> & init_centroids, int num_iters)
	{
		m_features = init_centroids;
		this->AssignLables();

		int i = 0;
		while (i < num_iters)
		{
			std::cout << "Running Iterator " << i << "\r\n";
			this->ComputeMeans();
			this->AssignLables();
			i++;
		}
	}

	void PrintClusters() const
	{
		for (int k = 0; k < m_k; k++)
		{
			std::cout << "Cluster: " << k << "\r\n";
			for (auto const & feature : m_data[k])
			{
				std::cout << " [" << std::get<0>(feature) << "," << std::get<1>(feature) << "] ";
			}
			std::cout << "\r\n";
		}
	}

private:
	void AssignLables()
	{
		std::valarray<std::vector<std::pair< double, double>>> new_data(m_k);
	
		//loop over array of clusters 
		for (auto const & clusters : m_data)
		{
			for (auto const & features : clusters)
			{
				//create container for closest centroid 
				int closestmean = this->ComputeClosestCentroid(features);
				//push the data back using features and closest mean
				new_data[closestmean].push_back(features);
			}
		}
		//set pushed back data to member data
		m_data = new_data;
	}

	bool ComputeMeans()
	{
		bool res = true;
		//for each cluster/class, loop through data members;
		for (int i = 0; i < m_k; i++)
		{
			//create variable mean for means
			std::pair<double,double> mean(0,0);

			//access length of the vector of valarray container and store it
			int feature_size = m_data[i].size();
			//iterate through data and add them together, increment data member/sum them up
			for (auto const & it : m_data[i])
			{
				//increment
				std::get<0>(mean) += std::get<0>(it);
				std::get<1>(mean) += std::get<1>(it);
			}

			//divide mean by the number of features to get the average 
			std::get<0>(mean) /= feature_size;
			std::get<1>(mean) /= feature_size;

			//check if mean and res are both true, check for convergence
			res = (m_features[i] == mean && res == true) ? true : false;

			//set current mean to the overall mean
			m_features[i] = mean;

			std::cout << "Cluster Centroid " << i << ": " << std::get<0>(mean) << " , " << std::get<1>(mean) << "\t\n";
		}

		return res;
	}

	int ComputeClosestCentroid(const std::pair<double, double> & point_)
	{
		//create valarray container for distances and set to cluster number
		std::valarray<double> distances(m_k);

		//loop through clusters and 
		for (int i = 0; i < m_k; i++)
		{
			//square the distance in x and y before doing the sqrt (pythagoras) 
			double del_x = std::get<0>(point_) - std::get<0>(m_features[i]);
			double del_y = std::get<1>(point_) - std::get<1>(m_features[i]);
			double dist = sqrt((del_x * del_x) + (del_y * del_y));
			distances[i] = dist;
		}

		auto closest_mean = std::distance(std::begin(distances), std::min_element(begin(distances), std::end(distances)));
		return closest_mean;

	}

	int m_k;
	std::valarray<std::pair<double, double>> m_features;
	std::valarray<std::vector<std::pair<double, double>>> m_data;
};

int main()
{
	//std::vector<std::pair<double, double>> data =
	//{ { 53.445107, -2.252369 },{ 53.443698, -2.254933 },{ 53.443860, -2.248530 },{ 53.462558, -2.253039 },{ 53.460236, -2.253401 },
	//{ 53.460988, -2.249157 } };


	//std::valarray<std::pair<double, double>> init_centroids = { { 53.462878, -2.245696 },{ 53.443948, -2.256569 } };

    return 0;
}

