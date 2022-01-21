/*******************************************************************************
 * Copyright (c) 2022 CEA
 * This program and the accompanying materials are made available under the 
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 * Contributors: Francois Letierce
 *******************************************************************************/

#include <array>
#include <initializer_list>
#include <memory>
#include <type_traits>
#include <iterator>
#include <cassert>
#include <string>
#include <stdexcept>
 
#ifdef FOR_KOKKOS
#define __COMPILE_FLAG__ KOKKOS_INLINE_FUNCTION
#else
#define __COMPILE_FLAG__
#endif

/**
 * @class SentinelArray
 * @brief Simple static 1D array which adds variable end support to std::array.
 *
 *                   -----------------
 * std::array<int,4> | 0 | 1 | 2 | 3 |
 *                   -----------------
 * 
 *                                   -------------------------
 * SentinelArray<int, 4>({42, 1337}) | 42 | 1337 | xxx | xxx |
 *                                   -------------------------
 *                                                ^^^^^^^^^^^
 *                                  uninitialized and wasted memory space
 * Sentinel value is 2 is this case.
 * Using iterators, you will have access to 42 and 1337.
 * 
 * 
 * @note GPU kernel friendly. 
 *       Handy for variable data size use when vector cannot be used
 * 
 * 
 * @todo - Add __cplusplus macro check and constexpr methods for >c++17
 *       - Implement a better GPU macros support (__global__, KOKKOS_*, etc.)
 * 
 * 
 * @tparam T Data type
 * @tparam N Maximum container size
 */
template <typename T, size_t N>
class SentinelArray : public std::array<T,N>
{
 public:
  /***************************** Ctors ****************************************/
  __COMPILE_FLAG__
  SentinelArray() = default;
  
  __COMPILE_FLAG__
  SentinelArray(std::initializer_list<T> init) : m_sentinel(init.size())
  {
    assert(m_sentinel <= N);
    // std::uninitialized_copy(init.begin(), init.end(), this->data());  // host function only
    size_t i(0);
    for (auto it(init.begin()); it != init.end(); ++it, ++i)
      this->data()[i] = *it;
  }
  
  template <typename Iter, typename std::enable_if_t<std::is_same_v<typename std::iterator_traits<Iter>::iterator_category, std::random_access_iterator_tag>>* = nullptr>
  __COMPILE_FLAG__
  SentinelArray(Iter begin, Iter end) : m_sentinel(std::distance(begin, end))
  {
    assert(m_sentinel <= N);
    // std::uninitialized_copy(begin, end, this->data());  // host function only :(
    size_t i(0);
    for (Iter it(begin); it != end; ++it, ++i)
      this->data()[i] = *it;
  }
  
  /***************************** Copy operator ********************************/
  __COMPILE_FLAG__
  SentinelArray& operator=(std::initializer_list<T> init)
  {
    m_sentinel = init.size();
    assert(m_sentinel <= N);
    // std::uninitialized_copy(init.begin(), init.end(), this->data());  // host function only
    size_t i(0);
    for (auto it(init.begin()); it != init.end(); ++it, ++i)
      this->data()[i] = *it;
    return *this;
  }
  
  /***************************** End iterators ********************************/
  __COMPILE_FLAG__
  typename std::array<T,N>::iterator end() noexcept
  {
      return typename std::array<T,N>::iterator(this->data()+m_sentinel);
  }
  __COMPILE_FLAG__
  typename std::array<T,N>::const_iterator end() const noexcept
  {
    return typename std::array<T,N>::const_iterator(this->data()+m_sentinel);
  }
  __COMPILE_FLAG__
  typename std::array<T,N>::const_iterator cend() const noexcept
  {
    return typename std::array<T,N>::const_iterator(this->data()+m_sentinel);
  }

  /***************************** Reverse begin iterators **********************/
  __COMPILE_FLAG__
  typename std::array<T,N>::reverse_iterator rbegin() noexcept
  {
    return typename std::array<T,N>::reverse_iterator(this->data()+m_sentinel);
  }
  __COMPILE_FLAG__
  typename std::array<T,N>::const_reverse_iterator rbegin() const noexcept
  {
    return typename std::array<T,N>::const_reverse_iterator(this->data()+m_sentinel);
  }
  __COMPILE_FLAG__
  typename std::array<T,N>::const_reverse_iterator crbegin() const noexcept
  {
    return typename std::array<T,N>::const_reverse_iterator(this->data()+m_sentinel);
  }

  /***************************** Size operator ********************************/
  __COMPILE_FLAG__
  typename std::array<T,N>::size_type size() const noexcept
  {
    return m_sentinel;
  }
  
  /***************************** Bound checking accessors *********************/
  typename std::array<T,N>::reference at(typename std::array<T,N>::size_type pos)
  {
      if (!(pos < m_sentinel))
          throw std::out_of_range("array::at: pos (which is " + std::to_string(pos) + ") >= m_sentinel (which is " + std::to_string(m_sentinel) + ")");
      return (*this)[pos];
  }
  typename std::array<T,N>::const_reference at(typename std::array<T,N>::size_type pos) const
  {
      if (!(pos < m_sentinel))
          throw std::out_of_range("array::at: pos (which is " + std::to_string(pos) + ") >= m_sentinel (which is " + std::to_string(m_sentinel) + ")");
      return (*this)[pos];
  }
  
  /***************************** Back accessor ********************************/
  typename std::array<T,N>::reference back() noexcept
  {
    return m_sentinel ? *(end() - 1) : *end();
  }

  /***************************** Sentinel setter ******************************/
  __COMPILE_FLAG__
  void setSentinel(typename std::array<T,N>::size_type s)
  {
    m_sentinel = s;
  }

  /***************************** Private attribute ****************************/
 private:
  typename std::array<T,N>::size_type m_sentinel;
};
