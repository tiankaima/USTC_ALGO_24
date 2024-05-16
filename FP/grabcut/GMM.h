//
// Code extracted from OpenCV
//

#ifndef GRABCUT_GMM_H
#define GRABCUT_GMM_H

#include <opencv2/opencv.hpp>

typedef std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat> WeightType;

enum GrabCutClasses {
    BACKGROUND = 0,  //!< an obvious background pixels
    FOREGROUND = 1,  //!< an obvious foreground (object) pixel
    PR_BACKGROUND = 2,  //!< a possible background pixel
    PR_FOREGROUND = 3   //!< a possible foreground pixel
};

/*
 GMM - Gaussian Mixture Model
*/
class GMM {
public:
    static const int componentsCount = 5;

    explicit GMM(cv::Mat &_model);

    double operator()(const cv::Vec3d &color) const;

    double operator()(int ci, const cv::Vec3d &color) const;

    [[nodiscard]] int which_component(const cv::Vec3d &color) const;

    void init_learning();

    void add_sample(int ci, const cv::Vec3d &color);

    void end_learning();

private:
    void calc_inv_and_det(int ci, double singular_fix);

    cv::Mat model_;
    double *coefficients_;
    double *mean_;
    double *cov_;

    double inverse_cov_[componentsCount][3][3];
    double cov_det_[componentsCount];
    double sum_[componentsCount][3];
    double prod_[componentsCount][3][3];

    int sample_counts[componentsCount];
    int total_sample_count;
};

double calc_beta(const cv::Mat &img);

void calc_weights(
        const cv::Mat &img,
        WeightType &weights,
        double beta,
        double gamma
);

void init_mask_with_rect(cv::Mat &mask, cv::Size imgSize, cv::Rect rect);

void init_GMMs(const cv::Mat &img, const cv::Mat &mask, GMM &background_GMM, GMM &foreground_GMM);

void assign_GMM_components(
        const cv::Mat &img,
        const cv::Mat &mask,
        const GMM &background_GMM,
        const GMM &foreground_GMM,
        cv::Mat &component_id_xs
);

void learn_GMMs(
        const cv::Mat &img,
        const cv::Mat &mask,
        const cv::Mat &component_id_xs,
        GMM &background_GMM,
        GMM &foreground_GMM
);


#endif //GRABCUT_GMM_H
