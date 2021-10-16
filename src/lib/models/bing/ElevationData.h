//
// Created by tomas on 16.10.21.
//


#pragma once

#include <vector>
#include <memory>
namespace MapGenerator{
    class ElevationData {
    public:
        explicit ElevationData(const std::vector<double> &data);
        ElevationData(int rows, int cols);
        void setAt(int row, int col, double value);
        void setAt(int rowFrom, int rowTo, int colFrom, int colTo, const std::vector<double> &newData);

        std::shared_ptr<std::vector<double>> getData(){
            return this->data;
        }

    private:
        int rows;
        int cols;
        std::shared_ptr<std::vector<double>> data;

        void setSameDimension(int rowFrom, int colFrom, int rowCount, int colCount, const std::vector<double> &newData);
        void setDifferentDimension(int rowFrom, int colFrom, int rowCount, int colCount, const std::vector<double> &newData);

    };
}




