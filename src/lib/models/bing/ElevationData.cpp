//
// Created by tomas on 16.10.21.
//

#include <models/bing/ElevationData.h>

#include <utility>


namespace MapGenerator {
    ElevationData::ElevationData(const std::vector<double> &data) {
        this->data = std::make_shared<std::vector<double>>();
        this->data->reserve(data.size());
        for (double i: data) {
            this->data->push_back(i);
        }
    }

    ElevationData::ElevationData(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        this->data = std::make_shared<std::vector<double>>();
        auto size = rows * cols;
        this->data->reserve(size);
        for (auto i = 0; i < size; i++) {
            this->data->push_back(0);
        }
    }

    void ElevationData::setAt(int row, int col, double value) {
        auto position = row * this->rows + col;
        (*data)[position] = value;
    }

    void ElevationData::setAt(int rowFrom, int rowTo, int colFrom, int colTo, const std::vector<double> &newData) {
        int rowCount = rowTo - rowFrom;
        int colCount = colTo - colFrom;
        if(rowCount == colCount){
            setSameDimension(rowFrom, colFrom, rowCount, colCount, newData);
        }if(rowCount < colCount){
            setDifferentDimension(rowFrom, colFrom, rowCount, colCount, newData);
        }

    }

    void ElevationData::setSameDimension(int rowFrom, int colFrom,  int rowCount, int colCount ,const std::vector<double> &newData){
        int rIndex = rowFrom;
        for (int r = 0; r < rowCount; r++) {
            int cIndex = colFrom;
            for (int c = 0; c < colCount; c++) {
                int dataIndex  = (r * rowCount) + (colCount - c - 1);
                setAt(rIndex, cols - cIndex - 1, newData.at(dataIndex));
                cIndex++;
            }
            rIndex++;
        }
    }

    void ElevationData::setDifferentDimension(int rowFrom, int colFrom, int rowCount, int colCount, const std::vector<double> &newData) {
        int cIndex = colFrom + colCount - 1;

        for (int c = 0; c < colCount; c++) {
            int rIndex = rowFrom;
            for (int r = 0; r < rowCount; r++) {
                int dataIndex  = (r*rowCount) + c;
                setAt(rIndex, cols - cIndex - 1, newData.at(dataIndex));
                rIndex++;
            }
            cIndex--;
        }
    }

}
