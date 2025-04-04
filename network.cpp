#include "kan.hpp"
#include "network.hpp"
#include "functions.hpp"

KAN::Network::Network(std::vector<Integer> const &layerWidths, Integer bSplineBasisOrder)
{
    std::vector<Layer> net;
    Layer layer;
    Integer prevLayerSize = 0;
    std::for_each(layerWidths.begin(), layerWidths.end(), [&](Integer n) {
        for (std::size_t i = 0; i < n; ++i)
        {
            // If we are at the input layer, add BaseNodes instead of Node, as there are no incoming edges associated with the input layer.
            if (i == 0)
            {
                BaseNode node;
                std::pair<BaseNode, Edge> pair{node, NULL}; // There are no incoming edges associated with the input layer.
                layer.insert(pair);
            }
            else
            {
                Node node(prevLayerSize);
                for (std::size_t j = 0; j < prevLayerSize * n; ++j)
                {
                    Edge edge(bSplineBasisOrder);
                    std::pair<BaseNode, Edge> pair{node, edge}; // Must use copy insertion because node is being reused each time.
                    layer.insert(pair);
                }
            }
        }
        prevLayerSize = n;
        net.push_back(layer); // Add the new layer to the network being constructed.
    });
    this->net = net; // Assign the network as a member variable in KAN.
}

BaseNode::BaseNode() : output(0.0){

                       };

Node::Node(Integer edgeCount) : BaseNode(), inputs(edgeCount){

                                            };

Edge::Edge(Integer bSplineBasisOrder) : originValue(0.0), terminusValue(0.0)
{
    spline = Spline(bSplineBasisOrder);
};

void BaseNode::computeOutput()
{
    // Do nothing.
}

void Node::computeOutput()
{
    output = sumScalars(inputs);
}

void Edge::computeTerminusValue()
{
    // operate on originValue using the spline stored in the edge object...
    // terminusValue = basisFunction::silu() + spline op...
    // math
}
