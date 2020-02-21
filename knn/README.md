# [mlpack][]

- [Github][]
- [Method-specific Tutorials][]

[mlpack]: https://www.mlpack.org
[Github]: https://github.com/mlpack/mlpack
[Method-specific Tutorials]: https://www.mlpack.org/doc/mlpack-3.2.2/cli_documentation.html#method-specific-tutorials


## Install


    brew install mlpack


## [Nearest-neighbors search][]

[Nearest-neighbors search]: https://www.mlpack.org/doc/mlpack-3.2.2/doxygen/nstutorial.html



### Using the CPI tool

First, create some data and some queries that are not in the data:


    perl -le "print join ',',rand,rand,rand for 1..1000" > reference-dataset.csv
    perl -le "print join ',',rand,rand,rand for 1..50" > query-dataset.csv


Now, execute the queries:


    mlpack_knn -q query-dataset.csv -r reference-dataset.csv -n neighbors.csv -d distances.csv -k 5 -v
    [INFO ] Loading 'reference-dataset.csv' as CSV data.  Size is 3 x 1000.
    [INFO ] Loaded reference data from 'reference-dataset.csv' (3 x 1000).
    [INFO ] Building reference tree...
    [INFO ] Tree built.
    [INFO ] Loading 'query-dataset.csv' as CSV data.  Size is 3 x 50.
    [INFO ] Loaded query data from 'query-dataset.csv' (3x50).
    [INFO ] Searching for 5 neighbors with dual-tree kd-tree search...
    [INFO ] 3097 node combinations were scored.
    [INFO ] 7674 base cases were calculated.
    [INFO ] 3097 node combinations were scored.
    [INFO ] 7674 base cases were calculated.
    [INFO ] Search complete.
    [INFO ] Saving CSV data to 'd.csv'.
    [INFO ] Saving CSV data to 'n.csv'.
    [INFO ]
    [INFO ] Execution parameters:
    [INFO ]   algorithm: dual_tree
    [INFO ]   distances_file: d.csv
    [INFO ]   epsilon: 0
    [INFO ]   help: 0
    [INFO ]   info:
    [INFO ]   input_model_file:
    [INFO ]   k: 5
    [INFO ]   leaf_size: 20
    [INFO ]   neighbors_file: n.csv
    [INFO ]   output_model_file:
    [INFO ]   query_file: query-dataset.csv
    [INFO ]   random_basis: 0
    [INFO ]   reference_file: reference-dataset.csv
    [INFO ]   rho: 0.7
    [INFO ]   seed: 0
    [INFO ]   tau: 0
    [INFO ]   tree_type: kd
    [INFO ]   true_distances_file:
    [INFO ]   true_neighbors_file:
    [INFO ]   verbose: 1
    [INFO ]   version: 0
    [INFO ] Program timers:
    [INFO ]   computing_neighbors: 0.000695s
    [INFO ]   loading_data: 0.002163s
    [INFO ]   saving_data: 0.000650s
    [INFO ]   total_time: 0.003345s
    [INFO ]   tree_building: 0.000193


## c++ example

See `use-mlpack.cpp` in this folder.
