#include "Patterns.h"
#include "ConflictDetection.h"
#include "MyMatrix.h"
#include <iostream>
#include <string>
#include "LPFormat.h"
#include "GMPPartition.h"
#include "ChenyuePartition.h"
#include "CodeTransformation.h"

using std::cout;
using namespace patterns;
int main(){
	cout << "DENOISE:\n";
	printMatrix(DENOISE);
	cout << "DECONV:\n";
	printMatrix(DECONV);
	cout << "BICUBIC:\n";
	printMatrix<int,3,3>(BICUBIC); // The compiler can deduct the <int,3,3> itself
	cout << "MOTION_C:\n";
	printMatrix<int,3,3>(MOTION_C);
	cout << "DENOISE_UR:\n";
	printMatrix(DENOISE_UR);
	cout << "SOBEL:\n";
	printMatrix(SOBEL,5,5);
	cout << "MOTION_LV:\n";
	printMatrix(MOTION_LV,6,3);
	cout << "MOTION_LH:\n";
	printMatrix(MOTION_LH,3,6);
	
	const int test[] = { 1, 2, 3, 4 };
	// test1-- using an array to initialize 
	Matrix<int, 2, 2> mat(test,4);
	mat.printMatrix();
	// test2-- using a Matrix<int,2,2> to initialize
	Matrix<int, 2, 2> mat2(mat);
	mat2.printMatrix();
	// test3-- using a matrix to initialize
	Matrix<int, 3, 3> mat3(DENOISE);
	cout << "mat3 -- DENOISE:\n";
	mat3.printMatrix();

	Matrix<int, 3, 3> mat4(DECONV);
	cout << "mat4 -- DECONV:\n";
	mat4.printMatrix();

	Matrix<int, 3, 3 > mat5;

	mat5 = mat3 + mat4;
	mat5 = DENOISE;
	cout << "mat5 = mat3+mat4:\n";
	mat5.printMatrix();

	mat5 = 5 * mat5 * 3;
	mat5 *= 3;
	cout << "mat5 = 5*mat5*3:\n";
	mat5.printMatrix();

	Matrix<int, 2, 1> mat6;
	int mm[][1] = { 1, 2 };
	mat6 = mat * mm;
	cout << "mat6 = mat*mm\n";
	mat6.printMatrix();

	mat6 = mat * mat6;
	cout << "mat6 = mat * mat6:\n";
	mat6.printMatrix();

	const int lmat[][2] = { 1, 2, 3, 4 };
	mat6 = lmat * mat6;
	cout << "mat6 = lmat*mat6:\n";
	mat6.printMatrix();
	cout << std::endl;


	cout << "\n now is testing of ConflictDetection.h!\n";
	const int A[][3] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	const int A2[][3] = { 2, 3, 8, 9, 0, 3, 8, 4, 5 };
	const int C[3][1] = { 2, 3 };
	const int C2[3][1] = { 3, 2, 1 };
	Reference<3, 3> Ref(mat3, C);
	Reference<3, 3> Ref1(A2, C2);
	Ref.print();
	Reference<3, 3> Ref2(A, C);
	cout << "Ref2:\n"; 
	Ref2.print();
	const int alp[][3] = { 1, 2, 2};
	Matrix<int, 1, 3> alpha(alp);

	cout << std::endl;
	Reference<1, 3> Ref3 = alp * Ref2;
	cout << "ref3 = alpha * Ref2:\n";
	Ref3.print();

	cout << "\n now is testing of LPFormula.h!\n";
	const int lpA1[][2] = { 1, -2 };
	const int lpA2[][2] = { 3, 4 };
	LPFormula<2> lp;
	lp.addItem(lpA1, "i");
	lp.addItem(lpA2, "x");
	lp.addItem(3, "k0");
	lp.addItem(5, "k1");
	lp.print();


	Polytope<3, 3> polytope(Ref, Ref2);
	polytope.setThirdReference( &Ref1);
	polytope.isConflict(alpha, 2);
//	polytope.dumpPolytope("ILPPolytope.txt");

	GMPPartition<2, 2> partition;
	partition.readPattern(DECONV);
	partition.printReferences();
    partition.partition();
    partition.printSolutions();

    std::cout << "Chenyue et al's partitioning...\n\n";
	ChenyuePartition<2, 2> cypartition;
	partition.printReferences();
	cypartition.readPattern(CANNY);
    cypartition.partition();
    
    CodeTrans<1920, 1080> code(LOG);
    ofstream outf("code.c");
    const int alpha_code[1][2] = {1,4};
    code.initialize(alpha_code, 13);
    code.genAddress(outf);
    outf.close();


    return 0;
}
