#include "network_components.hpp"
#include "functions.hpp"

Integer UniqueHashIdEnforcer::nextHashId_ = 0;
Integer UniqueHashIdEnforcer::getNextHashId() { return ++nextHashId_; }

BaseNode::BaseNode(Integer hashId) : hashId_(hashId), output_(0.0){};

BaseNode::BaseNode() : hashId_(0){};

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

Scalar Edge::terminusValue() const
{
    return terminusValue_;
}

Scalar BaseNode::output() const
{
    return output_;
}
