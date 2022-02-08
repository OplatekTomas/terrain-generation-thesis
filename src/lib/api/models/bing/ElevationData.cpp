//
// Created by tomas on 16.10.21.
//

#include "ElevationData.h"

#include <utility>

#include <iostream>

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

}
