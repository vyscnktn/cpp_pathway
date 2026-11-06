#include <print>

using std::println;

int main()
{
    int a = 7;
    int b = 2;
    double o1 = (a + b) / 2.0;
    double o2 = static_cast<double>(a + b) / 2;
    double o3 = static_cast<double>((a + b)/ 2);
    println("Ortalama: {}", o1);
    println("Ortalama: {}", o2);
    println("Ortalama: {}", o3);
}