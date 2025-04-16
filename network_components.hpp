#pragma once

#include "functions.hpp"
#include "spline.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <variant>

// #ifndef COMPONENTS_H
// #define COMPONENTS_H

class BaseNode;
class Edge;
struct NodeHasher
{
    template <class TNode> // Assumed that all "node" types have a hashId!!
    std::size_t operator()(TNode const &node) const
    {
        return std::hash<Integer>()(node.hashId_);
    }
};

struct NodeEquals
{
    template <class TNode>
    bool operator()(TNode const &nodeA, TNode const &nodeB) const
    {
        return nodeA.hashId_ == nodeB.hashId_; // WARNING: The onus is on the node creator to ensure there are no duplicates.
    }
};

struct UniqueHashIdEnforcer
{
    UniqueHashIdEnforcer() = delete;
    static Integer getNextHashId();

private:
public:
    static Integer nextHashId_;
};

// #endif // COMPONENTS_H

typedef std::vector<Scalar> EdgeTerminusValues;
typedef std::unordered_multimap<BaseNode, std::variant<Edge, Scalar>, NodeHasher, NodeEquals> Layer;

// For input layer only (no edges needed).
class BaseNode
{

public:
    BaseNode(Integer hashId);
    BaseNode(); // Default constructor required for passing type to equal_range.
    const Integer hashId_;
    Scalar output() const;

protected:
    Scalar output_;

private:
    virtual void computeOutput();
};

class Node : public BaseNode
{
    friend class Network;

public:
    Node(Integer edgeCount, Integer hashId);

private:
    void computeOutput();

    EdgeTerminusValues inputs_;
};

class Edge
{
    friend class Network;

public:
    Edge(Integer bSplineBasisOrder); // Depending on the order of our B-spline basis functions, our member variable Spline will look different.
    Scalar terminusValue() const;

private:
    void computeTerminusValue();

    Scalar originValue_;
    Scalar terminusValue_;
    Spline spline_; // Activation function parameterised as a spline.
};