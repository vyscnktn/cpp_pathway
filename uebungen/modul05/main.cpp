#include <print>
#include <vector>

using std::println;
using std::vector;

double relu(double x)
{
    if (x <= 0)
    {
        return 0.0;
    }
    return x;
}

void clear_all(vector<double>& v)
{
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        v[i] = 0.0;
    }

}

double neuron(const vector<double>& w,const vector<double>& x)
{
    if (w.size() != x.size())
    {
        println("Hata: w {} elemanli, x {} elemanli", w.size(), x.size());
        return 0.0;
    }
    
    double s = 0.0;
    for (std::size_t i = 0; i < w.size(); ++i)
    {
        s += w[i] * x.at(i);
    }
    return relu(s);

}

int main()
{
    vector<double> w = {0.5, -1.2, 0.8};
    vector<double> x = {1.0, 2.0};

    println("Neuron = {}", neuron(w, x));
    clear_all(w);
    clear_all(x);
    println("After clear_all: w = {}, x = {}", w, x);
    return 0;
}