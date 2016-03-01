#include <accumulate.h>

#define TESTINATOR_MAIN
#include <testinator.h>

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

DEF_PROPERTY(SimpleAccumulate, Accumulate, const vector<unsigned int>& v)
{
  auto x = accumulate(v.cbegin(), v.cend(), 0, std::plus<>());
  auto y = acc::accumulate(v.cbegin(), v.cend(), 0, std::plus<>());
  return x == y;
}

DEF_PROPERTY(MinElement, Accumulate, const vector<unsigned int>& v)
{
  auto x = min_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::min_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MinElementValue, Accumulate, const vector<unsigned int>& v)
{
  auto x = min_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::min_element_value(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MaxElement, Accumulate, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MaxElementValue, Accumulate, const vector<unsigned int>& v)
{
  auto x = max_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::max_element_value(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) || (*x == y);
}

DEF_PROPERTY(MinMaxElement, Accumulate, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element(v.cbegin(), v.cend(), std::less<>());
  return x == y;
}

DEF_PROPERTY(MinMaxElementValue, Accumulate, const vector<unsigned int>& v)
{
  auto x = minmax_element(v.cbegin(), v.cend(), std::less<>());
  auto y = acc::minmax_element_value(v.cbegin(), v.cend(), std::less<>());
  return (v.empty() && y == std::experimental::nullopt) ||
    (*x.first == y->first && *x.second == y->second);
}

DEF_PROPERTY(Count, Accumulate, const vector<unsigned int>& v)
{
  auto i = v.empty() ? 0 : v[0];
  auto x = count(v.cbegin(), v.cend(), i);
  auto y = acc::count(v.cbegin(), v.cend(), i);
  return x == y;
}

DEF_PROPERTY(CountIf, Accumulate, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i%1) == 0; };
  auto x = count_if(v.cbegin(), v.cend(), even);
  auto y = acc::count_if(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(Copy, Accumulate, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = copy(v.cbegin(), v.cend(), outx.data());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy(v.cbegin(), v.cend(), outy.data());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(CopyIf, Accumulate, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i%1) == 0; };

  vector<unsigned int> outx(v.size(), 0);
  auto x = copy_if(v.cbegin(), v.cend(), outx.data(), even);

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::copy_if(v.cbegin(), v.cend(), outy.data(), even);

  auto sx = x - outx.data();
  auto sy = y - outy.data();

  return sx == sy
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(Transform, Accumulate, const vector<unsigned int>& v)
{
  auto add1 = [] (unsigned int i) { return i+1; };

  vector<unsigned int> outx(v.size(), 0);
  auto x = transform(v.cbegin(), v.cend(), outx.data(), add1);

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::transform(v.cbegin(), v.cend(), outy.data(), add1);

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(Transform2, Accumulate, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = transform(v.cbegin(), v.cend(), v.cbegin(), outx.data(), plus<>());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::transform(v.cbegin(), v.cend(), v.cbegin(), outy.data(), plus<>());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}

DEF_PROPERTY(Find, Accumulate, const vector<unsigned int>& v)
{
  unsigned int e = 0;
  if (v.size() > 0) e = v[v.size()/2];

  auto x = find(v.cbegin(), v.cend(), e);
  auto y = acc::find(v.cbegin(), v.cend(), e);
  return x == y;
}

DEF_PROPERTY(FindIf, Accumulate, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i%1) == 0; };
  auto x = find_if(v.cbegin(), v.cend(), even);
  auto y = acc::find_if(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(FindIfNot, Accumulate, const vector<unsigned int>& v)
{
  auto even = [] (unsigned int i) { return (i%1) == 0; };
  auto x = find_if_not(v.cbegin(), v.cend(), even);
  auto y = acc::find_if_not(v.cbegin(), v.cend(), even);
  return x == y;
}

DEF_PROPERTY(Mismatch, Accumulate, const vector<unsigned int>& v)
{
  auto w = v;
  if (w.size() > 0) ++(w[w.size()/2]);

  auto x = mismatch(v.cbegin(), v.cend(), w.cbegin(), equal_to<>());
  auto y = acc::mismatch(v.cbegin(), v.cend(), w.cbegin(), equal_to<>());
  return x == y;
}

DEF_PROPERTY(ReverseCopy, Accumulate, const vector<unsigned int>& v)
{
  vector<unsigned int> outx(v.size(), 0);
  auto x = reverse_copy(v.cbegin(), v.cend(), outx.data());

  vector<unsigned int> outy(v.size(), 0);
  auto y = acc::reverse_copy(v.cbegin(), v.cend(), outy.data());

  using ST = typename vector<unsigned int>::size_type;
  auto sx = static_cast<ST>(x - outx.data());
  auto sy = static_cast<ST>(y - outy.data());

  return sx == outx.size() && sy == outy.size()
    && equal(outx.cbegin(), outx.cend(), outy.cbegin(), outy.cend());
}
