#pragma once
#include "iostream"

struct MyStruct
{
    int myInt = 0;
};
struct vector2 {
    float x, y;

    vector2() : x(0), y(0) {}

    vector2(float newX, float newY) : x(newX), y(newY) {} // same as x=newx y=newy

    vector2 operator+(const vector2& additionVec) const {
        return vector2(x + additionVec.x, y + additionVec.y);
    }

    vector2 operator+=(const vector2& additionVec) {
        x += additionVec.x;
        y += additionVec.y;
        return *this;
    }

    vector2 operator-(const vector2& subtractionVec) const {
        return vector2(x - subtractionVec.x, y - subtractionVec.y);
    }

    vector2 operator-=(const vector2& subtractionVec) {
        x -= subtractionVec.x;
        y -= subtractionVec.y;
        return *this;
    }

    vector2 operator*(const vector2& multVec) const {
        return vector2(x * multVec.x, y * multVec.y);
    }

    vector2 operator*=(const vector2& multVec) {
        x *= multVec.x;
        y *= multVec.y;
        return *this;
    }

    vector2 operator/(const vector2& divisionVec) const {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2(0, 0);
        return vector2(x / divisionVec.x, y / divisionVec.y);
    }

    vector2 operator/=(const vector2& divisionVec) {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2(0, 0);
        x /= divisionVec.x;
        y /= divisionVec.y;
        return *this;
    }

    vector2 operator=(const vector2& setVec) {
        x = setVec.x;
        y = setVec.y;
        return *this;
    }

    bool operator==(const vector2& compareVec) const {
        return x == compareVec.x && y == compareVec.y;
    }

    bool operator<(const vector2& compareVec) const {
        return x < compareVec.x && y < compareVec.y;
    }

    bool operator<=(const vector2& compareVec) const {
        return x <= compareVec.x && y <= compareVec.y;
    }

    bool operator>(const vector2& compareVec) const {
        return x > compareVec.x && y > compareVec.y;
    }

    bool operator>=(const vector2& compareVec) const {
        return x >= compareVec.x && y <= compareVec.y;
    }
};

std::ostream& operator<<(std::ostream& os, const vector2& vecToSend) {
    os << vecToSend.x << ", " << vecToSend.y;
    return os;
}

struct vector2Int {
    int x, y;

    vector2Int() : x(0), y(0) {}

    vector2Int(int newX, int newY) : x(newX), y(newY) {} // same as x=newx y=newy

    vector2Int operator+(const vector2Int& additionVec) const {
        return vector2Int(x + additionVec.x, y + additionVec.y);
    }

    vector2Int operator+=(const vector2Int& additionVec) {
        x += additionVec.x;
        y += additionVec.y;
        return *this;
    }

    vector2Int operator-(const vector2Int& subtractionVec) const {
        return vector2Int(x - subtractionVec.x, y - subtractionVec.y);
    }

    vector2Int operator-=(const vector2Int& subtractionVec) {
        x -= subtractionVec.x;
        y -= subtractionVec.y;
        return *this;
    }

    vector2Int operator*(const vector2Int& multVec) const {
        return vector2Int(x * multVec.x, y * multVec.y);
    }

    vector2Int operator*=(const vector2Int& multVec) {
        x *= multVec.x;
        y *= multVec.y;
        return *this;
    }

    vector2Int operator/(const vector2Int& divisionVec) const {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2Int(0, 0);
        return vector2Int(x / divisionVec.x, y / divisionVec.y);
    }

    vector2Int operator/=(const vector2Int& divisionVec) {
        if (divisionVec.x == 0 || divisionVec.y == 0) return vector2Int(0, 0);
        x /= divisionVec.x;
        y /= divisionVec.y;
        return *this;
    }

    vector2Int operator=(const vector2Int& setVec) {
        x = setVec.x;
        y = setVec.y;
        return *this;
    }

    bool operator==(const vector2Int& compareVec) const {
        return x == compareVec.x && y == compareVec.y;
    }

    bool operator<(const vector2Int& compareVec) const {
        return x < compareVec.x && y < compareVec.y;
    }

    bool operator<=(const vector2Int& compareVec) const {
        return x <= compareVec.x && y <= compareVec.y;
    }

    bool operator>(const vector2Int& compareVec) const {
        return x > compareVec.x && y > compareVec.y;
    }

    bool operator>=(const vector2Int& compareVec) const {
        return x >= compareVec.x && y <= compareVec.y;
    }
};

std::ostream& operator<<(std::ostream& os, const vector2Int& vecToSend) {
    os << vecToSend.x << ", " << vecToSend.y;
    return os;
}