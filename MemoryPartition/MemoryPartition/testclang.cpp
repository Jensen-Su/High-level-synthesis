#include <iostream>
using namespace std;

template<int dim, int loop>
class Reference{
    public:
        template< int r, int c> using matrix = int[r][c];

    	Reference(const matrix<dim, loop> &mat){}	
};

template<int dim, int loop>
class Partition{
    //...
    public:
    //...
    template<int r, int c> using matrix_t = int[r][c];
    template<int r, int c>void readPattern(const matrix_t<r,c> &pattern){}
};

#include <array>
template<int size> void foo(std::array<int, size> a){}

struct ImageSize{
    int width;
    int height;
    ImageSize(int w, int h): width(w), height(h){}
};

const ImageSize SD(640, 480);
const ImageSize HD(1280, 720);
const ImageSize FullHD(1920, 1080);
const ImageSize WQXGA(2560, 1600);
const ImageSize FourK(3840, 2160);


int main(){

    foo<4>({1,2,3});
    const int DENOISE_UR[3][4] = {0};
    Partition<1,2> partition;
    partition.readPattern(DENOISE_UR);
    cout << "hello from c++ world!\n";

    cout << "SD: " << SD.width << " x " << SD.height << "\n";

    char s1[80] = {"this is a developed country."};
    char *s2[8] ;
    s2[0] = &s1[10];
    cout << *s2 << endl;

    return 0;
}
