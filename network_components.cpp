#include "kan.hpp"
#include "network_components.hpp"
#include "functions.hpp"

BaseNode::BaseNode(Integer hashId) : output_(0.0){

                                     };

Node::Node(Integer edgeCount, Integer hashId) : BaseNode(hashId), inputs_(edgeCount){

                                                                  };

Edge::Edge(Integer bSplineBasisOrder) : originValue_(0.0), terminusValue_(0.0)
{
    spline_ = Spline(bSplineBasisOrder);
};

void BaseNode::computeOutput()
{
    // Do nothing.
}

void Node::computeOutput()
{
    output_ = sumScalars(inputs_);
}

void Edge::computeTerminusValue()
{
    // operate on originValue using the spline stored in the edge object...
    // terminusValue = basisFunction::silu() + spline op...
    // math
}
