#ifndef RANDOM_GENERATOR_HPP
#define RANDOM_GENERATOR_HPP

#include <random>

class RandomFloatGenerator {
public:
    float generate(const float min, const float max) {
        return std::uniform_real_distribution<float>{min, max}(eng);
    }

private:
    std::mt19937 eng{std::random_device{}()};
};

#endif 