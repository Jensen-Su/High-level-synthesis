// ===== -------------------------------------------------------------------- ==== //
// ===== Copyright @sjcshindou 2015.07.21                                     ==== //
// ===== -------------------------------------------------------------------- ==== //


#ifndef _PARTITION_H
#define _PARTITION_H

#include "ConflictDetection.h"
#include "MyMatrix.h"
#include "myGLPK.h"
#include <iostream>
#include <vector>
using std::vector;

template<int dim, int loop>
class Partition{
protected:
	vector< Reference<dim, loop> > _refs;
	int _nbanks; // partition factor
	int _w[dim];
    int _ports;
public:
	template<int r, int c> using matrix = int[r][c];

	Partition();
	template<int r, int c> Partition(const matrix<r, c> & patt);
		
protected:
	void addReference(const Reference<dim, loop> & ref) { _refs.push_back(ref); }

public:	
	template<int r, int c> void  readPattern(const matrix<r, c> & patt);
	void printReferences();
    
public:
    virtual void partition() = 0;

private:
	int A[dim][loop];
};

template<int dim, int loop>
Partition<dim, loop>::Partition()
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < loop; j++)
        {
            if (i == j) A[i][j] = 1;
            else A[i][j] = 0;
        }
    }

    _ports = 1;
}
template<int dim, int loop>
template<int r, int c> 
Partition<dim, loop>::Partition(const matrix<r, c> & patt)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < loop; j++)
        {
            if (i == j) A[i][j] = 1;
            else A[i][j] = 0;
        }
    }
    readPattern(patt);
}

template<int dim, int loop>
template<int r, int c> 
void  Partition<dim, loop>::readPattern(const matrix<r, c> & patt)
{
    _refs.clear();
    int C[dim][1];
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (patt[i][j])
            {
                C[0][0] = i;
                C[1][0] = j;
                addReference(Reference<dim, loop>(A, C));
            }
        }
    }
}

template<int dim, int loop>
void Partition<dim, loop>::printReferences()
{
    for (auto ref : _refs)
    {
        ref.print();
        std::cout << "\n";
    }
}

#endif
