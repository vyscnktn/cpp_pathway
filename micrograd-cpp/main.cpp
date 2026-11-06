#include <functional>
#include <memory>
#include <print>
#include <utility>
#include <vector>
using std::function;
using std::make_shared;
using std::println;
using std::shared_ptr;
using std::vector;

struct Value;
using Ptr = shared_ptr<Value>;

struct Value
{
  double data = 0.0;
  double grad = 0.0;
  vector<Ptr> prev;
  char op = ' ';
  function<void()> backward_fn = [] {};

  Value(double d) : data(d) {}
  Value(double d, vector<Ptr> children, char o)
      : data(d), prev(std::move(children)), op(o) {}
};

Ptr make(double d)
{
  return make_shared<Value>(d);
}

Ptr operator+(const Ptr& a, const Ptr& b)
{
  auto out = make_shared<Value>(a->data + b->data, vector<Ptr>{a, b}, '+');
  Value* o = out.get();
  out->backward_fn = [a, b, o]()
  {
    a->grad += 1.0 * o->grad;
    b->grad += 1.0 * o->grad;
  };
  return out;
}

Ptr operator*(const Ptr& a, const Ptr& b)
{
  auto out = make_shared<Value>(a->data * b->data, vector<Ptr>{a, b}, '*');
  Value* o = out.get();
  out->backward_fn = [a, b, o]()
  {
    a->grad += b->data * o->grad;
    b->grad += a->data * o->grad;
  };
  return out;
}

int main()
{
  auto a = make(2.0);
  auto b = make(3.0);
  auto c = a * b;
  auto d = c + a;

  d->grad = 1.0;
  d->backward_fn();
  c->backward_fn();

  println("d = {}", d->data);
  println("a.grad = {}", a->grad);
  println("b.grad = {}", b->grad);
  println("c.grad = {}", c->grad);

  return 0;
}