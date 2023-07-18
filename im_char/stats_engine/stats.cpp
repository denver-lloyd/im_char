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

double avg_offset(vector<vector<float>>& img){
    /** get the average offset of image */
    float avg, summ;
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

double spatial_variance(vector<vector<float>>& img){
    /** calculate spatial variance */
    float spat_var, avg;
    int rows, cols, val;
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

/** calculate column averages */
vector<float> col_avgs(vector<vector<float>>& img){
    float col_var=0, summ=0, avg=0;
    int rows, cols;
    tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = get<0>(rows_cols);
    cols = get<1>(rows_cols);

    // create array for storing column averages
    vector<float> col_avg(cols);

    // get average of each column
    for(int col = 0; col < cols; col++)
    {
        summ = 0;
        for(int row=0; row < rows; row++)
        {
            // get the average column value
            summ += img[row][col];
        }

        // add average column
        col_avg[col] = summ / rows;
    }

    return col_avg;
}

/** calculate column average */
double col_avg(vector<vector<float>>& img){
    float col_var=0, summ=0;
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
    for(int col=0; col < cols; col++){
        summ += col_avgs_[col];
    }

    return summ / cols;

}

/** calculate approximate column variance, this is not the exact solution */
double col_var_cav(vector<vector<float>>& img, int L = 1){
    float col_var=0, summ=0, avg=0;
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
    for(int col=0; col < cols; col++){
        col_var += pow((col_avgs_[col] - avg), 2);
    }

    return col_var / cols;
}

int main(){
    vector<vector<float>> img = {{0, 100}, {0, 100}};
    //input(img);
    cout << "average offset:" << avg_offset(img) << endl;
    cout << "spatial variance:" << spatial_variance(img) << endl;
    cout << "column average:" << col_avg(img) << endl;
    cout << "col spatial variance:" << col_var_cav(img) << endl;
    }

