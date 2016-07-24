// ===== -------------------------------------------------------------------------- ===== //
// ===== Copyright@sjcshindou 2015-07-12 ~ 14                                       ===== //
// ===== -------------------------------------------------------------------------- ===== //

#ifndef _LPFORMAT_H
#define _LPFORMAT_H

#include "MyMatrix.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using std::string;
using std::ostringstream;
using std::ofstream;
using std::vector;
using std::list;
using std::cout;
struct Item{
	int _factor;
	string _symbol;
};
class SimpleFormula{
	
};
template<int loops>
class LPFormula{
	std::list<Matrix<int, 1, loops> > _matrixs;
	std::list<string> _syms_mat;
	std::list<int> _factors;
	std::list<string> _syms;
	string _obj;
	int _constant;
	ostringstream oss;
	void buildFormula();
public:
	LPFormula(const string &obj = " = ") :_obj(obj) {} // This, however, cannot be omitted
//	LPFormula(const LPFormula<col> &) = delete;
	LPFormula<loops> &operator= (const LPFormula<loops> &) = delete;
	vector<string> &getSyms(){}
	void addItem(const Matrix<int, 1, loops> & mat, const string & sym)
	{
		_matrixs.push_back(mat);
		_syms_mat.push_back(sym);
	}
	void addItem(int a, const string &sym){
		_factors.push_back(a);
		_syms.push_back(sym);
	}
	void setConstant(int c){ _constant = c; }
	void setObject(const string &str = " = "){
		_obj = str;
	}
	void clear(){
		_matrixs.clear();
		_syms_mat.clear();
		_factors.clear();
		_syms.clear();
		oss.str("");
		oss.clear();
	}
	void dumpConstraintFile(ofstream &outf){
		buildFormula();
		outf << oss.str();
	}
	void print(){
		buildFormula();
		cout << oss.str();
	}
};

template<int loops>
void LPFormula<loops>::buildFormula(){
	int r = 0;
	auto symbol = _syms_mat.begin();

	for (auto mat : _matrixs)
	{
		for (int c = 0; c < loops; c++)
        {
            if(mat(0, c) > 0) oss << " + ";
            else if(mat(0,c) == 0) continue;
			oss << " " << mat(0, c) << " " << *symbol << c;
		}
		symbol++;
		r++;
		//oss << "\n";
	}
	r = 0;

	symbol = _syms.begin();
	for (auto a : _factors)
	{		
        if(a > 0) oss << " + ";
        else if( a == 0 ) continue;
		oss << a << " " << *symbol;
		symbol++;
    }
	oss << "  ";
	oss << _obj << " " << -_constant << "\n";
}

#endif
