// ===== -------------------------------------------------------------------------- ===== //
// ===== Eight patterns of kernels usually used in image processing                 ===== //
// ===== Copyright@sjcshindou 2015-07-12                                            ===== //
// ===== -------------------------------------------------------------------------- ===== //


#ifndef _PATTERNS_H
#define _PATTERNS_H
#include <iostream>
namespace patterns{
	const int nrefs = 4;

	const int DENOISE[][3] = { { 0, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } };
	const int DECONV[][3] = { { 0, 1, 0 }, { 1, 1, 1 }, { 0, 1, 0 } };
	const int BICUBIC[][3] = { { 1, 0, 1 }, { 0, 0, 0 }, { 1, 0, 1 } };
	const int MOTION_C[][3] = { { 1, 1, 0 }, { 1, 1, 0 }, { 0, 0, 0 } };
	const int DENOISE_UR[][4] = { { 0, 1, 1, 0 }, { 1, 1, 1, 1 }, { 0, 1, 1, 0 } };
	const int SOBEL[][5] = { { 0, 0, 0, 0, 0 }, { 0, 1, 1, 1, 0 }, { 0, 1, 1, 1, 0 }, { 0, 1, 1, 1, 0 }, { 0, 0, 0, 0, 0 } };
	const int MOTION_LV[][3] = { { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 } };
	const int MOTION_LH[][6] = { { 0, 0, 0, 0, 0, 0 }, { 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0 } };
	const int LOG[][5] = { { 0, 0, 1, 0, 0 }, { 0, 1, 1, 1, 0 }, { 1, 1, 1, 1, 1 }, { 0, 1, 1, 1, 0 }, { 0, 0, 1, 0, 0 } };
	const int CANNY[][5] = { { 1, 1, 1, 1, 1, }, { 1, 1, 1, 1, 1, }, { 1, 1, 1, 1, 1, }, { 1, 1, 1, 1, 1, }, { 1, 1, 1, 1, 1, } };
	const int PREWITT[][3] = { { 1, 1, 1 }, { 1, 0, 1 }, { 1, 1, 1 } };
	const int SE[][3] = { { 0, 1, 0 }, { 1, 1, 1 }, { 0, 1, 0 } };
    const int MEDIAN[][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    const int GAUSSIAN[][5] = {};

    struct ImageSize{
        int _width;
        int _height;
        ImageSize(int w, int h): _width(w), _height(h){}
    };

    const ImageSize SD(640, 480);
    const ImageSize HD(1280, 720);
    const ImageSize FullHD(1920, 1080);
    const ImageSize WQXGA(2560, 1600);
    const ImageSize FourK(3840, 2160);

	template<typename Ty> void printMatrix(const Ty matrix, int rows, int cols){
		for (int r = 0; r < rows; r++){
			for (int c = 0; c < cols; c++)
			{
				std::cout << matrix[r][c] << "\t";
			}
			std::cout << "\n\n";
		}
	}

	template<typename Ty, int rows, int cols> using matrix = Ty[rows][cols];
	template<typename Ty, int rows, int cols> void printMatrix(const matrix<Ty, rows,cols> &mat){
		for (int r = 0; r < rows; r++){
			for (int c = 0; c < cols; c++)
			{
				std::cout << mat[r][c] << "\t";
			}
			std::cout << "\n\n";
		}
	}


}
#endif
