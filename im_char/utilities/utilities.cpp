#include <iostream>
using std::cout;
using std::endl;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
using std::make_tuple;

void print_array(vector<vector<int>>& img){
    int rows = img.size();
    int cols = img[0].size();

    for(int i = 0; i < cols; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            cout << img[i][j] << " ";
        }
        cout << endl;
    }
}

tuple<int, int> get_dims(vector<vector<float>>& img){
    /** get number of rows and columns */
    int rows = img.size();
    int cols = img[0].size();

    return make_tuple(rows, cols);
}
