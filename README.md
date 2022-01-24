# SentinelArray

## Simple static 1D array which adds variable end support to std::array.

*GPU kernel friendly. Handy for variable data size when vector cannot be used*

---

### Use case:

`std::array<int,4>`
| 0 | 1 | 2 | 3 |
|---|---|---|---|

`SentinelArray<int, 4>({42, 1337})`
| 42 | 1337 | xxx | xxx |
|---|---|---|---|

               ^^^^^^^^^^^^^
               uninitialized and wasted memory space

Sentinel value is 2 is this case.

Using iterators, you will have access to 42 and 1337.


```
std::vector<int> v{100,200,300};
SentinelArray<int, 10> a(v.begin(), v.end())  // 7 undefined values after 3rd element
for (auto i : a)
 std::cout << i << ",";
```
> 100,200,300

---


### TODO list:

-  Add tests !!!!!
- Implement better GPU macros support (`__global__`, `KOKKOS_*`, etc.)
- Add override method or support for :
  - [ ] fill
  - [ ] swap
  - [ ] operator==
  - [ ] operator!=
  - [ ] operator<
  - [ ] operator<=
  - [ ] operator>
  - [ ] operator>=
  - [ ] operator<=>
  - [ ] std::get
  - [ ] std::swap
  - [ ] to_array
  - [ ] std::tuple_size
- Add `__cplusplus` macro check and write constexpr methods for *std > c++17. Reminder:*
  - C++ pre-C++98: `__cplusplus` is 1`
  - C++98: `__cplusplus` is `199711L`
  - C++11: `__cplusplus` is `201103L`
  - C++14: `__cplusplus` is `201402L`
  - C++17: `__cplusplus` is `201703L`
  - C++20: `__cplusplus` is `202002L`
