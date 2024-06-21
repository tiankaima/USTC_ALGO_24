#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "maxflow/graph.h"

#define pii std::pair<int, int>
#define mp std::make_pair
#define inf 100000000

#define DEBUG 1

typedef Graph<double, double, double> GraphType;
int ans_x, ans_y, tex_x, tex_y, sum[10000][10000];
int di[4][2] = {1, 0, 0, 1, -1, 0, 0, -1}, cnt;
double min_cover = 0.1;
std::map<int, int> mp1;
std::map<pii, int> mp2;

cv::Mat new_img, img;
cv::Mat vis;
cv::Mat vis2;
cv::Mat pom;
cv::Mat img11;
cv::Mat img12;
cv::Mat img21;
cv::Mat img22;

int node_id(int x, int y) { return x * ans_y + y; }

pii node_edge(int x, int y, int k) {
    if (k == 0)
        return mp(x, y * 2 - 1);
    if (k == 1)
        return mp(x, y * 2);
    if (k == 2)
        return mp(x - 1, y * 2 - 1);
    if (k == 3)
        return mp(x, y * 2 - 2);
}

double norm(cv::Vec3b x) {
    return std::sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}

double normm(cv::Vec3b x, cv::Vec3b y) {
    return std::sqrt(
            (int(x[0]) - int(y[0])) * (int(x[0]) - int(y[0])) + (int(x[1]) - int(y[1])) * (int(x[1]) - int(y[1])) +
            (int(x[2]) - int(y[2])) * (int(x[2]) - int(y[2])));
}

int judge() {
    int fl = 0;
    for (int i = 0; i < ans_x; i++) {
        for (int j = 0; j < ans_y; j++) {
            sum[i][j] = 0;
            if (i == 0) {
                if (j == 0)
                    sum[i][j] = vis.at<uchar>(i, j);
                else
                    sum[i][j] = sum[i][j - 1] + vis.at<uchar>(i, j);
            } else {
                if (j == 0)
                    sum[i][j] = sum[i - 1][j] + vis.at<uchar>(i, j);
                else
                    sum[i][j] = sum[i][j - 1] + sum[i - 1][j] - sum[i - 1][j - 1] + vis.at<uchar>(i, j);
            }
            if (vis.at<uchar>(i, j) == 0)
                fl = 1;
        }
    }
    return fl;
}

double work(GraphType &gra, int cur_x, int cur_y, int edge_x, int edge_y) {

    // std::cout<<"   "<<cur_x<<' '<<cur_y<<std::endl;
    cnt = 0;

    std::cout << 1 << std::endl;
    for (int i = cur_x; i < edge_x; i++)
        for (int j = cur_y; j < edge_y; j++) {
            if (vis.at<uchar>(i, j) == 1) {
                int fl1 = 0, fl2 = 0;
                for (int k = 0; k < 4; k++) {
                    int dx = i + di[k][0], dy = j + di[k][1];
                    // if(i==141||j==141) std::cout<<i<<' '<<j<<' '<<dx<<' '<<dy<<' '<<edge_x<<' '<<edge_y<<' '<<int(vis.at<uchar>(dx,dy))<<std::endl;
                    if (dx < 0 || dx >= ans_x || dy < 0 || dx >= ans_y)
                        continue;
                    if (dx >= cur_x && dx < edge_x && dy >= cur_y && dy < edge_y && vis.at<uchar>(dx, dy) == 1)
                        continue;
                    // std::cout<<"hhh"<<std::endl;
                    if (vis.at<uchar>(dx, dy) == 1)
                        fl1 = 1;
                    else if (dx >= cur_x && dx < edge_x && dy >= cur_y && dx < edge_y)
                        fl2 = 1;
                }
                gra.add_node();
                int idd = cnt;
                mp1[node_id(i, j)] = cnt++;
                if (fl1 == 1) {
                    if (fl2 == 1)
                        gra.add_tweights(idd, 0, 0);
                    else
                        gra.add_tweights(idd, inf, 0);
                } else {
                    if (fl2 == 1)
                        gra.add_tweights(idd, 0, inf);
                    else
                        gra.add_tweights(idd, 0, 0);
                }
                // if(fl1&fl2!=1) std::cout<<' '<<fl1<<' '<<fl2<<' '<<i<<' '<<j<<std::endl;
            }
        }

    std::cout << 2 << std::endl;

    for (int i = cur_x; i < edge_x; i++) {
        for (int j = cur_y; j < edge_y; j++) {
            if (vis.at<uchar>(i, j) == 0)
                continue;
            int idd = mp1[node_id(i, j)];
            for (int k = 0; k < 2; k++) {
                int dx = i + di[k][0], dy = j + di[k][1];
                if (dx < cur_x || dx >= edge_x || dy < cur_y || dy >= edge_y || vis.at<uchar>(i, j) == 0)
                    continue;
                if (vis2.at<uchar>(node_edge(i, j, k).first, node_edge(i, j, k).second) == 0) {
                    double diss = normm(new_img.at<cv::Vec3b>(i, j), img.at<cv::Vec3b>(i - cur_x, j - cur_y)) +
                                  normm(new_img.at<cv::Vec3b>(dx, dy), img.at<cv::Vec3b>(dx - cur_x, dy - cur_y));
                    //    std::cout<<i<<' '<<j<<' '<<dx<<' '<<dy<<' '<<diss<<' '<<int(new_img.at<cv::Vec3b>(i,j)[0])<<' '<<int(img.at<cv::Vec3b>(i-cur_x,j-cur_y)[0])<<std::endl;
                    gra.add_edge(idd, mp1[node_id(dx, dy)], diss, diss);
                } else {
                    vis2.at<uchar>(node_edge(i, j, k).first, node_edge(i, j, k).second) = 0;
                    gra.add_node();
                    int iddd = cnt;
                    mp2[node_edge(i, j, k)] = cnt++;
                    cv::Vec3b a11, a12, a21, a22, b1, b2;
                    a11 = img11.at<cv::Vec3b>(node_edge(i, j, k).first, node_edge(i, j, k).second);
                    a12 = img12.at<cv::Vec3b>(node_edge(i, j, k).first, node_edge(i, j, k).second);
                    a21 = img21.at<cv::Vec3b>(node_edge(i, j, k).first, node_edge(i, j, k).second);
                    a22 = img22.at<cv::Vec3b>(node_edge(i, j, k).first, node_edge(i, j, k).second);
                    b1 = img.at<cv::Vec3b>(i - cur_x, j - cur_y);
                    b2 = img.at<cv::Vec3b>(dx - cur_x, dy - cur_y);
                    //    std::cout<<iddd<<' '<<idd<<' '<<mp1[node_id(dx,dy)]<<std::endl;
                    gra.add_tweights(iddd, 0, norm(a11 - a21) + norm(a12 - a22));
                    gra.add_edge(iddd, idd, norm(a11 - b1) + norm(a12 - b2), norm(a11 - b1) + norm(a12 - b2));
                    gra.add_edge(iddd, mp1[node_id(dx, dy)], norm(a21 - b1) + norm(a22 - b2),
                                 norm(a21 - b1) + norm(a22 - b2));
                    // std::cout<<iddd<<' '<<idd<<' '<<mp1[node_id(dx,dy)]<<std::endl;
                }
            }
        }
    }
    std::cout << 3 << std::endl;
    int flow = gra.maxflow();
    std::cout << flow << std::endl;
}

pii Random() {
    std::vector<pii > img_rand;

    // std::cout<<1<<std::endl;
    for (int i = 0; i < ans_x; i++) {
        for (int j = 0; j < ans_y; j++) {
            int edge_x = std::min(i + tex_x, ans_x) - 1;
            int edge_y = std::min(j + tex_y, ans_y) - 1;
            int ret = sum[edge_x][edge_y];
            // std::cout<<i<<' '<<j<<' '<<edge_x<<' '<<edge_y<<' '<<ret<<std::endl;
            if (i == 0) {
                if (j != 0)
                    ret -= sum[edge_x][j - 1];
            } else {
                if (j == 0)
                    ret -= sum[i - 1][edge_y];
                else
                    ret += sum[i - 1][j - 1] - sum[edge_x][j - 1] - sum[i - 1][edge_y];
            }
            if (ret >= min_cover * tex_x * tex_y)
                img_rand.push_back(mp(i, j));
        }
    }
    // std::srand(std::time(NULL));
    int ran = std::rand() % img_rand.size();
    // std::cout<<2<<' '<<ran<<' '<<img_rand.size()<<std::endl;
    return img_rand[ran];
}

int handle(
        const std::string &input_filename,
        const std::string &output_filename,
        const std::string &mmodule,
        const int ans_xx,
        const int ans_yy) {

#if DEBUG
    std::cout << -1 << std::endl;
#endif

    // read input.png:
    img = cv::imread(input_filename);
    if (img.empty()) {
        std::cout << "Cannot read img file: input.png\n";
        return -1;
    }

#if DEBUG
    {
        // output input-framed.png:
        auto img_cp = img.clone();
        cv::imwrite("input-framed.png", img_cp);
    } // so that img_cp is released
#endif

    ans_x = ans_xx;
    ans_y = ans_yy;
    tex_x = img.rows;
    tex_y = img.cols;

#if DEBUG
    std::cout << 0 << std::endl;
#endif

    new_img = cv::Mat(ans_x, ans_y, CV_8UC3, cv::Scalar(0, 0, 0));
    vis = cv::Mat(ans_x, ans_y, CV_8U, cv::Scalar(0));
    vis2 = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8U, cv::Scalar(0));
    pom = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8U);
    img11 = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8UC3);
    img12 = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8UC3);
    img21 = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8UC3);
    img22 = cv::Mat(ans_x * 2 - 1, ans_y * 2, CV_8UC3);

    cv::imwrite(output_filename, new_img);
#if DEBUG
    std::cout << 1 << std::endl;
#endif

    int tot = 1;

    for (int i = 0; i < tex_x; i++) {
        for (int j = 0; j < tex_y; j++) {
            vis.at<uchar>(i, j) = 1;
            new_img.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i, j);
            // std::cout<<int(new_img.at<cv::Vec3b>(i,j)[0])<<' '<<int(img.at<cv::Vec3b>(i,j)[0])<<std::endl;
        }
    }

    while (judge()) {

        int x;
        //    std::cin>>x;
        cv::imwrite(output_filename, new_img);
        std::cout << "   " << tot++ << std::endl;
        pii cur;
        if (mmodule == "RANDOM")
            cur = Random();
        // if(mmodule=="RANDOM") cur = Random();
        int cur_x = cur.first, cur_y = cur.second;
        int edge_x = std::min(tex_x + cur_x, ans_x);
        int edge_y = std::min(tex_y + cur_y, ans_y);
        GraphType *GRAPH = new GraphType(cur_x * cur_y, cur_x * cur_y);
        GraphType &gra = *GRAPH;

        double flow = work(gra, cur_x, cur_y, edge_x, edge_y);

        for (int i = cur_x; i < edge_x; i++) {
            for (int j = cur_y; j < edge_y; j++) {
                if (vis.at<uchar>(i, j) == 0) {
                    new_img.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i - cur_x, j - cur_y);
                    vis.at<uchar>(i, j) = 1;
                    continue;
                }
                int idd = mp1[node_id(i, j)];
                if (gra.what_segment(idd) == GraphType::SOURCE) {
                    for (int k = 0; k < 2; k++) {
                        int dx = i + di[k][0], dy = j + di[k][1];
                        if (dx < cur_x || dx >= edge_x || dy < cur_y || dy >= edge_y)
                            continue;
                        if (gra.what_segment(mp1[node_id(dx, dy)]) == GraphType::SOURCE)
                            continue;
                        pii de_id = node_edge(i, j, k);
                        vis2.at<uchar>(de_id.first, de_id.second) = 1;
                        pom.at<uchar>(de_id.first, de_id.second) = 0;
                        img11.at<cv::Vec3b>(de_id.first, de_id.second) = new_img.at<cv::Vec3b>(i, j);
                        img12.at<cv::Vec3b>(de_id.first, de_id.second) = new_img.at<cv::Vec3b>(dx, dy);
                        img21.at<cv::Vec3b>(de_id.first, de_id.second) = img.at<cv::Vec3b>(i - cur_x, j - cur_y);
                        img22.at<cv::Vec3b>(de_id.first, de_id.second) = img.at<cv::Vec3b>(dx - cur_x, dy - cur_y);
                    }
                } else {
                    for (int k = 0; k < 2; k++) {
                        int dx = i + di[k][0], dy = j + di[k][1];
                        if (dx < cur_x || dx >= edge_x || dy < cur_y || dy >= edge_y)
                            continue;
                        if (gra.what_segment(mp1[node_id(dx, dy)]) == GraphType::SINK)
                            continue;
                        pii de_id = node_edge(i, j, k);
                        vis2.at<uchar>(de_id.first, de_id.second) = 1;
                        pom.at<uchar>(de_id.first, de_id.second) = 1;
                        img11.at<cv::Vec3b>(de_id.first, de_id.second) = new_img.at<cv::Vec3b>(dx, dy);
                        img12.at<cv::Vec3b>(de_id.first, de_id.second) = new_img.at<cv::Vec3b>(i, j);
                        img21.at<cv::Vec3b>(de_id.first, de_id.second) = img.at<cv::Vec3b>(i - cur_x, j - cur_y);
                        img22.at<cv::Vec3b>(de_id.first, de_id.second) = img.at<cv::Vec3b>(dx - cur_x, dy - cur_y);
                    }
                    new_img.at<cv::Vec3b>(i, j) = img.at<cv::Vec3b>(i - cur_x, j - cur_y);
                }
            }
        }
        // break;
    }

#if DEBUG
    std::cout << 2 << std::endl;
#endif

    cv::imwrite(output_filename, new_img);

#if DEBUG
    std::cout << 3 << std::endl;
#endif

    return 0;
}

int main(int argc, char **argv) {
    // freopen("test.out","w",stdout);
    std::cout << 111 << std::endl;
    return handle("strawberry.png", "strawberry_output.png", "RANDOM", 1000, 1000);

    return 0;
}