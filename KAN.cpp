#include "kan.hpp"
#include "network_components.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cstring>

/* */
KAN::KAN(fs::path const &path) : net_(std::unique_ptr<Network>(new Network()))
{
    retrieveAndSetDefaultHyperparameters(path);
    initialiseNetwork();
}

KAN::~KAN() = default;

KAN::Network::Network(std::vector<Integer> const &layerWidths, Integer bSplineBasisOrder)
{
    std::vector<Layer> layers;
    Layer layer;
    Integer prevLayerSize = 0;
    std::for_each(layerWidths.begin(), layerWidths.end(), [&](Integer n) {
        for (std::size_t i = 0; i < n; ++i)
        {
            // If we are at the input layer, add BaseNodes instead of Node, as there are no incoming edges associated with the input layer.
            if (i == 0)
            {
                BaseNode node(UniqueHashIdEnforcer::getNextHashId());
                std::pair<BaseNode, Integer> pair(node, 0); // There are no incoming edges associated with the input layer.
                layer.insert(pair);
            }
            else
            {
                Node node(prevLayerSize, UniqueHashIdEnforcer::getNextHashId());
                for (std::size_t j = 0; j < prevLayerSize * n; ++j)
                {
                    Edge edge(bSplineBasisOrder);
                    std::pair<BaseNode, Edge> pair(node, edge); // Must use copy insertion because node is being reused each time.
                    layer.insert(pair);
                }
            }
        }
        prevLayerSize = n;
        layers.push_back(layer); // Add the new layer to the network being constructed. // emplace back??
        // Reset the layer object!!!
        layer.clear();
    });
    layers_ = layers; // Assign the network as a member variable in KAN.
}

KAN::Network::Network() {}

class InvalidHyperparameterException : public std::exception
{
public:
    InvalidHyperparameterException(std::string hyperparam, std::string value)
    {
        m_hyperparam = hyperparam;
        m_value = value;
    }
    std::string getHyperparam() { return m_hyperparam; }
    std::string getValue() { return m_value; }
    virtual const char *what() const noexcept
    {
        return "not in the list of allowed hyperparameters for this network";
    }

private:
    std::string m_hyperparam;
    std::string m_value;
};

class MissingHyperparameterException : public std::exception
{
public:
    MissingHyperparameterException(HyperParameterList allowedHyperparams)
    {
        m_allowedHyperparams = allowedHyperparams;
    }
    HyperParameterList getAllowedHyperparams() { return m_allowedHyperparams; }
    virtual const char *what() const noexcept
    {
        return "one or more hyperparameters is missing from the config file";
    }

private:
    HyperParameterList m_allowedHyperparams;
};

class FileException : public std::exception
{
public:
    FileException(fs::path const &path)
    {
        m_path = path;
    }
    fs::path getPath() { return m_path; }
    virtual const char *what() const noexcept
    {
        return "failed to open file";
    }

private:
    fs::path m_path;
};

std::string removeCarriageReturn(std::string str)
{
    if (!str.empty() && str[str.size() - 1] == '\r')
    {
        return str.erase(str.size() - 1);
    }
    return str;
}

// template <typename T>
// auto convert(T &&t)
// {
//     if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value)
//     {
//         return std::forward<T>(t).c_str();
//     }
//     else
//     {
//         return std::forward<T>(t);
//     }
// }

// template <typename... Args>
// std::string formatStringInternal(const std::string &format, Args... args)
// {
//     int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for NULL terminator.
//     if (size_s <= 0)
//     {
//         throw std::runtime_error("Error during formatting.");
//     }
//     auto size = static_cast<size_t>(size_s); // Cast to signed type if error not thrown (snprintf returns negative if error occured).
//     auto buf = std::make_unique<char[]>(size);
//     std::snprintf(buf.get(), size, format.c_str(), args...);
//     return std::string(buf.get(), buf.get() + size - 1); // Without NULL terminator.
// }

// template <typename... Args>
// std::string formatString(std::string fmt, Args &&... args)
// {
//     return formatStringInternal(fmt, convert(std::forward<Args>(args))...);
// }

// Retrieve default hyperparameters from text file at specified file path.
void KAN::retrieveAndSetDefaultHyperparameters(fs::path const &path)
{
    std::string hyperparam;
    std::string value;
    std::ifstream file(path);
    try
    {
        if (file.is_open())
        {
            std::string line;
            std::string delim = "=";
            std::size_t pos = 0;

            HyperParameterList hyperparams;
            auto allowedHyperparams = hyperparameterList_;

            while (std::getline(file, line))
            {
                line = removeCarriageReturn(line); // On Windows, need to remove hidden \r from end of line.
                pos = line.find(delim);
                hyperparam = line.substr(0, pos);
                value = line.substr(pos + 1);
                hyperparams.insert(std::pair<std::string, std::string>(hyperparam, value));
                if (auto findit = allowedHyperparams.find(hyperparam); findit != allowedHyperparams.end())
                {
                    allowedHyperparams.erase(hyperparam); // Remove from the set of allowed hyperparameters for error checking later.
                }
                else if (hyperparam != "") // If empty we don't want to throw the following exception, just keep going.
                {
                    throw InvalidHyperparameterException(hyperparam, value);
                }
            }
            if (!allowedHyperparams.empty())
            {
                throw MissingHyperparameterException(allowedHyperparams);
            }
            hyperparameterList_ = hyperparams;
        }
        else
        {
            throw FileException(path);
        }
    }
    catch (FileException &ex)
    {
        std::cerr << "Error: " << ex.what() << " " << ex.getPath() << "\n";
    }
    catch (InvalidHyperparameterException &ex)
    {
        std::cerr << "Error: hyperparameter [" << ex.getHyperparam() << "] with value [" << ex.getValue() << "] " << ex.what() << ". \n";
    }
    catch (MissingHyperparameterException &ex)
    {
        std::cerr << "Error: " << ex.what() << " (";
        bool first = true;
        for (auto &pair : ex.getAllowedHyperparams())
        {
            if (!first)
            {
                std::cerr << ", ";
            }
            std::cerr << pair.first;
            first = false; // Done this way to prevent printing an additional comma.
        }
        std::cerr << ")\n";
    }
}

template <typename T>
std::vector<T> stringToVector(std::string str)
{
    std::string delim = ",";
    std::size_t pos = 0;
    std::vector<T> vec;
    char chars[] = "()[]{}"; // Need to remove any surrounding braces/brackets.
    try
    {
        for (std::size_t i = 0; i < strlen(chars); ++i)
        {
            str.erase(std::remove(str.begin(), str.end(), chars[i]), str.end());
        }
        while (str.size() != 0)
        {
            pos = str.find(delim);
            std::string substr = str.substr(0, pos);
            vec.push_back((T)std::stod(substr)); // Cast to a double (highest precision), then to type T - should handle double, float, int.
            if (pos == std::string::npos)
            {
                break;
            }
            str.erase(0, pos + delim.length());
        }
    }
    catch (...)
    {
        std::cerr << "There was an error converting the input string to a vector format. \n";
    }
    return vec;
}

void KAN::initialiseNetwork()
{
    // Set the newly created network (KAN::Network) in KAN class.
    auto layerWidths = getLayerWidthVector();
    Integer bSplineBasisOrder = std::stoi(hyperparameterList_.find("bSplineBasisOrder")->second);
    Network net(layerWidths, bSplineBasisOrder);
    *net_ = net;
}

// const HyperParameterList &KAN::getHyperparameterList() const
// {
//     return this->hyperparameterList;
// }

const std::vector<Integer> KAN::getLayerWidthVector() const
{
    std::vector<Integer> layerWidths = stringToVector<Integer>(hyperparameterList_.find("layerWidths")->second);
    return layerWidths;
}

// Expected to be a (relatively) slow operation (?)
const std::vector<std::pair<Scalar, std::vector<Scalar>>> KAN::Network::getNetworkValues() const
{
    std::vector<std::pair<Scalar, EdgeTerminusValues>> vec;
    EdgeTerminusValues edgeTerminusValues;
    auto it = vec.begin();
    for (Layer layer : layers_)
    {
        decltype(layer.equal_range(BaseNode())) range; // Pair of const iterators.
        for (auto i = layer.begin(); i != layer.end(); i = range.second)
        {
            // Get the range of the current BaseNode/Node key.
            range = layer.equal_range(i->first);
            auto j = range.first;
            for (; j != range.second; ++j)
            {
                // at j->first->hashId put j->second->
                if (!std::holds_alternative<Edge>(j->second))
                {
                    // Shouldn't need error checking because it should already hold an Edge (?)
                    //Edge edge = std::get<Edge> j;
                    edgeTerminusValues.push_back(std::get<Edge>(j->second).terminusValue());
                }
            }
            vec.insert(it + j->first.hashId_, std::pair<Scalar, std::vector<Scalar>>(j->first.output(), edgeTerminusValues));
            vec.clear();
        }
    }
    return vec;
}
