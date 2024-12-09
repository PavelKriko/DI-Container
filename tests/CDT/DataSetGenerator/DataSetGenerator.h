#pragma once

#include"Csv.h"

#include <iostream>
#include <random>
#include <cstdint>
#include <chrono>
#include <thread>
#include <utility>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>


struct Point {
    double x;
    double y;
};

void turnPoint(Point& point_to_turn, double angle, Point turn_center);


class Rectangle {
public:
    Rectangle(double x, double y, double width, double height, double x_direction, double y_direction) : center({ x,y }), width(width), height(height), 
    x_direction(x_direction),y_direction(y_direction) {}

    std::vector<Point> getVertexes() const {

        if (abs(y_direction - 1) < std::numeric_limits<double>::epsilon()&& abs(x_direction)< std::numeric_limits<double>::epsilon()) {
            return { { center.x - width / 2,center.y + height / 2 },
                     { center.x + width / 2,center.y + height / 2 },
                     { center.x - width / 2,center.y - height / 2 },
                     { center.x + width / 2,center.y - height / 2 }
            };
        }
        else {
            Point A = { center.x - width / 2,center.y + height / 2 };
            Point B = { center.x + width / 2,center.y + height / 2 };
            Point C = { center.x - width / 2,center.y - height / 2 };
            Point D = { center.x + width / 2,center.y - height / 2 };

            const Point center = this->getCenter();

            const double angle = acos(y_direction);    //angle = arcos ( a*b/ (|a|* |b|) )
                                                      // where a = (0;1), b = (x_direction,y_direction);
            turnPoint(A, angle, center);
            turnPoint(B, angle, center);
            turnPoint(C, angle, center);
            turnPoint(D, angle, center);
            return { A,B,C,D };
        }

    }


    Point getCenter() const {
        return center;
    }
private:
    Point center;
    double width;
    double height;
    double x_direction; //direction is normalizer vector
    double y_direction;
};


class GaussGenerator
{
public:
    GaussGenerator(double mean, double stddev, std::uint32_t seed)
        : engine_(seed), distribution_(mean, stddev) {}

    GaussGenerator(double mean, double stddev)
        : distribution_(mean, stddev)
    {
        using namespace std;
        seed_seq seeds{
            (uint64_t)chrono::high_resolution_clock::now().time_since_epoch().count(),
            (uint64_t)chrono::system_clock::now().time_since_epoch().count(),
            (uint64_t)hash<thread::id>{}(this_thread::get_id()),
        };
        engine_.seed(seeds);
    }

    double operator()() { return distribution_(engine_); }
private:
    std::mt19937 engine_;
    std::normal_distribution<double> distribution_;
};




struct Projection {
    double min_x;
    double max_x;
    double min_y;
    double max_y;
};

Projection projectionsOxOy(const std::vector<Point>& points);


bool isCollision(const Rectangle& rec1, const Rectangle& rec2);

void createDataSet(const std::string& name, int sizeOfDataSet);

void normalize(double &x, double &y);