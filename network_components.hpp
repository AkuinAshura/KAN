#pragma once

#include "functions.hpp"
#include "spline.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>

typedef std::vector<Scalar> EdgeTerminusValues;

class BaseNode;
class Edge;

struct NodeHasher
{
    template <class TNode> // Assumed that all "node" types have a hashId!!
    std::size_t operator()(TNode const &node) const
    {
        return std::hash<TNode>()(node.hashId_);
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

typedef std::unordered_multimap<BaseNode, Edge, NodeHasher, NodeEquals> Layer;

// struct UniqueHashIdEnforcer
// {
//     static Integer getNextHashId() { return ++nextHashId_; }
//     static Integer nextHashId_;
//     UniqueHashIdEnforcer() = delete;
// };
//Integer UniqueHashIdEnforcer::nextHashId_ = 0;

// For input layer only (no edges needed).
class BaseNode
{

public:
    BaseNode(Integer hashId);
    Integer hashId_;

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

private:
    void computeTerminusValue();

    Scalar originValue_;
    Scalar terminusValue_;
    Spline spline_; // Activation function parameterised as a spline.
};