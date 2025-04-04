#pragma once

#include "functions.hpp"
#include "spline.hpp"

#include <vector>
#include <unordered_map>

typedef std::vector<Scalar> EdgeTerminusValues;
typedef std::unordered_multimap<BaseNode, Edge> Layer;

//typedef std::vector<>

class KAN::Network
{

public:
    // Construct an appropriately sized network according to the desired layer widths.
    Network(std::vector<Integer> const &layerWidths, Integer bSplineBasisOrder);

private:
    Scalar &getNodeOutput(Node node) const { return node.output; }
    Scalar &getEdgeTerminusValue(Edge edge) const { return edge.terminusValue; }

    void setEdgeSpline();

    std::vector<Layer> net;
};

// For input layer only (no edges needed).
class BaseNode
{

public:
    BaseNode();

protected:
    Scalar output;

private:
    virtual void computeOutput();
};

class Node : public BaseNode
{
    friend class KAN::Network;

public:
    Node(Integer edgeCount);

private:
    void computeOutput();

    EdgeTerminusValues inputs;
};

class Edge
{
    friend class KAN::Network;

public:
    Edge(Integer bSplineBasisOrder); // Depending on the order of our B-spline basis functions, our member variable Spline will look different.

private:
    void computeTerminusValue();

    Scalar originValue;
    Scalar terminusValue;
    Spline spline; // Activation function parameterised as a spline.
};