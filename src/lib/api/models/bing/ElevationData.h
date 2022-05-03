//
// Created by tomas on 16.10.21.
//


#pragma once

#include <vector>
#include <memory>
namespace MapGenerator{
    class ElevationData {
    public:
        explicit ElevationData(const std::vector<double> &data, int resolution);
        ElevationData(int rows, int cols);
        void setAt(int row, int col, double value);
        void setAt(int rowFrom, int rowTo, int colFrom, int colTo, const std::vector<double> &newData);

        std::shared_ptr<std::vector<double>> getData(){
            return this->data;
        }
        void setScale(float scale){
            this->scale = scale;
        }
        void setNormalizedMax(double max){
            this->normalizedMax = max;
        }
        void setNormalizedMin(double min){
            this->normalizedMin = min;
        }

        [[nodiscard]] float getScale() const{
            return this->scale;
        }

        double getAt(int row, int col);
        [[nodiscard]] double getNormalizedMax() const{
            return this->normalizedMax;
        }
        [[nodiscard]] double getNormalizedMin() const{
            return this->normalizedMin;
        }
        double getAt(float row, float col);


    private:
        double normalizedMin;
        double normalizedMax;
        int rows;
        int cols;
        float scale;
        std::shared_ptr<std::vector<double>> data;

    };
}




