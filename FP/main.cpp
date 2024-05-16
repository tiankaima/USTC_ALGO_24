#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

int main() {
    int x_min = 983 - 860, x_max = 983;
    int y_min = 2048 - 1460, y_max = 2048;

    // read input.png:
    cv::Mat input = cv::imread("input.png", cv::IMREAD_COLOR);
    if (input.empty()) {
        std::cerr << "Error: Could not read input.png" << std::endl;
        return 1;
    }

    // draw rectangle:
    cv::rectangle(input, cv::Point(x_min, y_min), cv::Point(x_max, y_max), cv::Scalar(0, 255, 0), 2);

    // write output.png:
    cv::imwrite("output.png", input);

    // display output.png:
    cv::imshow("output.png", input);
    cv::waitKey(0);


    return 0;
}
