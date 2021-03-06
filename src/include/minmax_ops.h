#pragma once

#include "accumulate.h"
#include "modifying_seq_ops.h"
#include "non_modifying_seq_ops.h"

#ifndef _MSC_VER
#include <experimental/optional>
#endif

#include <initializer_list>
#include <iterator>
#include <tuple>
#include <utility>

// ---------------------------------------------------------------------------
// 10 minimum/maximum operations
//
// max
// max_element
// min
// min_element
// minmax
// minmax_element
// lexicographical_compare
// is_permutation
// next_permutation
// prev_permutation
//

namespace acc
{

  // ---------------------------------------------------------------------------
  // min_element

  template <typename ForwardIt, typename Compare>
  inline ForwardIt min_element(
      ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? a : b;
        });
  }

  // ---------------------------------------------------------------------------
  // max_element

  template <typename ForwardIt, typename Compare>
  inline ForwardIt max_element(
      ForwardIt first, ForwardIt last, Compare cmp)
  {
    return acc::accumulate_iter(
        first, last, first,
        [&] (const ForwardIt& a, const ForwardIt& b) {
          return cmp(*a, *b) ? b : a;
        });
  }

  // ---------------------------------------------------------------------------
  // minmax_element

  template <typename ForwardIt, typename Compare>
  inline std::pair<ForwardIt, ForwardIt> minmax_element(
      ForwardIt first, ForwardIt last, Compare cmp)
  {
    using P = std::pair<ForwardIt, ForwardIt>;
    return acc::accumulate_iter(
        first, last, P{ first, first },
        [&] (const P& a, const ForwardIt& b) -> P {
          auto& f = cmp(*a.first, *b) ? a.first : b;
          auto& s = cmp(*a.second, *b) ? b : a.second;
          return { f, s };
        });
  }

  // ---------------------------------------------------------------------------
  // min, max, minmax (initializer_list forms)

  template <typename T, typename Compare>
  inline T min(std::initializer_list<T> ilist, Compare cmp)
  {
    return *acc::min_element(ilist.begin(), ilist.end(), cmp);
  }

  template <typename T, typename Compare>
  inline T max(std::initializer_list<T> ilist, Compare cmp)
  {
    return *acc::max_element(ilist.begin(), ilist.end(), cmp);
  }

  template <typename T, typename Compare>
  inline std::pair<T, T> minmax(
      std::initializer_list<T> ilist, Compare cmp)
  {
    auto p = acc::minmax_element(ilist.begin(), ilist.end(), cmp);
    return { *p.first, *p.second };
  }

#ifndef _MSC_VER

  // ---------------------------------------------------------------------------
  // min_element (safe value form)

  template <typename InputIt, typename Compare>
  inline std::experimental::optional<
    typename std::iterator_traits<InputIt>::value_type>
  min_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&] (const T& a, const T& b) {
            return cmp(a, b) ? a : b;
          }) };
  }

  // ---------------------------------------------------------------------------
  // max_element (safe value form)

  template <typename InputIt, typename Compare>
  inline std::experimental::optional<
    typename std::iterator_traits<InputIt>::value_type>
  max_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, init,
          [&] (const T& a, const T& b) {
            return cmp(a, b) ? b : a;
          }) };
  }

  // ---------------------------------------------------------------------------
  // minmax_element (safe value form)

  template <typename InputIt, typename Compare>
  inline std::experimental::optional<
    std::pair<typename std::iterator_traits<InputIt>::value_type,
              typename std::iterator_traits<InputIt>::value_type>>
  minmax_element_safe(InputIt first, InputIt last, Compare cmp)
  {
    if (first == last) return std::experimental::nullopt;
    using T = typename std::iterator_traits<InputIt>::value_type;
    using P = std::pair<T, T>;
    auto init = *first;
    return {
      acc::accumulate(
          ++first, last, P{ init, init },
          [&] (const P& a, const T& b) -> P {
            auto& f = cmp(a.first, b) ? a.first : b;
            auto& s = cmp(a.second, b) ? b : a.second;
            return { f, s };
          }) };
  }

#endif

  // ---------------------------------------------------------------------------
  // lexicographical_compare

  template <typename InputIt1, typename InputIt2, typename Compare>
  inline bool lexicographical_compare(
      InputIt1 first1, InputIt1 last1,
      InputIt2 first2, InputIt2 last2,
      Compare cmp)
  {
    using T = typename std::iterator_traits<InputIt1>::value_type;
    using P = std::pair<bool, InputIt2>;
    try
    {
     return acc::accumulate(
         first1, last1, P{ false, first2 },
         [&] (P a, const T& b) {
           if (a.second == last2 || cmp(*a.second, b)) throw false;
           if (cmp(b, *a.second)) throw true;
            auto next = ++a.second;
            return P{ next != last2, next };
         }).first;
    }
    catch (bool b)
    {
      return b;
    }
  }

  // ---------------------------------------------------------------------------
  // is_permutation

  template <typename ForwardIt1, typename ForwardIt2>
  inline bool is_permutation(ForwardIt1 first, ForwardIt1 last,
                             ForwardIt2 d_first)
  {
    std::tie(first, d_first) = acc::mismatch(first, last, d_first, std::equal_to<>{});
    if (first == last) return true;

    ForwardIt2 d_last = d_first;
    std::advance(d_last, std::distance(first, last));
    return acc::accumulate_iter(
        first, last, true,
        [&] (bool b, ForwardIt1 i) {
          if (!b) return false;
          if (acc::count(first, i, *i) != 0) return true;
          auto m = acc::count(d_first, d_last, *i);
          return m != 0 && acc::count(i, last, *i) == m;
        });
  }


  // ---------------------------------------------------------------------------
  // next_permutation (note weakened iterator category)

  template <typename ForwardIt, typename Compare>
  inline bool next_permutation(ForwardIt first, ForwardIt last, Compare cmp)
  {
    using T = typename std::iterator_traits<ForwardIt>::value_type;
    std::reverse_iterator<ForwardIt> rfirst(last);
    std::reverse_iterator<ForwardIt> rlast(first);

    auto i = acc::adjacent_find(
        rfirst, rlast,
        [&] (const T& a, const T& b) { return cmp(b, a); });
    if (i == rlast)
    {
      acc::reverse(first, last);
      return false;
    }
    else
    {
      ++i;
      auto j = acc::find_if(
          rfirst, i,
          [&] (const T& a) { return cmp(*i, a); });
      std::iter_swap(i, j);
      acc::reverse(rfirst, i);
      return true;
    }
  }

  template <typename ForwardIt>
  inline bool next_permutation(ForwardIt first, ForwardIt last)
  {
    return acc::next_permutation(first, last, std::less<>{});
  }

  // ---------------------------------------------------------------------------
  // prev_permutation (note weakened iterator category)

  template <typename ForwardIt, typename Compare>
  inline bool prev_permutation(ForwardIt first, ForwardIt last, Compare cmp)
  {
    using T = typename std::iterator_traits<ForwardIt>::value_type;
    std::reverse_iterator<ForwardIt> rfirst(last);
    std::reverse_iterator<ForwardIt> rlast(first);

    auto i = acc::adjacent_find(
        rfirst, rlast,
        [&] (const T& a, const T& b) { return cmp(a, b); });
    if (i == rlast)
    {
      acc::reverse(first, last);
      return false;
    }
    else
    {
      ++i;
      auto j = acc::find_if(
          rfirst, i,
          [&] (const T& a) { return cmp(a, *i); });
      std::iter_swap(i, j);
      acc::reverse(rfirst, i);
      return true;
    }
  }

  template <typename ForwardIt>
  inline bool prev_permutation(ForwardIt first, ForwardIt last)
  {
    return acc::prev_permutation(first, last, std::less<>{});
  }

}
