#ifndef _CODE_TRANSFORMATION_H_
#define _CODE_TRANSFORMATION_H_

#include "MyMatrix.h"
#include "ConflictDetection.h"
#include <iostream>
#include <vector>
#include <list>
using std::list;
using std::vector;
using std::cout;
using std::ofstream;

template<int row, int col> 
class CodeTrans
{
private:
   list< Reference<2, 2> > _refs;
   const int A[2][2] = {{1,0}, {0,1}};
    Matrix<int, 1, 2> _alpha; 
    int _nbanks; 

public:
    template<int r, int c> using matrix = int[r][c];

	template<int r, int c> CodeTrans(const matrix<r, c> & patt);

    template<int r, int c> void readPattern(const matrix<r, c> &patt);

    void initialize(const Matrix<int, 1, 2> &alpha, int n)
    {
        _alpha = alpha;
        _nbanks = n;
    }
    void addReference(const Reference<2, 2> &ref){ _refs.push_back(ref); }
    int genBankIndex(ofstream &outf, Reference<2, 2> ref, int m)
    {
        outf << "BankIndex" << m << " = ";

        int t = (_alpha * ref.getC())(0, 0);
        outf << "(" << _alpha(0, 0) << " * i + " << _alpha(0, 1) << " * j";
        outf << " + " << t << ") % " << _nbanks << ";\n";
    }

    int genBankOffset(ofstream &outf, Reference<2, 2> ref, int m)
    {
        outf << "BankOffset" << m << " = ";
        outf << (int)(col/_nbanks) << " * ( i + " << ref.getC()(0, 0);
        outf << " ) + j;\n";
    }

    void genAddress(ofstream &outf)
    {
        outf << "for(int i = 0; i < " << row << "; i++)\n";
        outf << "{\n";
        outf << "\tfor(int j = 0; j < " << col << "; j++)\n";
        outf << "\t{\n";

        int i = 0;
        for( auto & ref : _refs )
        {
            outf << "\t\t";
            genBankIndex(outf, ref, i);
            outf << "\t\t";
            genBankOffset(outf, ref, i);
            outf << "\n";
            i++;
        }

        outf << "\t}\n";
        outf << "}\n";
    }
};

template<int row, int col>
template<int r, int c>
CodeTrans<row, col>::CodeTrans(const matrix<r, c> &patt)
{
    readPattern(patt);
}

template<int row, int col>
template<int r, int c>
void CodeTrans<row, col>::readPattern(const matrix<r, c> &patt)
{
     _refs.clear();
    int C[2][1];
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (patt[i][j])
            {
                C[0][0] = i;
                C[1][0] = j;
                addReference(Reference<2, 2>(A, C));
            }
        }
    }

}


#endif 
