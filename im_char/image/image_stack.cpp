#include <vector>
using std::vector;

/**
 * Create an ImageStack instance to be used for
 * all calculations. This supports multiple input types
 * and maps the image stack to a 1D vector so there are
 * no cache misses and mean/var can be calculated quickly
 *
 * This is a nice idea currently, but needs to be further thought
 * through and performance further evaluated, for example when
 * a 3D vector is passed all values are currently looped to map
 * to the 1D array
 *
 * @param L number of frames
 * @param rows number of rows
 * @param cols number of columns
 * or
 * @param array2d 2D vector with dims=(rows, cols)
 * or
 * @param array3d 3D vector with dims=(L, rows, cols)
*/
template<typename T>
struct ImageStack {
    int L, rows, cols;
    vector<T> data;

    // default constructor
    ImageStack(int L, int rows, int cols)
    : L(L), rows(rows), cols(cols), data(L*rows*cols){};

    // 2D vector constructor
    ImageStack(vector<vector<T>> array2d)
    : data(array2d.size() * array2d[0].size()){
        L = 1;
        rows = array2d.size();
        cols = array2d[0].size();
        for (int row = 0; row < rows; row++){
            for (int col = 0; col < cols; col++){
                data[(L*rows + row)*cols + col] = array2d[row][col];
            }
        }
    }

    // 3D vector constructor
    ImageStack(vector<vector<vector<T>>> array3d)
    : data(array3d.size() * array3d[0].size() * array3d[0][0].size()){
        L = array3d.size();
        rows = array3d[0].size();
        cols = array3d[0][0].size();
        for (int l = 0; l < L; l++){
            for (int row = 0; row < rows; row++){
                for (int col = 0; col < cols; col++){
                    data[(l*rows + row)*cols + col] = array3d[l][row][col];
                }
            }
        }
    }

    T& operator()(int l, int row, int col){
        return data[(l*rows + row)*cols + col];
    }

    int size(){
        if (L != 0){
            return L*rows*cols;
        }
        else{
            return rows*cols;
        }
    }

    auto begin(){
        return data.begin();
    }

    auto end(){
        return data.end();
    }

};