// Karl Yerkes / 2020-02-20
//
// Example of using mlpack to do NN search of random data
//
// build like this:
//
//   c++ -std=c++11 use-mlpack.cpp -lmlpack -larmadillo
//
#include <iostream>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

typedef mlpack::neighbor::NeighborSearch<   //
    mlpack::neighbor::NearestNeighborSort,  //
    mlpack::metric::EuclideanDistance,      //
    arma::mat,                              //
    mlpack::tree::BallTree>                 //
    MyKNN;

// tree types:
// mlpack::tree::BallTree
// mlpack::tree::KDTree
// mlpack::tree::RStarTree
// mlpack::tree::RTree
// mlpack::tree::StandardCoverTree

int main() {
  // make up some random data
  //
  arma::mat dataset(3, 1000, arma::fill::randu);

  // make up some data not in the corpus above
  //
  arma::mat query(3, 50, arma::fill::randu);

  // empty; filled in by the search
  //
  arma::mat distances;
  arma::Mat<size_t> neighbors;

  // execute the search
  //
  MyKNN myknn(dataset);
  myknn.Search(query, 1, neighbors, distances);

  //
  for (size_t i = 0; i < neighbors.n_elem; ++i) {
    std::cout << neighbors[i] << " " << distances[i] << std::endl;
  }
}

