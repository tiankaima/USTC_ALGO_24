//
// Code extracted from OpenCV
//

#include "GMM.h"

#define CV_TERMCRIT_ITER 1

GMM::GMM(cv::Mat &_model) { // NOLINT
    const int modelSize = 3 /*mean_*/ + 9 /*covariance*/ + 1 /*component weight*/;
    if (_model.empty()) {
        _model.create(1, modelSize * componentsCount, CV_64FC1);
        _model.setTo(cv::Scalar(0));
    }

    model_ = _model;
    coefficients_ = model_.ptr<double>(0);
    mean_ = coefficients_ + componentsCount;
    cov_ = mean_ + 3 * componentsCount;

    total_sample_count = 0;

    for (int ci = 0; ci < componentsCount; ci++)
        if (coefficients_[ci] > 0)
            calc_inv_and_det(ci, 0.0);

}

double GMM::operator()(const cv::Vec3d &color) const {
    double res = 0;
    for (int ci = 0; ci < componentsCount; ci++)
        res += coefficients_[ci] * (*this)(ci, color);
    return res;
}

double GMM::operator()(int ci, const cv::Vec3d &color) const {
    double res = 0;
    if (coefficients_[ci] > 0) {
        cv::Vec3d diff = color;
        double *m = mean_ + 3 * ci;
        diff[0] -= m[0];
        diff[1] -= m[1];
        diff[2] -= m[2];

        double result = 0;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                result += diff[i] * diff[j] * inverse_cov_[ci][i][j];

        res = 1.0f / sqrt(cov_det_[ci]) * exp(-0.5f * result);
    }
    return res;
}

int GMM::which_component(const cv::Vec3d &color) const {
    int k = 0;
    double max = 0;

    for (int ci = 0; ci < componentsCount; ci++) {
        double p = (*this)(ci, color);
        if (p > max) {
            k = ci;
            max = p;
        }
    }
    return k;
}

void GMM::init_learning() {
    for (int ci = 0; ci < componentsCount; ci++) {
        sum_[ci][0] = sum_[ci][1] = sum_[ci][2] = 0;
        prod_[ci][0][0] = prod_[ci][0][1] = prod_[ci][0][2] = 0;
        prod_[ci][1][0] = prod_[ci][1][1] = prod_[ci][1][2] = 0;
        prod_[ci][2][0] = prod_[ci][2][1] = prod_[ci][2][2] = 0;
        sample_counts[ci] = 0;
    }
    total_sample_count = 0;
}

void GMM::add_sample(int ci, const cv::Vec3d &color) {
    sum_[ci][0] += color[0];
    sum_[ci][1] += color[1];
    sum_[ci][2] += color[2];
    prod_[ci][0][0] += color[0] * color[0];
    prod_[ci][0][1] += color[0] * color[1];
    prod_[ci][0][2] += color[0] * color[2];
    prod_[ci][1][0] += color[1] * color[0];
    prod_[ci][1][1] += color[1] * color[1];
    prod_[ci][1][2] += color[1] * color[2];
    prod_[ci][2][0] += color[2] * color[0];
    prod_[ci][2][1] += color[2] * color[1];
    prod_[ci][2][2] += color[2] * color[2];
    sample_counts[ci]++;
    total_sample_count++;
}

void GMM::end_learning() {
    for (int ci = 0; ci < componentsCount; ci++) {
        int n = sample_counts[ci];
        if (n == 0)
            coefficients_[ci] = 0;
        else {
            double inv_n = 1.0 / n;
            coefficients_[ci] = (double) n / total_sample_count;

            double *m = mean_ + 3 * ci;
            m[0] = sum_[ci][0] * inv_n;
            m[1] = sum_[ci][1] * inv_n;
            m[2] = sum_[ci][2] * inv_n;

            double *c = cov_ + 9 * ci;
            c[0] = prod_[ci][0][0] * inv_n - m[0] * m[0];
            c[1] = prod_[ci][0][1] * inv_n - m[0] * m[1];
            c[2] = prod_[ci][0][2] * inv_n - m[0] * m[2];
            c[3] = prod_[ci][1][0] * inv_n - m[1] * m[0];
            c[4] = prod_[ci][1][1] * inv_n - m[1] * m[1];
            c[5] = prod_[ci][1][2] * inv_n - m[1] * m[2];
            c[6] = prod_[ci][2][0] * inv_n - m[2] * m[0];
            c[7] = prod_[ci][2][1] * inv_n - m[2] * m[1];
            c[8] = prod_[ci][2][2] * inv_n - m[2] * m[2];

            calc_inv_and_det(ci, 0.01);
        }
    }
}

void GMM::calc_inv_and_det(int ci, const double singular_fix) {
    if (coefficients_[ci] > 0) {
        double *c = cov_ + 9 * ci;
        double det = c[0] * (c[4] * c[8] - c[5] * c[7]) - c[1] * (c[3] * c[8] - c[5] * c[6]) +
                     c[2] * (c[3] * c[7] - c[4] * c[6]);
        if (det <= 1e-6 && singular_fix > 0) {
            c[0] += singular_fix;
            c[4] += singular_fix;
            c[8] += singular_fix;
            det = c[0] * (c[4] * c[8] - c[5] * c[7]) - c[1] * (c[3] * c[8] - c[5] * c[6]) +
                  c[2] * (c[3] * c[7] - c[4] * c[6]);
        }
        cov_det_[ci] = det;

        double inv_det = 1.0 / det;
        inverse_cov_[ci][0][0] = (c[4] * c[8] - c[5] * c[7]) * inv_det;
        inverse_cov_[ci][1][0] = -(c[3] * c[8] - c[5] * c[6]) * inv_det;
        inverse_cov_[ci][2][0] = (c[3] * c[7] - c[4] * c[6]) * inv_det;
        inverse_cov_[ci][0][1] = -(c[1] * c[8] - c[2] * c[7]) * inv_det;
        inverse_cov_[ci][1][1] = (c[0] * c[8] - c[2] * c[6]) * inv_det;
        inverse_cov_[ci][2][1] = -(c[0] * c[7] - c[1] * c[6]) * inv_det;
        inverse_cov_[ci][0][2] = (c[1] * c[5] - c[2] * c[4]) * inv_det;
        inverse_cov_[ci][1][2] = -(c[0] * c[5] - c[2] * c[3]) * inv_det;
        inverse_cov_[ci][2][2] = (c[0] * c[4] - c[1] * c[3]) * inv_det;
    }
}

double calc_beta(const cv::Mat &img) {
    double beta = 0;
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3d color = img.at<cv::Vec3b>(y, x);
            if (x > 0) // left
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y, x - 1);
                beta += diff.dot(diff);
            }
            if (y > 0 && x > 0) // up-left
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x - 1);
                beta += diff.dot(diff);
            }
            if (y > 0) // up
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x);
                beta += diff.dot(diff);
            }
            if (y > 0 && x < img.cols - 1) // up-right
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x + 1);
                beta += diff.dot(diff);
            }
        }
    }
    if (beta <= std::numeric_limits<double>::epsilon())
        beta = 0;
    else
        beta = 1.f / (2 * beta / (4 * img.cols * img.rows - 3 * img.cols - 3 * img.rows + 2));

    return beta;
}

void calc_weights(const cv::Mat &img, WeightType &weights, double beta, double gamma) {
    const double gammaDivSqrt2 = gamma / std::sqrt(2.0f);
    std::get<0>(weights).create(img.rows, img.cols, CV_64FC1);
    std::get<1>(weights).create(img.rows, img.cols, CV_64FC1);
    std::get<2>(weights).create(img.rows, img.cols, CV_64FC1);
    std::get<3>(weights).create(img.rows, img.cols, CV_64FC1);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            cv::Vec3d color = img.at<cv::Vec3b>(y, x);
            if (x - 1 >= 0) // left
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y, x - 1);
                std::get<0>(weights).at<double>(y, x) = gamma * exp(-beta * diff.dot(diff));
            } else
                std::get<0>(weights).at<double>(y, x) = 0;
            if (x - 1 >= 0 && y - 1 >= 0) // up-left
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x - 1);
                std::get<1>(weights).at<double>(y, x) = gammaDivSqrt2 * exp(-beta * diff.dot(diff));
            } else
                std::get<1>(weights).at<double>(y, x) = 0;
            if (y - 1 >= 0) // up
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x);
                std::get<2>(weights).at<double>(y, x) = gamma * exp(-beta * diff.dot(diff));
            } else
                std::get<2>(weights).at<double>(y, x) = 0;
            if (x + 1 < img.cols && y - 1 >= 0) // up-right
            {
                cv::Vec3d diff = color - (cv::Vec3d) img.at<cv::Vec3b>(y - 1, x + 1);
                std::get<3>(weights).at<double>(y, x) = gammaDivSqrt2 * exp(-beta * diff.dot(diff));
            } else
                std::get<3>(weights).at<double>(y, x) = 0;
        }
    }
}


void init_mask_with_rect(cv::Mat &mask, cv::Size imgSize, cv::Rect rect) {
    auto mask_bk = mask.clone();
    mask.create(imgSize, CV_8UC1);
    mask.setTo(BACKGROUND);

    rect.x = std::max(0, rect.x);
    rect.y = std::max(0, rect.y);
    rect.width = std::min(rect.width, imgSize.width - rect.x);
    rect.height = std::min(rect.height, imgSize.height - rect.y);

    (mask(rect)).setTo(cv::Scalar(PR_FOREGROUND));

    for (int y = 0; y < mask.rows; y++) {
        for (int x = 0; x < mask.cols; x++) {
            if (mask_bk.at<uchar>(y, x) != 0)
                mask.at<uchar>(y, x) = BACKGROUND;
        }
    }
}


void init_GMMs(const cv::Mat &img, const cv::Mat &mask, GMM &background_GMM, GMM &foreground_GMM) {
    const int kMeansItCount = 10;
    const int kMeansType = cv::KMEANS_PP_CENTERS;

    cv::Mat bgdLabels, fgdLabels;
    std::vector<cv::Vec3f> background_samples, foreground_samples;
    cv::Point p;
    for (p.y = 0; p.y < img.rows; p.y++) {
        for (p.x = 0; p.x < img.cols; p.x++) {
            if (mask.at<uchar>(p) == BACKGROUND || mask.at<uchar>(p) == PR_BACKGROUND)
                background_samples.push_back((cv::Vec3f) img.at<cv::Vec3b>(p));
            else // FOREGROUND | PR_FOREGROUND
                foreground_samples.push_back((cv::Vec3f) img.at<cv::Vec3b>(p));
        }
    }

    {
        cv::Mat _bgdSamples((int) background_samples.size(), 3, CV_32FC1, &background_samples[0][0]);
        int num_clusters = GMM::componentsCount;
        num_clusters = std::min(num_clusters, (int) background_samples.size());
        kmeans(_bgdSamples, num_clusters, bgdLabels,
               cv::TermCriteria(CV_TERMCRIT_ITER, kMeansItCount, 0.0), 0, kMeansType);
    }
    {
        cv::Mat _fgdSamples((int) foreground_samples.size(), 3, CV_32FC1, &foreground_samples[0][0]);
        int num_clusters = GMM::componentsCount;
        num_clusters = std::min(num_clusters, (int) foreground_samples.size());
        kmeans(_fgdSamples, num_clusters, fgdLabels,
               cv::TermCriteria(CV_TERMCRIT_ITER, kMeansItCount, 0.0), 0, kMeansType);
    }

    background_GMM.init_learning();
    for (int i = 0; i < (int) background_samples.size(); i++)
        background_GMM.add_sample(bgdLabels.at<int>(i, 0), background_samples[i]);
    background_GMM.end_learning();

    foreground_GMM.init_learning();
    for (int i = 0; i < (int) foreground_samples.size(); i++)
        foreground_GMM.add_sample(fgdLabels.at<int>(i, 0), foreground_samples[i]);
    foreground_GMM.end_learning();
}


void assign_GMM_components(
        const cv::Mat &img,
        const cv::Mat &mask,
        const GMM &background_GMM,
        const GMM &foreground_GMM,
        cv::Mat &component_id_xs
) {
    cv::Point p;
    for (p.y = 0; p.y < img.rows; p.y++) {
        for (p.x = 0; p.x < img.cols; p.x++) {
            cv::Vec3d color = img.at<cv::Vec3b>(p);
            component_id_xs.at<int>(p) = mask.at<uchar>(p) == BACKGROUND || mask.at<uchar>(p) == PR_BACKGROUND ?
                                         background_GMM.which_component(color) : foreground_GMM.which_component(color);
        }
    }
}


void learn_GMMs(
        const cv::Mat &img,
        const cv::Mat &mask,
        const cv::Mat &component_id_xs,
        GMM &background_GMM,
        GMM &foreground_GMM
) {
    background_GMM.init_learning();
    foreground_GMM.init_learning();
    cv::Point p;
    for (int ci = 0; ci < GMM::componentsCount; ci++) {
        for (p.y = 0; p.y < img.rows; p.y++) {
            for (p.x = 0; p.x < img.cols; p.x++) {
                if (component_id_xs.at<int>(p) == ci) {
                    if (mask.at<uchar>(p) == BACKGROUND || mask.at<uchar>(p) == PR_BACKGROUND)
                        background_GMM.add_sample(ci, img.at<cv::Vec3b>(p));
                    else
                        foreground_GMM.add_sample(ci, img.at<cv::Vec3b>(p));
                }
            }
        }
    }
    background_GMM.end_learning();
    foreground_GMM.end_learning();
}