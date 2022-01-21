# SentinelArray

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
