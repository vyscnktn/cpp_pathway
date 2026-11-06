#include <print> 
#include <cmath>

using std::println;

double relu(double x)
{
    if (x <= 0)
    {
        return 0.0;
    }
    return x;

}


int main()
{
    double x = 3.0;
    double y = -2.0;

    println("ReLU({}) = {}", x, relu(x));
    println("ReLU({}) = {}", y, relu(y));


    int n = 5;
    if (n == 0)
    {
        println("n sifir");
    }
    else
    {
        println("n sifir degil, n = {}", n);
    }

    double t = 0.1 + 0.2;
    if (std::abs(t - 0.3) < 1e-9)
    {
        println("esit");
    }
    else
    {
        println("esit degil");
    }
}