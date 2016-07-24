#ifndef _CHENYUE_PARTITION_H
#define _CHENYUE_PARTITION_H

#include "partition.h"
#include <vector>
#include <set>
#include <stdlib.h>
#include <iostream>

using std::set;
using std::vector;
template<int dim, int loop>
class ChenyuePartition: public Partition<dim, loop>
{
public:
    using Partition<dim, loop>::_refs;
//    using Partition<dim, loop>::_nbanks;
//    using Partition<dim, loop>::_ports;

	template<int r, int c> using matrix = int[r][c];
    ChenyuePartition():_Nf(0),_Nmax(0), _Nc(0){}
    template< int r, int c> ChenyuePartition( const matrix<r, c> &patt)
        :Partition<r, c>::Partition(patt){}

private:
    void determinAlpha();
    void minNf();
    void determinNc();
    

public:
    int getPartitionFactor()  
    {
        if( _Nc == 0 )
            determinNc();
        return _Nc;
    }
    void setNmax( int n ) { _Nmax = n; }
    void print();  
    void partition();

private:
    Matrix<int, 1, dim>  _alpha;
    int _Nf;
    int _Nmax;
    int _Nc;
};
 
template<int dim, int loop>
void ChenyuePartition<dim, loop>::print()
{
   std::cout << "alpha:\t";
   _alpha.printMatrix();
   std::cout << "Nf:\t" << _Nf << "\n";
   std::cout << "Nc:\t" << _Nc << "\n";
   
}

template<int dim, int loop>
void ChenyuePartition<dim, loop>::partition()
{
   determinAlpha();
   minNf();
   _Nmax = 10;
   determinNc();
   print();
}


template<int dim, int loop>
void ChenyuePartition<dim, loop>::determinAlpha()
{
    int D[dim];
    int minx[dim];
    int maxx[dim];
    for(int i = 0; i < dim; i++)
    {
        minx[i] = 1000; // initialize with a big nunmber
        maxx[i] = 0;
    }

    for(auto &ref : _refs)
    {
        for(int i = 0; i < dim; i++)
        {
            if(minx[i] > ref.getC()(i,0))
                minx[i] = ref.getC()(i,0);
            if(maxx[i] < ref.getC()(i,0))
                maxx[i] = ref.getC()(i,0);
        }
    }

    for(int i = 0; i < dim; i++) D[i] = maxx[i] - minx[i] + 1;

    for(int i = 0; i < dim - 1; i++)
    {
        _alpha(0,i) = 1;
        for( int j = i + 1; j < dim ; j++)
            _alpha(0,i) *=  D[j];
    }
    _alpha(0, dim-1) = 1;

    std::cout << "D:\t";
    for(int i = 0; i < dim; i++)
        std::cout << D[i] << "\t";
    std::cout << "\n";
}

template<int dim, int loop>
void ChenyuePartition<dim, loop>::minNf()
{
    int m = _refs.size();

    int z[m];
    int t = 0;
    Matrix<int, 1, 1> z_t;

    for(auto &ref : _refs)
    {
        z_t = _alpha * ref.getC();
        z[t] = z_t(0,0);
        t++;
    }

    set<int> Q;
    int M = 0;
    int absz;
    for(int i = 0; i < m - 1; i++)
    {
        for(int j = i + 1; j < m; j++)
        {
            absz = abs(z[i]-z[j]);
            if(absz > M) M = absz;
            Q.insert(absz);
        }
    }

    int E[M];
    for(int i = 0; i < M; i++) E[i] = 0;

    for(auto el : Q) E[el]++;
    
    _Nf = m; 
    int k = 1;
    while(k * _Nf <= M)
    {
        if(E[k * _Nf] != 0)
        {
            _Nf++;
            k = 1;
        }
        else
            k++;
    }
}

template<int dim, int loop>
void ChenyuePartition<dim,loop>::determinNc()
{
    if(_Nf == 0)
    {
        std::cerr << "_Nf not determined yet, you need to call ChenyuePartition<dim, loop>::minNf()!\n";
        exit(0);
    }

     if(_Nf <= _Nmax || _Nmax == 0)
    {
        _Nc = _Nf;
        return;
    }

    int m = _refs.size();
    int B[m];
    int minII = _Nmax;
    for(int n = 1; n <= _Nmax; n++)
    {
        cout << "\n";
        int k = 0;
        for(auto &ref : _refs)
        {
            // construct Bp|n = { B(x0), ..., B(xm-1) }
            // where B(x0) = (alpha * x0) % n ...
            B[k] = (_alpha * ref.getC())(0, 0);
            B[k] %= n;
            std::cout << "B[" <<  k << "] = " << B[k] << "\t";
            k++;
        }

        std::cout << "\n";

        // count the nunmber of occurences of each pattern in B[]
        int mark[n];
        for(int i = 0; i < n; i++) mark[i] = 0;
        for(int i = 0; i < m; i++)
        {
            mark[B[i]]++;
        }

        // find the max nunmber of initial interval
        int II = 0;
        for(int i = 0; i < n; i++)
        {
            if(mark[i] > II) II = mark[i];
            cout << "mark[" << i << "] = " << mark[i] << "\t";
        }
        cout << "II = " << II << "\n";
        // if the max number is 1(which is seem to be impossible when _Nmax < _Nf)
        // or 2, then terminate
        if(II == 1 || II == 2) 
        {
            _Nc = n;
            return;
        }
        if( II < minII ) 
        {
            minII = II;
            _Nc = n;
        }
    }

}

#endif
