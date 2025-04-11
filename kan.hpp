#pragma once

#include "network_components.hpp"
#include "functions.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

typedef std::unordered_map<std::string, std::string> HyperParameterList; // List of hyperparameters to be used when building the network.

namespace fs = std::filesystem;

/* KAN implementation class. Contains functionality for building and interacting with a KAN network. */
class KAN
{

public:
    KAN(fs::path const &path);
    ~KAN();

    /* Functions */
    void trainNetwork();
    void testNetwork();
    void saveModelState();
    // etc.

    // Getters for retrieving information about the network.
    std::string getSaveLocation() const;
    //std::vector<Layer> getNetworkState() const;
    //const HyperParameterList &getHyperparameterList() const;
    const std::vector<Integer> getLayerWidthVector() const;

    // Setters.
    void setSaveLocation(fs::path const &path);

private:
    class Network;
    std::unique_ptr<Network> net_; // Reference works because it doesn't require complete definition of network... (?)
    void retrieveAndSetDefaultHyperparameters(fs::path const &path);
    void initialiseNetwork();
    std::vector<Integer> layerWidths_;
    HyperParameterList hyperparameterList_{{"filename", ""}, {"saveLocation", ""}, {"datasetLocation", ""}, {"layerWidths", ""}, {"bSplineBasisOrder", ""}};
};

class KAN::Network
{

public:
    // Construct an appropriately sized network according to the desired layer widths.
    Network(std::vector<Integer> const &layerWidths, Integer bSplineBasisOrder);
    Network(); // Default constructor needed during pointer creation.

private:
    // Scalar &getNodeOutput(Node node) const { return node.output; }
    // Scalar &getEdgeTerminusValue(Edge edge) const { return edge.terminusValue; }

    void setEdgeSpline();

    std::vector<Layer> layers_;
};
