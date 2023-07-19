#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
using std::get;
using std::make_tuple;
#include <cmath>
#include "../utilities/utilities.cpp"


/**
 * Get the average offset of an image
 *
 * @param img 2D vector of pixel data
 *
 * @return avg offset of 2D array
*/
double avg_offset(vector<vector<float>>& img){
    float avg;
    float summ;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // get the sum
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            summ += img[row][col];
        }
    }
    return summ / (rows * cols);
}

/**
 * Calculate spatial variance of an image
 *
 * @param img 2D vector of pixel data
 *
 * @return spatial variance of img
*/
double spatial_variance(vector<vector<float>>& img){
    float spat_var;
    float avg;
    int rows, cols;
    int val;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // get average
    avg = avg_offset(img);

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            val = (img[row][col] - avg);
            spat_var += (val * val);
        }
    }

    return spat_var /= (rows * cols);
}

/**
 * Calculate column averages of an image
 *
 * @param img 2D vector of pixel data
 *
 * @return column averages of image
*/
vector<float> col_avgs(vector<vector<float>>& img){
    float col_var = 0;
    float summ = 0;
    float avg = 0;
    int rows, cols;
    tuple<int, int> rows_cols;
    vector<float> col_avg(cols);

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // get average of each column
    for(int col = 0; col < cols; col++)
    {
        summ = 0;
        for(int row = 0; row < rows; row++)
        {
            // get the average column value
            summ += img[row][col];
        }

        // add average column
        col_avg[col] = summ / rows;
    }

    return col_avg;
}

/**
 * Calculate column average
 *
 * @param img 2D vector of pixel data
 *
 * @return column average of image
*/
double col_avg(vector<vector<float>>& img){
    float col_var=0;
    float summ=0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // create array for storing column averages
    vector<float> col_avgs_(cols);
    col_avgs_ = col_avgs(img);

    // calculate average of all columns
    for(int col = 0; col < cols; col++){
        summ += col_avgs_[col];
    }

    return summ / cols;

}

/**
 * Calculate the appropriate column variance,
 * this is not the exact solution
 *
 * @param img 2D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return approximate column variance
*/
double col_var_cav(vector<vector<float>>& img, float ttn_var = 0, int L = 1){
    float col_var = 0;
    float summ = 0;
    float avg = 0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // get column averages
    vector<float> col_avgs_(cols);
    col_avgs_ = col_avgs(img);

    // get average of all column
    avg = col_avg(img);

    // calculate column variance
    for(int col = 0; col < cols; col++){
        col_var += pow((col_avgs_[col] - avg), 2);
    }
    col_var /= cols;

    // remove residual temporal noise
    col_var -= ttn_var / (L * rows);

    return col_var;
}

/**
 * Calculate row averages
 *
 * @param img 2D vector of pixel data
 *
 * @return array of row averages
*/
vector<float> row_avgs(vector<vector<float>>& img){
    float row_var = 0;
    float summ = 0;
    float avg = 0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // create array for storing row averages
    vector<float> row_avg(cols);

    // get average of each row
    for(int row = 0; row < rows; row++)
    {
        summ = 0;
        for(int col=0; col < cols; col++)
        {
            // get the average row
            summ += img[row][col];
        }

        // add average row value
        row_avg[row] = summ / cols;
    }

    return row_avg;
}

/**
 * Calculate row average
 *
 * @param img 2D vector of pixel data
 *
 * @return row average
*/
double row_avg(vector<vector<float>>& img){
    float row_var = 0;
    float summ = 0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // create array for storing column averages
    vector<float> row_avgs_(cols);
    row_avgs_ = row_avgs(img);

    // calculate average of all rows
    for(int row = 0; row < rows; row++){
        summ += row_avgs_[row];
    }

    return summ / cols;

}

/**
 * Calculate the appropriate row variance,
 * this is not the exact solution
 *
 * @param img 2D vector of pixel data
 * @param ttn_var total temporal noise of img
 * @param L number of frames in frame stack
 *
 * @return approximate row variance
*/
double row_var_cav(vector<vector<float>>& img, float ttn_var = 0, int L = 1){
    float row_var=0, summ=0, avg=0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // get row averages
    vector<float> row_avgs_(cols);
    row_avgs_ = row_avgs(img);

    // get average of all rows
    avg = row_avg(img);

    // calculate row variance
    for(int row=0; row < rows; row++){
        row_var += pow((row_avgs_[row] - avg), 2);
    }
    row_var /= rows;

    // remove residual temporal noise
    row_var -= ttn_var / (L * cols);

    return row_var / rows;
}

int main(){
    //vector<vector<float>> img = {{100, 100}, {0, 0}};
    vector<vector<float>> img = {{0, 100}, {0, 100}};
    //input(img);
    cout << "average offset:" << avg_offset(img) << endl;
    cout << "spatial variance:" << spatial_variance(img) << endl;
    cout << "column average:" << col_avg(img) << endl;
    cout << "col spatial variance:" << col_var_cav(img) << endl;
    cout << "row spatial variance:" << row_var_cav(img) << endl;
    }

