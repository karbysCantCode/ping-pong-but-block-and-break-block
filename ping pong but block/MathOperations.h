#pragma once

std::random_device rd;
std::mt19937 eng(rd());

double randomDouble(double min, double max) {
    std::uniform_real_distribution<> distr(min, max);
    return distr(eng);
}

float randomFloat(float min, float max) {
    std::uniform_real_distribution<> distr(min, max);
    return static_cast<float>(distr(eng));
}

int randomInt(int min, int max) {
    std::uniform_int_distribution<> distr(min, max);
    return distr(eng);
}

float mathAbs(float a) {
    return (a < 0) ? -a : a;
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int mathMinInt(int a, int b) {
    return (a < b) ? a : b;
}

// Min function for doubles
double mathMinDouble(double a, double b) {
    return (a < b) ? a : b;
}

// Min function for floats
float mathMinFloat(float a, float b) {
    return (a < b) ? a : b;
}

// Max function for integers
int mathMaxInt(int a, int b) {
    return (a > b) ? a : b;
}

// Max function for doubles
double mathMaxDouble(double a, double b) {
    return (a > b) ? a : b;
}

// Max function for floats
float mathMaxFloat(float a, float b) {
    return (a > b) ? a : b;
}

int findHCF(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


std::string abbreviateNumber(const int number) {
    if (number < 1000) return std::to_string(number);
    if (number >= 1000 && number < 1000000) return std::to_string(number / 1000) + 'K';
    if (number >= 1000000 && number < 1000000000) return std::to_string(number / 1000000) + 'M';
    if (number >= 1000000000 && number < 1000000000000) return std::to_string(number / 1000000000) + 'B';
    if (number >= 1000000000000 && number < 1000000000000000) return std::to_string(number / 1000000000000) + 'T';
    return std::to_string(number);
}