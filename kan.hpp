#include <iostream>
#include <vector>
#include <string>

/* KAN implementation class. Contains functionality necessary for building and interacting with a KAN network. */
class KAN
{
    typedef double Scalar;
    typedef std::vector<std::string> ArgsList; // Argument list used when building the network.

public:
    KAN(ArgsList args);

    /* Functions */
    void trainNetwork();
    void testNetwork();
    void saveModelState();
    // etc.

private:
    void resolveArguments(ArgsList args);
    class Net
    {
    public:
        Net();
        Net() {}
    };
};