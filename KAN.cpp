#include "kan.hpp"
#include "network.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <memory>

/* */
KAN::KAN(fs::path const &path)
{
    retrieveAndSetDefaultHyperparameters(path);
    initialiseNetwork();
}

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
        return "not in the list of allowed hyperparameters for this network. ";
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
        return "one or more hyperparameters is missing from the config file ";
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
        return "failed to open file ";
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
            auto allowedHyperparams = this->hyperparameterList;

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
            this->hyperparameterList = hyperparams;
        }
        else
        {
            throw FileException(path);
        }
    }
    catch (FileException &ex)
    {
        std::cerr << "Error: " << ex.what() << ex.getPath();
    }
    catch (InvalidHyperparameterException &ex)
    {
        std::cerr << "Error: hyperparameter [" << ex.getHyperparam() << "] with value [" << ex.getValue() << "] " << ex.what();
    }
    catch (MissingHyperparameterException &ex)
    {
        std::cerr << "Error: " << ex.what() << "(";
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
        std::cerr << ")";
    }
}

void KAN::initialiseNetwork()
{
    // Set the newly created network (KAN::Network) in KAN class. Use pointer to reduce overhead.
}

const HyperParameterList &KAN::getHyperparameterList() const
{
    return this->hyperparameterList;
}

const std::vector<Integer> &KAN::getLayerWidths() const
{
    return this->layerWidths;
}

int main(void)
{
    std::cout << "Using default hyperparameters.\n";
    fs::path defaultHyperparameterFilePath = "network_default_hyperparameters.txt";
    //std::cout << defaultHyperparameterFilePath << std::endl;

    // Instantiate the network
    KAN kan = KAN(defaultHyperparameterFilePath);
    HyperParameterList list = kan.getHyperparameterList();

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