#include "kan.hpp"
#include "network.hpp"

/* */
KAN::KAN(fs::path const &path)
{
    retrieveAndSetDefaultHyperparameters(path);
    initialiseNetwork();
}

class InvalidHyperparameterException : public std::exception
{
public:
    InvalidHyperparameterException(std::string hyperparam, std::string value) {
        m_hyperparam = hyperparam;
        m_value = value;
    }
    std::string getErrorHyperparam() const { return m_hyperparam; }
    std::string getErrorValue() const { return m_value; }
    virtual const char *what() const noexcept
    {
        return "Not in the list of allowed hyperparameters for this network. ";
    }
private:
    std::string m_hyperparam;
    std::string m_value;
};

class MissingHyperparameterException : public std::exception
{
public:
    virtual const char *what() const noexcept
    {
        return "One or more hyperparameters is missing from the config file. ";
    }
};

// template<typename T>
// auto convert(T&& t) {
//     if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value) {
//         return std::forward<T>(t).c_str();
//     } else {
//         return std::forward<T>(t);
//     }
// }

// template<typename ... Args>
// std::string formatStringInternal(const std::string &format, Args ... args) {
//     int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for NULL terminator.
//     if (size_s <= 0) {
//         throw std::runtime_error("Error during formatting.");
//     }
//     auto size = static_cast<size_t>(size_s); // Cast to signed type if error not thrown (snprintf returns negative if error occured).
//     auto buf = std::make_unique<char[]>(size);
//     std::snprintf(buf.get(), size, format.c_str(), args ...);
//     return std::string(buf.get(), buf.get() + size - 1); // Without NULL terminator.
// }

// template<typename ... Args>
// std::string formatString(std::string fmt, Args&& ... args) {
//     return formatStringInternal(fmt, convert(std::forward<Args>(args))...);
// }

// Retrieve default hyperparameters from text file at specified file path.
void KAN::retrieveAndSetDefaultHyperparameters(fs::path const &path)
{
    std::string hyperparam;
    std::string value;
    std::string test = "test";
    try
    {
        std::ifstream file(path);
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        if (file.is_open())
        {
            std::string line;
            std::string delim = "=";
            std::size_t pos = 0;

            HyperParameterList hyperparams;
            auto allowedHyperparams = this->hyperparameterList;

            while (std::getline(file, line))
            {
                pos = line.find(delim);
                hyperparam = line.substr(0, pos);
                hyperparams.insert(std::pair<std::string, std::string> (hyperparam, value));
                value = line.substr(pos + 1);
                //std::cout << formatString(" %s %s ", hyperparam, value) << std::endl;
                if (auto findit = allowedHyperparams.find(hyperparam); findit != allowedHyperparams.end())
                {
                    allowedHyperparams.erase(hyperparam); // Remove from the set of allowed hyperparameters for error checking later.
                }
                else
                {
                    throw InvalidHyperparameterException(hyperparam, value);
                }
            }
            if (!allowedHyperparams.empty())
            {
                throw MissingHyperparameterException();
            }
            this->hyperparameterList = hyperparams;
        }
    }
    catch (std::ifstream::failure const &ex)
    {
        std::cerr << "Failed to read in hyperparameter values from " << path << std::endl;
    }
    catch (InvalidHyperparameterException &ex)
    {
        std::cerr << ex.getErrorHyperparam() << ex.getErrorValue() << ex.what();
        
    }
    catch (MissingHyperparameterException &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void KAN::initialiseNetwork()
{
    //
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