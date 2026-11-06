#include <print>
#include <vector>


using std::println;
using std::vector;


double& biggest(vector<double>& w)
{
    std::size_t k = 0;
    for (std::size_t i = 1; i < w.size(); i++)
    {
        if (w[i] > w[k])
        {
            k = i;
        }
    }
    return w[k];
}

int main()
{
    vector<double> empty = {};
    biggest(empty) = 0.0;
    println("{}, {}", biggest(empty), empty);
}