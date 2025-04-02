#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <algorithm>
#include <unordered_map>
#include <memory>

typedef std::unordered_map<std::string, std::string> HyperParameterList; // List of hyperparameters to be used when building the network.

typedef double Scalar;
typedef int Integer;
typedef std::vector<Scalar> layer;

namespace fs = std::filesystem;

/* KAN implementation class. Contains functionality for building and interacting with a KAN network. */
class KAN
{

public:
    KAN(fs::path const &path);

    /* Functions */
    void trainNetwork();
    void testNetwork();
    void saveModelState();
    // etc.

    // Getters for retrieving information about the network.
    std::string getSaveLocation() const;
    std::vector<layer> getNetworkState() const;
    const HyperParameterList &getHyperparameterList() const;
    const std::vector<Integer> &getLayerWidths() const;

    // Setters.
    void setSaveLocation(fs::path const &path);

private:
    class Network;
    void retrieveAndSetDefaultHyperparameters(fs::path const &path);
    void initialiseNetwork();
    std::vector<Integer> layerWidths;
    HyperParameterList hyperparameterList{};
};

// template <typename... Args>
// auto g(Args &&...args) -> decltype(f(std::forward<Args>(args)...))
// {
//     return f(std::forward<Args>(args)...);
// }