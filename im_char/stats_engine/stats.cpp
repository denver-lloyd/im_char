#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <numeric>
using std::accumulate;
#include <cmath>
#include <map>
using std::map;
#include <string>
using std::string;
using std::make_pair;
using std::round;
#include "../image/image_stack.cpp"


/**
 * Get the average offset of an image
 *
 * @param img_stack 2D vector of pixel data
 *
 * @return avg offset of 2D array
*/
template<typename T>
T avg_offset(ImageStack<T>& img_stack){

    const size_t size = img_stack.size();

    T avg = accumulate(img_stack.begin(), img_stack.end(), 0);
    avg /= size;

    return avg;
}

/**
 * calculate the average frame of a frame stack
 *
 * @param img_stack 2D/3D vector of pixel data
 *
 * @return average frame of 3D vector
*/
template<typename T>
ImageStack<T> avg_img(ImageStack<T>& img){
    if (img.L == 0){
        return img;
    }

    ImageStack<T> img2d(1, img.rows, img.cols);
    T summ;

    for (int row = 0; row < img.rows; row++){
        for(int col = 0; col < img.cols; col++){
            summ = 0;
            for (int l = 0; l < img.L; l ++){
                summ += img(l, row, col);
            }
            img2d(0, row, col) = summ / img.L;
        }
    }

    return img2d;
}

/**
 * Calculate total variance of an image or stack
 *
 * @param img_stack to compute total variance of
 *
 * @return spatial variance of img
*/
template<typename T>
T total_var(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){
    T var = 0;
    ImageStack<T> img = avg_img(img_stack);
    T avg = avg_offset(img);
    const size_t size = img.size();

    // case where img_stack is a frame stack, so we calculate
    // total temporal variance, otherwise we just use the user
    // passed values for residual correction
    if (img_stack.L > 1){
        ttn_var = total_temp_var(img_stack);
        L = img_stack.L;
    }

    // variance calculation
    auto var_func = [&avg, &size](T accum,  T val){
        return accum + ((val - avg)*(val - avg) / (size));
    };

    // calculate variance
    var = accumulate(img.begin(), img.end(), 0.0, var_func);

    // remove residual temporal noise if passed by the user
    var -= (ttn_var / L);

    return var;

    }

/**
 * Calculate total temporal variance frame from image stack
 *
 * @param img_stack 2D/3D vector of pixel data
 *
 * @return total temporal variance frame
*/
template<typename T>
ImageStack<T> temp_var_from_stack(ImageStack<T>& img_stack){
    // TODO: add error handling if img_stack is a single frame

    T val = 0, avg = 0, m2 = 0;
    ImageStack<T> temp_img(1, img_stack.rows, img_stack.cols);

    for(int row = 0; row < img_stack.rows; row++){
        for(int col = 0; col < img_stack.cols; col++){
            val = 0, avg = 0, m2 = 0;
            for(int l = 0; l < img_stack.L; l++){
                // implementation of Welford"s algorithm for var
                // useful as we don't have to loop the same dims again
                T delta = img_stack(l, row, col) - avg;
                avg += delta / (l + 1);
                m2 += delta * (img_stack(l, row, col) - avg);
                val = m2 / (l);
            }
            temp_img(0, row, col) = val;
        }
    }

    return temp_img;

    }

/**
 * Calculate total temporal variance from image stack
 *
 * @param img_stack 2D/3D vector of pixel data
 *
 * @return total temporal variance from frame stack
*/
template<typename T>
T total_temp_var(ImageStack<T>& img_stack){

    ImageStack<T> temp_img = temp_var_from_stack(img_stack);
    T temp_var = avg_offset(temp_img);

    return temp_var;
}

/**
 * Calculate the approximate column variance
 * with residual temporal noise correction
 * EMVA 4.0 Eq. 41
 * this is not the exact solution
 *
 * @param img 2D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return approximate column variance
*/
template<typename T>
T col_var_cav(ImageStack<T>& img_stack, T ttn_var=0, int L=1){
    T col_var = 0;
    T summ, val;

    // get the average image if we are working with a frame stack
    ImageStack<T> img = avg_img(img_stack);
    T avg = avg_offset(img);

    // case where img_stack is a frame stack, so we calculate
    // total temporal variance, otherwise we just use the user
    // passed values for residual correction
    if (img_stack.L > 1){
        ttn_var = total_temp_var(img_stack);
        L = img_stack.L;
    }

    for(int col = 0; col < img.cols; col++){
        summ = 0;
        for(int row = 0; row < img.rows; row++)
        {
            // get the average column value
            summ += img(0, row, col);
        }
        val = summ / img.rows - avg;
        col_var += (val * val);
    }

    col_var /= img.cols;
    col_var -= ttn_var / (L * img.rows);

    return col_var;

    }

/**
 * Calculate the approximate row variance
 * EMVA 4.0 Eq. 41
 * this is not the exact solution
 *
 * @param img_stack 2D/3D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return approximate row variance
*/
template<typename T>
T row_var_rav(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){
    T row_var = 0;
    T summ, val;

    // get the average image if we are working with a frame stack
    ImageStack<T> img = avg_img(img_stack);
    std::cout << img_stack(0, 0, 0) << std::endl;
    T avg = avg_offset(img);

    // case where img_stack is a frame stack, so we calculate
    // total temporal variance, otherwise we just use the user
    // passed values for residual correction
    if (img_stack.L > 1){
        ttn_var = total_temp_var(img_stack);
        L = img_stack.L;
    }

    for(int row = 0; row < img.rows; row++){
        summ = 0;
        for(int col = 0; col < img.cols; col++)
        {
            // get the average column value
            summ += img(0, row, col);
        }
        val = summ / img.cols - avg;
        row_var += (val * val);
    }

        row_var /= img.rows;
        row_var -= ttn_var / (L * img.cols);

    return row_var;

    }

/**
 * exact solution for column variance
 * EMVA 4.0 Eq. 44
 *
 * @param rav_var approximate row variance
 * @param cav_var approximate col variance
 * @param tot_var total variance
 * @param M number of rows
 * @param N number of cols
 *
 * @return exact column variance
*/
template<typename T>
double _col_var(T rav_var, T cav_var, T tot_var, int M, int N){

    T col_var = ((M*N)-M)/(M*N-M-N)*cav_var - N/(M*N-M-N)*(tot_var - rav_var);

    return col_var;
}

/**
 * exact solution for row variance
 * EMVA 4.0 Eq. 44
 *
 * @param rav_var approximate row variance
 * @param cav_var approximate col variance
 * @param tot_var total variance
 * @param M number of rows
 * @param N number of cols
 *
 * @return exact row variance
*/
template<typename T>
double _row_var(T rav_var, T cav_var,
                T tot_var, int M, int N){

    T row_var = (M*N-N)/(M*N-M-N)*rav_var - M/(M*N-M-N)*(tot_var-cav_var);

    return row_var;
}

/**
 * exact solution for pixel variance
 * EMVA 4.0 Eq. 44
 *
 * @param rav_var approximate row variance
 * @param cav_var approximate col variance
 * @param tot_var total variance
 * @param M number of rows
 * @param N number of cols
 *
 * @return exact pixel variance
*/
template<typename T>
double _pix_var(T rav_var, T cav_var, T tot_var, int M, int N){

    T pix_var = (M*N)/(M*N-M-N)*(tot_var - cav_var - rav_var);

    return pix_var;
}

/**
 * exact solution for column variance
 * EMVA 4.0 Eq. 44
 *
 * @param img_stack 2D/3D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return exact column variance
*/
template<typename T>
double col_var(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){

    T rav_var = row_var_rav(img_stack, ttn_var, L);
    T cav_var = col_var_cav(img_stack, ttn_var, L);
    T tot_var = total_var(img_stack, ttn_var, L);

    return _col_var(rav_var, cav_var, tot_var, img_stack.rows, img_stack.cols);

}

/**
 * exact solution for row variance
 * EMVA 4.0 Eq. 44
 *
 * @param img_stack 2D/3D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return exact row variance
*/
template<typename T>
double row_var(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){

    T rav_var = row_var_rav(img_stack, ttn_var, L);
    T cav_var = col_var_cav(img_stack, ttn_var, L);
    T tot_var = total_var(img_stack, ttn_var, L);

    return _row_var(rav_var, cav_var, tot_var, img_stack.rows, img_stack.cols);
}

/**
 * exact solution for pixel variance
 * EMVA 4.0 Eq. 44
 *
 * @param img_stack 2D/3D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return exact pixel variance
*/
template<typename T>
double pix_var(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){

    T rav_var = row_var_rav(img_stack, ttn_var, L);
    T cav_var = col_var_cav(img_stack, ttn_var, L);
    T tot_var = total_var(img_stack, ttn_var, L);

    return _pix_var(rav_var, cav_var, tot_var, img_stack.rows, img_stack.cols);
}

/**
 * Get all component wise noise metrics
 * EMVA 4.0 Definitions
 *
 * @param img_stack 2D/3D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return result, all component wise noise metrics
*/
template<typename T>
map<string, T> agg_results(ImageStack<T>& img_stack, T ttn_var = 0, int L = 1){

    map<string, T> result;

    // TODO: simplify to result["metric"] = metric
    result.insert(make_pair("mean", avg_offset(img_stack)));
    result.insert(make_pair("total_temp_variance", total_temp_var(img_stack)));
    result.insert(make_pair("total_variance", total_var(img_stack, ttn_var, L)));
    result.insert(make_pair("pix_variance", pix_var(img_stack, ttn_var, L)));
    result.insert(make_pair("row_variance", row_var(img_stack, ttn_var, L)));
    result.insert(make_pair("col_variance", col_var(img_stack, ttn_var, L)));

    return result;
}

int main(){

    // basic example image
    ImageStack<double> img = vector<vector<vector<double>>> {{{0, 100, 100}, {0, 100, 100}}, {{100, 0, 0}, {100, 0, 0}}};

    // calculate all pixel metrics
    map<string, double> pix_metrics = agg_results(img);

    // view map result
    for (const auto &p : pix_metrics){
        std::cout << p.first << "\t" << p.second << std::endl;
    }

    }

