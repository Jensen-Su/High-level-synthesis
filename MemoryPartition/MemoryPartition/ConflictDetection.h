// ==== -------------------------------------------------------------------- ==== //
// ==== Copyright @ sjcshindou 2015.07.20           ------------------------ ==== //
// ==== Construct polytope for each pair or three references                 ==== //
// ==== Detect if the polytope is empty                                      ==== //
// ==== -------------------------------------------------------------------- ==== //


#ifndef _CONFLICT_DETECTION_H
#define _CONFLICT_DETECTION_H
#include <string>
#include "MyMatrix.h"
#include <list>
#include <iostream>
#include "LPFormat.h"
#include <glpk.h>
#include "myGLPK.h"
#include <stdio.h>
#include <stdlib.h>
using std::list;

/// A Reference to a dim-d memory is represented as:
//  Ref(x) = A * i + C
//  For instance, Ref(x) = (i0 + 2, i1 + 1, i2 + 0), then:
//       | 1 0 0 |       | i0 |       | 2 |
//  A =  | 0 1 0 |,  i = | i1 |,  C = | 1 |   
//       | 0 0 1 |       | i2 |       | 0 |
//  Where i is implicit such that Reference only has member of A and C
///
template<int dim, int nloops>
class Reference{
	Matrix<int, dim, nloops> _matA;
	Matrix<int, dim, 1> _matC;
public:
	Reference() {}
	~Reference(){}
	template< int r, int c > using matrix_t = int[r][c];
	template< int r > using array_t = int[dim][1];
	Reference(const matrix_t<dim, nloops> &mat, const array_t<dim> & mc):_matA(mat), _matC(mc){}	
	Reference(const Matrix<int, dim, nloops> &mat, const array_t<dim> &mc) : _matA(mat) , _matC(mc){}
	Reference(const Matrix<int, dim, nloops> &mat, const Matrix<int, dim, 1> &mc) : _matA(mat), _matC(mc){}
	Reference(const matrix_t<dim, nloops> &mat, const Matrix<int, dim, 1> &mc) : _matA(mat), _matC(mc){}
	Reference(const Reference<dim, nloops> & ref){ *this = ref; }

	void setA(const matrix_t<dim, nloops> &mat){ _matA = mat; }
	void setA(const Matrix<int, dim, nloops> &mat){ _matA = mat; }
	void setC(const matrix_t<dim, nloops> &mat){ _matC = mat; }
	void setC(const Matrix<int, dim, nloops> &mat){ _matC = mat; }
	Matrix<int, dim, nloops> &getA(){ return _matA; }
	Matrix<int, dim, 1> &getC(){ return _matC; }
	const Matrix<int, dim, nloops> &getA()const { return _matA; }
	const Matrix<int, dim, 1> &getC()const { return _matC; }

	Reference<dim, nloops> &operator=(const Reference<dim, nloops> & ref)
	{
		if (this == &ref) return *this;
		_matA = ref._matA;
		_matC = ref._matC;
		return *this;
	}
	Reference<dim, nloops> operator+(const Reference<dim, nloops> &ref)
	{
		return Reference(_matA + ref._matA, _matC + ref._matC);
	}
	Reference<dim, nloops> operator-(const Reference<dim, nloops> &ref)
	{
		return Reference(_matA - ref._matA, _matC - ref._matC);
	}
	friend Reference<1, nloops> operator*(const matrix_t<1,dim> &alp, const Reference<dim,nloops> &ref)
	{
		return Reference<1,nloops>(alp * ref._matA, alp * ref._matC);
	}
	friend Reference<1, nloops> operator*(const Matrix<int,1, dim> &alp, const Reference<dim, nloops> &ref)
	{
		return Reference<1, nloops>( (alp * ref._matA), (alp * ref._matC));
	}

	Reference<dim, nloops> operator*(const Reference<dim, nloops> &ref) = delete;
	Reference<dim, nloops> operator/(const Reference<dim, nloops> &ref) = delete;
	void print(){
		for (int r = 0; r < dim; r++)
		{
			for (int c = 0; c < nloops; c++)
			{
				std::cout << _matA(r, c) << "\t";
			}
			std::cout << "\t\t" << _matC(r,0) << "\n";
		}
	}
};

/// Polytope 
/// A polytope is consist of two References or there or more with:
//  _ref1 Mod N = _ref2 Mod N or
//  _ref1 Mod N = _ref2 Mod N && _ref2 Mod N = _ref3 Mod N
//  or the like
///  
template<int dim, int nloops>
class Polytope{
private:
	Reference<dim, nloops> _ref1,_ref2; //  memory references to build a polytope
	Reference<dim, nloops> *_ref3;
	int _upBound[nloops];
//	Matrix<int, 1, dim> _alpha; // partion vector
//	int _nbanks; // partition factor
private:
    glp_prob *lp;
	LPFormula<nloops> _formula1;
	LPFormula<nloops>* _formula2;
	
//	int _nports; // memory port number
//	int _bsize;  // partition block size
	
	void configFormulas(const Matrix<int,1,dim> &alpha, int nbanks);
    void dumpPolytope(std::ofstream &outf);

public:
	Polytope(const Reference<dim, nloops> &r1, 
		const Reference<dim, nloops> &r2,
		 Reference<dim,nloops> *r3 = nullptr)
		:_ref1(r1),_ref2(r2), _ref3(r3), _formula2(nullptr) {}
	
	~Polytope(){ if (_formula2) delete _formula2; }

	void setThirdReference(Reference<dim, nloops> *ref){ _ref3 = ref; }

	virtual bool isConflict(const Matrix<int, 1, dim> &alpha, int nbanks);

	void dumpPolytope(const string &filename);
};


template<int dim, int nloops>
void Polytope<dim, nloops>::configFormulas(const Matrix<int,1,dim> &alpha, int nbanks){
    Reference<1, nloops> ref(alpha*(_ref1 - _ref2));
    const Matrix<int, 1, nloops> &matrixA = ref.getA();
    const Matrix<int, 1, 1> &matrixC = ref.getC();

    _formula1.addItem(matrixA, "i");
    _formula1.addItem(nbanks, "k0");
    _formula1.setConstant(matrixC(0, 0));
    _formula1.setObject(" = ");

    if (_ref3 == nullptr) return;

    ref = alpha*(_ref2 - *_ref3);
    const Matrix<int, 1, nloops> &matrixA2 = ref.getA();
    const Matrix<int, 1, 1> &matrixC2 = ref.getC();

    _formula2 = new LPFormula<nloops>();
    _formula2->addItem(matrixA2, "i");
    _formula2->addItem(nbanks, "k1");
    _formula2->setConstant(matrixC2(0, 0));
    _formula2->setObject(" = ");
}

template<int dim, int nloops>
void Polytope<dim, nloops>::dumpPolytope(std::ofstream &outf){	
    outf << "\nMinimize\n";
    for (int i = 0; i < nloops; i++)
    {
        outf << "i" << i << "  +  ";
    }
    outf << " k0  +  k1 \n";

    outf << "\nSubject To\n";
    _formula1.dumpConstraintFile(outf);
    if (_formula2)
        _formula2->dumpConstraintFile(outf);

    outf << "\nBound \n";
    for (int i = 0; i < nloops; i++)
    {
        outf << "0 <= i" << i << "  <=  10000\n";
    }
    outf << "-100 <= k0 <= 100\n";
    if(_formula2)
        outf << "-100 <= k0 <= 100\n";

    outf << "\nInteger \n";
    for (int i = 0; i < nloops; i++){
        outf << "i" << i << "\n";
    }
    outf << "k0\n";
    if (_formula2)
        outf << "k1\n";

    outf << "\nEnd\n";
}

template<int dim, int nloops>
bool Polytope<dim, nloops>::isConflict(const Matrix<int, 1, dim> &alpha, int nbanks){
    // configure the formula
    configFormulas(alpha, nbanks);
    std::string filename = "ILPPolytope.txt";
    dumpPolytope(filename);

    /// next is proccess to sovle the ILP problem //
    glp_smcp smcp;
    glp_iocp iocp;
    glpkCall* glpkObj = new glpkCall("/usr/local/lib/libglpk.so");	

    // initial
    lp = glpkObj->glp_create_prob();
    glpkObj->glp_set_prob_name(lp, "Polytope");
    glpkObj->glp_init_smcp(&smcp);
    glpkObj->glp_init_iocp(&iocp);

    // Solving ILP problem
    if(!(glpkObj->glp_read_lp( lp, NULL, filename.c_str() ) ) ) // read in cplex lp file
    {
        // Optimal basis to initial LP relaxation must be provided
        glpkObj->glp_simplex( lp, &smcp );
        // Use branch and bound method to find optimal solution base on optimal basis
        glpkObj->glp_intopt( lp, &iocp );
    }
    else
    {
        std::cerr << "GLPK tool cannot read in lp problem file: " << filename << "\n";
        std::cerr << "error occurs in function:\n";
        std::cerr << "template<int dim, in nloops> virtual bool polytope<dim, nloops>::isConflict( const Matrix<int, 1, dim>&, int)\n";
        exit(0);
    }

    // check if feasible
    int mip_status = glpkObj->glp_mip_status(lp);
    // if feasible, conflict
    // if infeasible, not conflict
    return  (mip_status != GLP_NOFEAS);
}

template<int dim, int nloops>
void Polytope<dim, nloops>::dumpPolytope(const string &filename){
    std::ofstream outf(filename);
    if (!outf){
        std::cerr << "Can not open file: " << filename << "\n";
        exit(0);
    }
    dumpPolytope(outf);
    outf.close();
}

#endif
