#include "kan.hpp"

#include <iostream>

int main(void)
{
    std::cout << "Using default hyperparameters.\n";
    fs::path defaultHyperparameterFilePath = "network_default_hyperparameters.txt";
    //std::cout << defaultHyperparameterFilePath << std::endl;

    // Instantiate the network
    KAN kan = KAN(defaultHyperparameterFilePath);
    std::cout << "success" << std::endl;
    //HyperParameterList list = kan.getHyperparameterList();

    // // Report details of the constructed network.
    // std::cout << "Successfully constructed network with layers of width [";
    // for (const auto &hyperparam : kan.getHyperparameterList())
    // {
    //     std::cout << hyperparam.first << ", " << hyperparam.second << "\n";
    // }

    // std::vector<Integer> &layers = kan.getLayerWidths();
    // for (std::vector<Integer>::const_iterator i = layers.begin(); i != layers.end(); ++i)
    // {
    //     if (i != layers.end() - 1)
    //         std::cout << ', ';
    // }
    while (1)
        ;
    return 0;
}