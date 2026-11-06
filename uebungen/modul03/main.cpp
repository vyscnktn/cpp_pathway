#include <print>

using std::println;

int main()
{

    int n = 784;
    double sum = 0.0;

   
    for (int i = 0;  i <= n; ++i)
    {
        double w = i / 100.0;
        double x = 1.0;

        sum += w * x;
    }
    println("i = {}", i);
    println("sum = {}", sum);
   
}