#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>

int main() {

  cv::Mat img = cv::imread("images/is.png", cv::IMREAD_GRAYSCALE);
  if (img.empty()) {
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
  cv::threshold(img, img, 120, 255, cv::THRESH_BINARY);
  cv::Mat bin_zoom;
  cv::resize(img, bin_zoom, cv::Size(), 6.0, 6.0, cv::INTER_NEAREST);
  cv::namedWindow("Binary image", cv::WINDOW_NORMAL);
  cv::resizeWindow("Binary image", 800, 800);
  cv::imshow("Binary image", bin_zoom);
  cv::waitKey(0);
  cv::destroyAllWindows();

  cv::Mat dist;
  cv::distanceTransform(binary, dist, cv::DIST_C, 5);

  cv::Mat skel = cv::Mat::zeros(dist.size(), CV_8UC1);
  for (int y = 1; y < dist.rows - 1; y++) {
    for (int x = 1; x < dist.cols - 1; x++) {
      float d = dist.at<float>(y, x);
      if (d > 0 && d >= dist.at<float>(y - 1, x) &&
          d >= dist.at<float>(y + 1, x) && d >= dist.at<float>(y, x - 1) &&
          d >= dist.at<float>(y, x + 1)) {
        skel.at<uchar>(y, x) = 255;
      }
    }
  }

  return 0;
}
