#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "Usage: opencv_test <image path>" << endl;
        return -1;
    }

    char *imgName = argv[1];
    Mat image;

    image = imread(imgName, 1);
    if (!image.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat gray_img;

    cvtColor(image, gray_img, COLOR_RGB2GRAY);
    imwrite("images/result.jpg", gray_img);

    return 0;
}