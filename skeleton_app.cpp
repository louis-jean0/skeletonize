#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include "Skeletonize.h"

int main(int argc, char* argv[]) {
    cv::Mat img = cv::imread("images/is.png", cv::IMREAD_GRAYSCALE);
    if(img.empty()) {
        std::cerr << "Error : could not open or find the image!" << std::endl;
        return -1;
    }
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Image", 800, 800);
    cv::Mat img_zoom;
    cv::resize(img, img_zoom, cv::Size(), 6.0, 6.0, cv::INTER_NEAREST);
    cv::imshow("Image", img_zoom);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::Mat bin;
    cv::threshold(img_zoom, bin, 120, 255, cv::THRESH_BINARY);
    cv::namedWindow("Binary image", cv::WINDOW_NORMAL);
    cv::resizeWindow("Binary image", 800, 800);
    cv::imshow("Binary image", bin);
    cv::waitKey(0);
    cv::destroyAllWindows();
    skeleton_tracer_t tracer;
    tracer.W = bin.cols;
    tracer.H = bin.rows;
    tracer.im = new skeleton_tracer_t::uchar[tracer.W * tracer.H];
    for (int i = 0; i < tracer.W * tracer.H; i++) {
        tracer.im[i] = img_zoom.data[i] > 0 ? 1 : 0;
    }
    tracer.thinning_zs();
    skeleton_tracer_t::polyline_t* polylines = tracer.trace_skeleton(0, 0, tracer.W, tracer.H, 10);
    cv::Mat output(img_zoom.rows, img_zoom.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    //cv::cvtColor(img, output, cv::COLOR_GRAY2BGR);
    for(auto* poly = polylines; poly; poly = poly->next) {
        skeleton_tracer_t::point_t* point = poly->head;
        while(point && point->next) {
            cv::Point p1(point->x, point->y);
            cv::Point p2(point->next->x, point->next->y);
            cv::line(output, p1, p2, cv::Scalar(0, 0, 255), 1);
            point = point->next;
        }
    }
    cv::namedWindow("Skeleton output", cv::WINDOW_NORMAL);
    cv::resizeWindow("Skeleton output", 800, 800);
    cv::Mat out_zoom;
    cv::resize(output, out_zoom, cv::Size(), 6.0, 6.0, cv::INTER_NEAREST);
    cv::imshow("Skeleton output", out_zoom);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("output.png", output);
    tracer.destroy_polylines(polylines);
    tracer.destroy();
    return 0;
}