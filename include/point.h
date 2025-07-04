#ifndef POINT_H
#define POINT_H
#pragma once
#include <boost/serialization/serialization.hpp>


struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/) {
        ar & x & y;
    }
};

#endif // POINT_H