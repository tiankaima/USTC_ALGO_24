#include <iostream>
#include <opencv2/opencv.hpp>
#include "maxflow/graph.h"
#include "GMM.h"

#define DEBUG 1

typedef Graph<double, double, double> GraphType;

static void construct_graph(
        const cv::Mat &img,
        const cv::Mat &mask,
        const GMM &background_GMM,
        const GMM &foreground_GMM,
        double lambda,
        const WeightType &weights,
        GraphType &graph
) {
    cv::Point p;
    for (p.y = 0; p.y < img.rows; p.y++) {
        for (p.x = 0; p.x < img.cols; p.x++) {
            // add node
            graph.add_node();
            int node_id = p.y * img.cols + p.x;
            cv::Vec3b color = img.at<cv::Vec3b>(p);

            // set t-weights
            double fromSource, toSink;
            if (mask.at<uchar>(p) == PR_BACKGROUND || mask.at<uchar>(p) == PR_FOREGROUND) {
                fromSource = -log(background_GMM(color));
                toSink = -log(foreground_GMM(color));
            } else if (mask.at<uchar>(p) == BACKGROUND) {
                fromSource = 0;
                toSink = lambda;
            } else // FOREGROUND
            {
                fromSource = lambda;
                toSink = 0;
            }
            graph.add_tweights(node_id, fromSource, toSink);

            // set n-weights
            if (p.x > 0) {
                double w = std::get<0>(weights).at<double>(p);
                graph.add_edge(node_id, node_id - 1, w, w);
            }
            if (p.x > 0 && p.y > 0) {
                double w = std::get<1>(weights).at<double>(p);
                graph.add_edge(node_id, node_id - img.cols - 1, w, w);
            }
            if (p.y > 0) {
                double w = std::get<2>(weights).at<double>(p);
                graph.add_edge(node_id, node_id - img.cols, w, w);
            }
            if (p.x < img.cols - 1 && p.y > 0) {
                double w = std::get<3>(weights).at<double>(p);
                graph.add_edge(node_id, node_id - img.cols + 1, w, w);
            }
        }
    }
}

static void estimate_segmentation(GraphType &graph, cv::Mat &mask) {
    graph.maxflow();
    cv::Point p;
    for (p.y = 0; p.y < mask.rows; p.y++) {
        for (p.x = 0; p.x < mask.cols; p.x++) {
            if (mask.at<uchar>(p) == PR_BACKGROUND || mask.at<uchar>(p) == PR_FOREGROUND) {
                if (graph.what_segment(p.y * mask.cols + p.x) == GraphType::SOURCE)
                    mask.at<uchar>(p) = PR_FOREGROUND;
                else
                    mask.at<uchar>(p) = PR_BACKGROUND;
            }
        }
    }
}

int handle(
        const std::string &input_filename,
        const std::string &mask_filename,
        const std::string &output_filename,
        const int min_x,
        const int min_y,
        const int max_x,
        const int max_y
) {
    // read input.png:
    cv::Mat img = cv::imread(input_filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Cannot read img file: input.png\n";
        return -1;
    }

    // read mask.png:
    cv::Mat mask = cv::imread(mask_filename, cv::IMREAD_GRAYSCALE);
    if (mask.empty()) {
        std::cout << "Cannot read img file: mask.png\n";
        return -1;
    }

    cv::Rect rect(min_x, min_y, max_x - min_x, max_y - min_y);

#if DEBUG
    {
        // output input-framed.png:
        auto img_cp = img.clone();
        cv::rectangle(img_cp, rect, cv::Scalar(0, 0, 255), 2);
        cv::imwrite("input-framed.png", img_cp);
    } // so that img_cp is released
#endif

    cv::Mat background_model, foreground_model;
    GMM background_GMM(background_model), foreground_GMM(foreground_model);
    cv::Mat component_id_xs(img.size(), CV_32SC1);

    init_mask_with_rect(mask, img.size(), rect);
    init_GMMs(img, mask, background_GMM, foreground_GMM);

#if DEBUG
    cv::imwrite("output-mask-init.png", mask);
#endif

    const double gamma = 50;
    const double lambda = 9 * gamma;
    const double beta = calc_beta(img);

    assign_GMM_components(img, mask, background_GMM, foreground_GMM, component_id_xs);
    learn_GMMs(img, mask, component_id_xs, background_GMM, foreground_GMM);

    int vertex_count = img.cols * img.rows;
    int edge_count = 2 * (4 * img.cols * img.rows - 3 * (img.cols + img.rows) + 2);
    GraphType graph = GraphType(vertex_count, edge_count);

    WeightType weight;
    calc_weights(img, weight, beta, gamma);
    construct_graph(img, mask, background_GMM, foreground_GMM, lambda, weight, graph);
    estimate_segmentation(graph, mask);

#if DEBUG
    cv::imwrite("output-mask.png", mask);
#endif

    // apply mask to img:
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            if (mask.at<uchar>(y, x) == PR_BACKGROUND || mask.at<uchar>(y, x) == BACKGROUND) {
                img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }
    cv::imwrite(output_filename, img);

    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        // default values
        return handle("input.png", "mask.png", "output.png", 137, 507, 986, 2041);
    }

    if (argc != 8) {
        std::cout << "Usage: ./grabcut input.png mask.png output.png min_x min_y max_x max_y\n";
        return -1;
    }

    return handle(argv[1], argv[2], argv[3], std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6]),
                  std::stoi(argv[7]));
}