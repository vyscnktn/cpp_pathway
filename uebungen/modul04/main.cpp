#include <print>

using std::println;

double relu(double x)
{
    if (x <= 0)
    {
        return 0.0;
    }
    return x;
}

double neuron(double scale, int n)
{
    double s = 0.0;
    for (int i=0; i < n; ++i)
    {
        s += (scale * i) * 1.0;
    }
    return relu(s);
}

int main()
{
    println("Neuron 1 = {}", neuron(0.01, 784));
    println("Neuron 2 = {}", neuron(0.02, 784));
    println("Neuron 3 = {}", neuron(-0.03, 784));
    return 0;

}

