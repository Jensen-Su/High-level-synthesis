// ===== -------------------------------------------------------------------------- ===== //
// ===== A Simple Matrix Class                                                      ===== //
// ===== Copyright@sjcshindou 2015-07-12 ~ 14                                       ===== //
// ===== -------------------------------------------------------------------------- ===== //
// ===== Just implementing simple operations(+/+=, -/-= and */*=(scalar)            ===== //
// ===== -------------------------------------------------------------------------- ===== //

#ifndef _MYMATRIX_H
#define _MYMATRIX_H
#include <iostream>
#include<memory>

template <class InfoTy, int nrows, int ncols> 
class Matrix{
private:
	 InfoTy* _matrix;
	 int _size;

public:
	//typedef Matrix<InfoTy, nrows, ncols> theMatrix;
	
	using theMatrix = Matrix < InfoTy, nrows, ncols >;
//	template< int nrows2, int ncols2 > using matrix_t = InfoTy[nrows2][ncols2];
	template< int r, int c > using matrix_t = InfoTy[r][c];

	Matrix();
	Matrix(const matrix_t<nrows, ncols> &mat);	// initialize with a matrix
	Matrix(const theMatrix & rhs);   // initialize with a Matrix object
	Matrix(const InfoTy* m, int s);  // initiate with an array and its size

	~Matrix(){delete []_matrix;}
	theMatrix & operator=(const theMatrix &m);

	theMatrix operator+(const theMatrix &rhs);
	theMatrix operator+(const theMatrix &rhs)const;
	theMatrix &operator+=(const theMatrix &rhs);

	theMatrix operator-(const theMatrix &rhs);
	theMatrix operator-(const theMatrix &rhs)const;
	theMatrix &operator-=(const theMatrix &rhs);

	theMatrix operator*(const InfoTy &rhs);
	theMatrix operator*(const InfoTy &rhs)const;
	theMatrix &operator*=(const InfoTy &rhs);

    bool operator==(const theMatrix &mat)
    {
        for(int i = 0; i < _size; i++)
            if(_matrix[i] != mat._matrix[i])
                return false;
        return true;
    }

    bool operator==(const theMatrix &mat)const 
    {
        for(int i = 0; i < _size; i++)
            if(_matrix[i] != mat._matrix[i])
                return false;
        return true;
    }


    bool operator==(const matrix_t<nrows, ncols> &mat)
    {
       // implemente when needed
       return false;
    }
    bool operator==(const matrix_t<nrows, ncols> &mat)const
    {
       // implemente when needed
       return false;
    }

	InfoTy &operator()(int row, int col);
    const InfoTy &operator()(int row, int col) const; 

	template< int ncols2 > Matrix<InfoTy, nrows, ncols2> operator*(const matrix_t<ncols, ncols2> &mat);
	template< int ncols2 > Matrix<InfoTy, nrows, ncols2> operator*(const Matrix<InfoTy, ncols, ncols2> &mat);
	template< int ncols2 > Matrix<InfoTy, nrows, ncols2> operator*(const Matrix<InfoTy, ncols, ncols2> &mat)const;
	// friend function using template parameter(s) of its belonging class could not define outside.
	template< int nrows2 > 
	friend Matrix<InfoTy, nrows2, ncols> operator*(const matrix_t<nrows2, nrows> &lmat, const theMatrix &rmat)
	{
		Matrix<InfoTy, nrows2, ncols> result;
		for (int r = 0; r < nrows2; r++)
		{
			for (int c = 0; c < ncols; c++)
			{
				result(r, c) = 0;
				for (int i = 0; i < nrows; i++){
					result(r, c) += (lmat[r][i] * rmat(i,c));
				}
			}
		}
		return result;
	}
	friend theMatrix operator*(const InfoTy &lhs, const theMatrix &rhs){
		theMatrix mat;
		for (int i = 0; i < rhs._size; i++)
			mat._matrix[i] = lhs * rhs._matrix[i];
		return mat;
	}

//	template<typename Scalar, typename Mat> friend Mat operator*(const Scalar & val, const Mat & mat)
//	{
//		Mat result;
//		for (int i = 0; i < rhs._size; i++)
//			result._matrix[i] = val * mat._matrix[i];
//		return result;
//	}

	void printMatrix();
};

template<class InfoTy, int nrows, int ncols> 
InfoTy &Matrix<InfoTy, nrows, ncols>::operator()(int row, int col){
    // to do check array bounds
    if (row*ncols + col > _size)
    {
        std::cerr << "out of size in operator()(int, int)!\n";
        exit(0);
    }

    return _matrix[ncols*row + col];
}

template<class InfoTy, int nrows, int ncols> 
const InfoTy &Matrix<InfoTy, nrows, ncols>::operator()(int row, int col) const {
    // to do check array bounds
    if (row*ncols + col > _size)
    {
        std::cerr << "out of size operator()(int, int)!\n";
        exit(0);
    }

    return _matrix[ncols*row + col];

    // return (*this)(row, col); // recursive on all control paths, function will cause runtime stack overflow 
}

template<class InfoTy, int nrows, int ncols> 
void Matrix<InfoTy, nrows, ncols>::printMatrix()
{
    int n, i, j;
    for ( i = 0; i < nrows; i++)
    {
        n = i*ncols;
        for ( j = 0; j < ncols; j++)
            std::cout << _matrix[n++] << "\t";
        std::cout << "\n\n";
    }
}


template<class InfoTy, int nrows, int ncols> 
Matrix<InfoTy, nrows, ncols>::Matrix(const matrix_t<nrows, ncols> &mat)
{
	_size = ncols * nrows;
	_matrix = new InfoTy[_size];

	int n;
	for (int i = 0; i < nrows; i++)
	{
		n = i*ncols;
		for (int j = 0; j < ncols; j++)
			_matrix[n++] = mat[i][j];
	}
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>::Matrix(){
	_size = nrows*ncols;
	_matrix = new InfoTy[_size];
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>::Matrix(const theMatrix & rhs){
	_size = nrows*ncols;
	_matrix = new InfoTy[_size];
	*this = rhs;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>::Matrix(const InfoTy* m, int s){
	_size = ncols*ncols;

	if (s > _size){
		std::cerr << "out of size!\n";
		exit(0);
	}

	_matrix = new InfoTy[_size];
	for (int i = 0; i < s; i++){
		_matrix[i] = m[i];
	}
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols> & Matrix<InfoTy, nrows, ncols>::operator=(const theMatrix &m){
	// check if it is a self-assignment
	if (this == &m) return *this;

	for (int i = 0; i < m._size; i++)
	{
		_matrix[i] = m._matrix[i];
	}
	return *this;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator+(const theMatrix &rhs){
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] + rhs._matrix[i];
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator+(const theMatrix &rhs)const{
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] + rhs._matrix[i];
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols> & Matrix<InfoTy, nrows, ncols>::operator+=(const theMatrix &rhs){
	for (int i = 0; i < _size; i++)
		_matrix[i] += rhs._matrix[i];
	return *this;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator-(const theMatrix &rhs){
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] - rhs._matrix[i];
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator-(const theMatrix &rhs)const{
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] - rhs._matrix[i];
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols> & Matrix<InfoTy, nrows, ncols>::operator-=(const theMatrix &rhs){
	for (int i = 0; i < _size; i++)
		_matrix[i] -= rhs._matrix[i];
	return *this;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator*(const InfoTy &rhs){
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] * rhs;
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols>  Matrix<InfoTy, nrows, ncols>::operator*(const InfoTy &rhs)const{
	theMatrix mat;
	for (int i = 0; i < _size; i++)
		mat._matrix[i] = _matrix[i] * rhs;
	return mat;
}

template <class InfoTy, int nrows, int ncols>
Matrix<InfoTy, nrows, ncols> & Matrix<InfoTy, nrows, ncols>::operator*=(const InfoTy &rhs){
	for (int i = 0; i < _size; i++)
		_matrix[i] = _matrix[i] * rhs;
	return *this;
}

template <class InfoTy, int nrows, int ncols>
template< int ncols2 > 
Matrix<InfoTy, nrows, ncols2> Matrix<InfoTy, nrows, ncols>::operator*(const matrix_t<ncols, ncols2> &mat){
	Matrix<InfoTy, nrows, ncols2> result;
	for (int r = 0; r < nrows; r++)
	{
		for (int c = 0; c < ncols2; c++)
		{
			result(r, c) = 0;
			for (int i = 0; i < ncols; i++){
				result(r, c) += (_matrix[r*ncols + i] * mat[i][c]);
			}
		}
	}
	return result;

}

template <class InfoTy, int nrows, int ncols>
template< int ncols2 > 
Matrix<InfoTy, nrows, ncols2> Matrix<InfoTy, nrows, ncols>::operator*(const Matrix<InfoTy, ncols, ncols2> &mat){
//	if (ncols != nrows2)
//	{
//		std::cerr << "unmatched matrix size! \n";
//		exit(0);
//	}
	Matrix<InfoTy, nrows, ncols2> result;
	for (int r = 0; r < nrows; r++)
	{
		for (int c = 0; c < ncols2; c++)
		{
			result(r, c) = 0;
			for (int i = 0; i < ncols; i++){
				result(r, c) += (_matrix[r*ncols + i] * mat(i, c));
			}
		}
	}
	return result;
}
template <class InfoTy, int nrows, int ncols>
template< int ncols2 >
Matrix<InfoTy, nrows, ncols2> Matrix<InfoTy, nrows, ncols>::operator*(const Matrix<InfoTy, ncols, ncols2> &mat)const{
	//	if (ncols != nrows2)
	//	{
	//		std::cerr << "unmatched matrix size! \n";
	//		exit(0);
	//	}
	Matrix<InfoTy, nrows, ncols2> result;
	for (int r = 0; r < nrows; r++)
	{
		for (int c = 0; c < ncols2; c++)
		{
			result(r, c) = 0;
			for (int i = 0; i < ncols; i++){
				result(r, c) += (_matrix[r*ncols + i] * mat(i, c));
			}
		}
	}
	return result;

}

#endif
