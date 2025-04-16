#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include "Skeletonize.h"

int main(int argc, char* argv[]) {
    cv::Mat img = cv::imread("images/test.png", cv::IMREAD_GRAYSCALE);
    if(img.empty()) {
        std::cerr << "Error: could not open or find the image!" << std::endl;
        return -1;
    }
    cv::imshow("Image", img);
    cv::waitKey(0);
    cv::threshold(img, img, 127, 255, cv::THRESH_BINARY);
    cv::imshow("Binary image", img);
    cv::waitKey(0);
    skeleton_tracer_t tracer;
    tracer.W = img.cols;
    tracer.H = img.rows;
    tracer.im = new skeleton_tracer_t::uchar[tracer.W * tracer.H];
    memcpy(tracer.im, img.data, tracer.W * tracer.H);
    tracer.thinning_zs();
    skeleton_tracer_t::polyline_t* polylines = tracer.trace_skeleton(0, 0, tracer.W, tracer.H, 10);
    cv::Mat output;
    cv::cvtColor(img, output, cv::COLOR_GRAY2BGR);
    for(auto* poly = polylines; poly; poly = poly->next) {
        skeleton_tracer_t::point_t* point = poly->head;
        while(point && point->next) {
            cv::Point p1(point->x, point->y);
            cv::Point p2(point->next->x, point->next->y);
            cv::line(output, p1, p2, cv::Scalar(0, 0, 255), 1);
            point = point->next;
        }
    }
    cv::imshow("Skeleton output", output);
    cv::waitKey(0);
    cv::imwrite("output.png", output);
    tracer.destroy_polylines(polylines);
    delete[] tracer.im;
    tracer.destroy();
    return 0;
}