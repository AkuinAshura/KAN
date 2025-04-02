#pragma once

#include "functions.hpp"
#include "kan.hpp"

class KAN::Network
{
public:
    // Construct an appropriately sized network according to the desired layer widths.
    Network(std::vector<Integer> const &layerWidths);
};