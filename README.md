count-matrix-transformer
===================

This tool takes a raw count matrix and compute relative gene abundance according to the method exposed in [Qin, J. et al. A metagenome-wide association study of gut microbiota in type 2 diabetes. Nature](http://www.nature.com/nature/journal/v490/n7418/full/nature11450.html)

The reverse operation consisting of computing a raw matrix from a normalized count matrix can also be performed.

Requirements
-------------
- A C++ compiler
- [Boost C++ Libraries](http://www.boost.org)
 
Usage
-------------
````
./count-matrix-transformer -c <input_count_matrix> -l <genes_length_file> -t <transformation_type> -o <transformed_count_matrix> 
````

* *-c  \<input_count_matrix\>*  count matrix to transform
* *-l \<genes_length_file\>* file (without an header) which contains line by line, tab separated pairs of values '\<gene name\> \<gene length\>'
* *-t \<transformation_type\>* transformation to apply to the count matrix ('normalize' or 'denormalize')
* *-o \<transformed_count_matrix\>*  output transformed count matrix

Input count matrix example
-------------
````
gene_id sample_1 sample_2 sample 3
gene_1 10 0 3
gene_2 0 21 8
gene_3 12 9 3
gene_4 10 12 11
````

genes length file example
-------------
````
gene_1 699
gene_2 135
gene_3 1422
gene_4 1827
gene_5 540
gene_6 61
````
