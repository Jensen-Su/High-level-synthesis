// ===== -------------------------------------------------------------------- ==== //
// ===== Copyright @sjcshindou 2015.07.21                                     ==== //
// ===== -------------------------------------------------------------------- ==== //
// ===== Jason's GMP memory partition algorithm 
// ===== Theory and Algorithm for Generalized Memory Partition in High-level Synthesis
// ===== -------------------------------------------------------------------- ==== //

#ifndef _GMP_PARTITION_H
#define _GMP_PARTITION_H

#include "partition.h"
#include <vector>
using std::vector;

template<int dim>
struct Solution{
	Matrix<int, 1, dim> _alpha;
	int _nbanks;
	Solution(const Matrix<int, 1, dim> &alp, int nb) :_alpha(alp), _nbanks(nb){}
    void print();
};

template<int dim>
void Solution<dim>::print()
{
    std::cout << "alpha:\t";
    _alpha.printMatrix();
    std::cout << "Patition factor: \t";
    std::cout << _nbanks << "\n";
        
}

template<int dim, int loop > 
class GMPPartition: public Partition<dim, loop>
{
public:
    using Partition<dim, loop>::_refs;
    using Partition<dim, loop>::_nbanks;
    using Partition<dim, loop>::_ports;

	template<int r, int c> using matrix = int[r][c];
    GMPPartition() = default;
    template< int r, int c> GMPPartition( const matrix<r, c> &patt)
        :Partition<r, c>::Partition(patt){}

private: 
	void addSolution(const Solution<dim> & s) { _solutions.push_back(s); }    
	bool isConflict2Refs(const Matrix<int, 1, dim> &alpha, int nbanks);
	bool isConflict3Refs(const Matrix<int, 1, dim> &alpha, int nbanks);
	void partition2dmemory();
    void partition3dmemory();

public:
    void partition();   
	/* void calculatePadding(); */
    void printSolutions();

private:
   int  _padding;
	vector< Solution<dim> > _solutions;
};

template<int dim, int loop>
bool GMPPartition<dim, loop>::isConflict2Refs(const Matrix<int, 1, dim> &alpha, int nbanks)
{
    // for each pair of references, they must not conflict
    for (int i = 0; i < _refs.size(); i++)
    {
        for (int j = i + 1; j < _refs.size(); j++)
        {
            // construct its polytope and detect if they conflict each other
            Polytope<dim, loop> polytope(_refs[i], _refs[j]);
            if (polytope.isConflict(alpha, nbanks)) return true; // if conflict
        }
    }
    return false;
}

// for single port memory

// for 2-port memory.
template<int dim, int loop>
bool GMPPartition<dim, loop>::isConflict3Refs(const Matrix<int, 1, dim> &alpha, int nbanks)
{
    // for each three references, one must not conflict with both the others
    for (int i = 0; i < _refs.size(); i++)
    {
        for (int j = i + 1; j < _refs.size(); j++)
        {
            for (int r = j + 1; r < _refs.size(); r++)
            {
                // construct a polytope for these three references, detecting if conflict
                Polytope<dim, loop> polytope(_refs[i], _refs[j]);
                polytope.setThirdReference(&_refs[r]);
                if (polytope.isConflict(alpha, nbanks)) return true; // if conflict
            }
        }
    }
    return false;
}

template<int dim, int loop>
void GMPPartition<dim, loop>::partition2dmemory()
{
    // initialize
    _nbanks = _refs.size();
    Matrix<int, 1, 2> alpha;
    alpha(0, 0) = 1;
    alpha(0, 1) = 1;

    bool conflict = true;
    for (_nbanks;; _nbanks++)
    {
        // for each point in _nbanks*_nbanks space
        for (alpha(0,0) = 1; alpha(0,0) <= _nbanks; alpha(0,0) ++)
        {
            for (alpha(0,1) = 1; alpha(0,1) <= _nbanks; alpha(0,1) ++)
            {
                if(_ports == 1)
                    conflict = isConflict2Refs(alpha, _nbanks);
                else if(_ports == 2)
                    conflict = isConflict3Refs(alpha, _nbanks);

                if (conflict) continue;

                _solutions.push_back(Solution<2>(alpha, _nbanks));
            }
        }
        if (_solutions.size())break;
    }
}

template<int dim, int loop>
void GMPPartition<dim, loop>::partition3dmemory()
{
    std::cerr << "partition for 3-d memory is not implemented in function: \n";
    std::cerr << "\t'template<in dim, int loop> void GMPPartition<dim,loop>::partition3dmemory()';\n";
}

template<int dim, int loop>
void GMPPartition<dim, loop>::partition()
{
    switch(dim)
    {
        case 2:
            partition2dmemory();
            break;
        case 3:
            partition3dmemory();
            break;
        default:
            std::cerr << "partition not implemented in 'void GMPPartition<dim, loop>::partition()'!\n";
            break;
    }
}

/* template<int dim, int loop> */
/* void GMPPartition<dim, loop>::calculatePadding() */
/* { */
/* 	double t = _w[dim - 1] / _nbanks; */
/* 	_padding = (int)t; */
/* 	if ((double)_padding != t) _padding++; */
/* } */

template<int dim, int loop>
void GMPPartition<dim, loop>::printSolutions()
{
    for(auto solution: _solutions)
    {
        solution.print();
        std::cout << "\n";
    }
}




#endif
