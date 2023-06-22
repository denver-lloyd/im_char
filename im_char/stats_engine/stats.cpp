#include <iostream>
#include <vector>
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

int main(){
    std::vector<std::vector<float>> img = {{0, 100}, {0, 100}};
    //input(img);
    std::cout << "average offset:" << avg_offset(img);
    std::cout << "spatial variance:" << spatial_variance(img);
    }

