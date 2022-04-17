//
// Created by tomas on 16.10.21.
//

#include "ElevationData.h"

#include <utility>

#include <iostream>

namespace MapGenerator {
    ElevationData::ElevationData(const std::vector<double> &data, int resolution) {
        this->rows = this->cols = resolution;
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
        auto position = row * this->rows + (this->cols - col - 1);
        (*data)[position] = value;
    }

    void ElevationData::setAt(int rowFrom, int rowTo, int colFrom, int colTo, const std::vector<double> &newData) {
        int rowCount = rowTo - rowFrom + 1;
        int colCount = colTo - colFrom + 1;
        int rIndex = rowFrom;

        for (int r = 0; r < rowCount; r++) {
            int cIndex = colFrom;
            for (int c = 0; c < colCount; c++) {
                int dataIndex = (r * rowCount) + (colCount - 1 - c);
                setAt(rIndex, cIndex, newData.at(dataIndex));
                cIndex++;
            }
            rIndex++;
        }

    }

    double ElevationData::getAt(int x, int y) {
        return (*data)[x * this->rows + y];
    }

    double ElevationData::getAt(float row, float col) {
        int lowerX = (int) row;
        int lowerY = (int) col;
        int upperX = lowerX + 1;
        int upperY = lowerY + 1;
        int lowerXIndex = lowerX * this->rows + lowerY;
        int upperXIndex = upperX * this->rows + lowerY;
        int lowerYIndex = lowerX * this->rows + upperY;
        int upperYIndex = upperX * this->rows + upperY;

        double lowerXValue = (*data)[lowerXIndex];
        double upperXValue = (*data)[upperXIndex];
        double lowerYValue = (*data)[lowerYIndex];
        double upperYValue = (*data)[upperYIndex];
        double max = std::max(std::max(lowerXValue, upperXValue), std::max(lowerYValue, upperYValue));
        return max;
        //Bilinear interpolation between the four surrounding points
        double x = row - lowerX;
        double y = col - lowerY;
        double x1 = 1 - x;
        double y1 = 1 - y;
        return x1 * y1 * lowerXValue + x * y1 * upperXValue + x1 * y * lowerYValue + x * y * upperYValue;
    }

}
