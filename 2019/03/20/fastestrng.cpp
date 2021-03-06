#include <cstdint>
#include <iostream>
#include <sys/time.h>

/***
 * Fast Splittable Pseudorandom Number Generators
 * Steele Jr, Guy L., Doug Lea, and Christine H. Flood. "Fast splittable
 * pseudorandom number splitmix64_xgenerators."
 * ACM SIGPLAN Notices 49.10 (2014): 453-472.
 * ***/

uint64_t splitmix64_x; /* The state can be seeded with any value. */

static inline void splitmix64_seed(uint64_t seed) { splitmix64_x = seed; }


static inline uint64_t splitmix64(void) {
  uint64_t z = (splitmix64_x += UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}



static unsigned long long wyseed;

inline unsigned long long wyrng(void) {
  wyseed += 0x60bee2bee120fc15ull;
  __uint128_t tmp = (__uint128_t)(wyseed)*0xa3b195354a39b70dull;
  unsigned long long m1 = (tmp >> 64) ^ tmp;
  tmp = (__uint128_t)m1 * 0x1b03738712fad5c9ull;
  return (tmp >> 64) ^ tmp;
}

static __uint128_t g_lehmer64_state;
inline unsigned long long lehmer64() {
  g_lehmer64_state *= 0xda942042e4dd58b5ull;
  return g_lehmer64_state >> 64;
}
static __uint128_t g_lehmer64_state2;
inline unsigned long long lehmer64_2() {
  g_lehmer64_state2 *= 0xda942042e4dd58b5ull;
  return g_lehmer64_state2 >> 64;
}
static __uint128_t g_lehmer64_state3;
inline unsigned long long lehmer64_3() {
  g_lehmer64_state3 *= 0xda942042e4dd58b5ull;
  return g_lehmer64_state3 >> 64;
}

using namespace std;
int main(void) {
#define N 524288
  uint64_t recipient[N];
  g_lehmer64_state = (__uint128_t)1232451235;
  g_lehmer64_state2 = (__uint128_t)135432;
  g_lehmer64_state3 = (__uint128_t)5135432;
  splitmix64_x = 44324;
  wyseed = 3214215;
  timeval beg, end;
  for (size_t ti = 0; ti < 4; ti++) {
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6*6;) {
      recipient[i++] = wyrng();
      recipient[i++] = wyrng();
      recipient[i++] = wyrng();
      recipient[i++] = wyrng();
      recipient[i++] = wyrng();
      recipient[i++] = wyrng();
    }
    gettimeofday(&end, NULL);
    cerr << "wyrng   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6*6; i++) {
      recipient[i++] = splitmix64();
      recipient[i++] = splitmix64();
      recipient[i++] = splitmix64();
      recipient[i++] = splitmix64();
      recipient[i++] = splitmix64();
      recipient[i++] = splitmix64();
    }
    gettimeofday(&end, NULL);
    cerr << "splitmix64   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6*6;) {
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6*6;) {
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64_2();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64_2();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64_2();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (2)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6*6;) {
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64_2();
      recipient[i++] = lehmer64_3();
      recipient[i++] = lehmer64();
      recipient[i++] = lehmer64_2();
      recipient[i++] = lehmer64_3();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (3)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << recipient[312] << endl;
    cout << endl
         << "Next we do random number computations only, doing no work."
         << endl;
    uint64_t s = 0;
    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6; i++) {
      s += wyrng();
      s += wyrng();
      s += wyrng();
      s += wyrng();
      s += wyrng();
      s += wyrng();
    }
    gettimeofday(&end, NULL);
    cerr << "wyrng   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6; i++) {
      s += splitmix64();
      s += splitmix64();
      s += splitmix64();
      s += splitmix64();
      s += splitmix64();
      s += splitmix64();
    }
    gettimeofday(&end, NULL);
    cerr << "splitmix64   \t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N/6; i++) {
      s += lehmer64();
      s += lehmer64();
      s += lehmer64();
      s += lehmer64();
      s += lehmer64();
      s += lehmer64();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N / 6; i++) {
      s += lehmer64() + lehmer64_2();
      s += lehmer64() + lehmer64_2();
      s += lehmer64() + lehmer64_2();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (2)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;

    gettimeofday(&beg, NULL);
    for (size_t i = 0; i < N / 6; i++) {
      s += lehmer64() + lehmer64_2() + lehmer64_3();
      s += lehmer64() + lehmer64_2() + lehmer64_3();
    }
    gettimeofday(&end, NULL);
    cerr << "lehmer64 (3)\t"
         << (end.tv_sec - beg.tv_sec) + 1e-6 * (end.tv_usec - beg.tv_usec)
         << " s" << endl;
    cout << "bogus:" << s << endl;
    cout << endl << endl;
  }
  return 0;
}
