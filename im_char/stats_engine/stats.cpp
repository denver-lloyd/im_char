#include <iostream>
#include <vector>
#include <array>
#include <tuple>
#include <cmath>

void input(std::vector<std::vector<float>>& img){
    int rows = img.size();
    int cols = img[0].size();

    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            img[i][j] = 10;
        }
    }
}

void print_array(std::vector<std::vector<int>>& img){
    int rows = img.size();
    int cols = img[0].size();

    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            std::cout << img[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::tuple<int, int> get_dims(std::vector<std::vector<float>>& img){
    /** get number of rows and columns */
    int rows = img.size();
    int cols = img[0].size();

    return std::make_tuple(rows, cols);
}

double avg_offset(std::vector<std::vector<float>>& img){
    /** get the average offset of image */
    float avg, summ;
    int rows, cols;
    std::tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = std::get<0>(rows_cols);
    cols = std::get<1>(rows_cols);

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

double spatial_variance(std::vector<std::vector<float>>& img){
    /** calculate spatial variance */
    float spat_var, avg;
    int rows, cols;
    std::tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = std::get<0>(rows_cols);
    cols = std::get<1>(rows_cols);

    // get average
    avg = avg_offset(img);

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            spat_var += pow((img[row][col] - avg), 2);
        }
    }

    return spat_var /= (rows * cols);
}

/** calculate approximate column variance, this is not the exact solution */
double col_var_cav(std::vector<std::vector<float>>& img){
    float col_var=0, summ=0, avg=0;
    int rows, cols;
    std::tuple<int, int> rows_cols;

    // get dims
    rows_cols = get_dims(img);
    rows = std::get<0>(rows_cols);
    cols = std::get<1>(rows_cols);

    // create array for storing column averages
    std::vector<float> col_avgs(cols);

    // get average of each column
    for(int col = 0; col < cols; col++)
    {
        summ = 0;
        for(int row=0; row < rows; row++)
        {
            // get the average column value
            summ += img[col][row];
        }

        // add average column
        col_avgs[col] = summ / rows;
    }

    // calculate average of all columns
    summ = 0;
    for(int col=0; col < cols; col++){
        summ += col_avgs[col];
    }
    avg = summ / cols;

    // calculate column variance
    for(int col=0; col < cols; col++){
        col_var += pow((col_avgs[col] - avg), 2);
    }

    return col_var;
}

int main(){
    std::vector<std::vector<float>> img = {{0, 100}, {0, 100}};
    //input(img);
    std::cout << "average offset:" << avg_offset(img) << std::endl;
    std::cout << "spatial variance:" << spatial_variance(img) << std::endl;
    std::cout << "Col spatial variance:" << col_var_cav(img) << std::endl;
    }

