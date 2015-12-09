#ifndef OFFSET_H
#define OFFSET_H
#include <glm/glm.hpp>

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {

  template <>
  struct hash<glm::ivec2>
  {
    std::size_t operator()(const glm::ivec2& k) const
    {
      size_t seed = 0;
      hash_combine(seed, k.x);
      hash_combine(seed, k.y);
      return seed;
    }
  };

  template <>
  struct hash<glm::ivec3>
  {
    std::size_t operator()(const glm::ivec3& k) const
    {
      size_t seed = 0;
      hash_combine(seed, k.x);
      hash_combine(seed, k.y);
      hash_combine(seed, k.z);
      return seed;
    }
  };

  template <>
  struct hash<glm::ivec4>
  {
    std::size_t operator()(const glm::ivec4& k) const
    {
      size_t seed = 0;
      hash_combine(seed, k.x);
      hash_combine(seed, k.y);
      hash_combine(seed, k.z);
      hash_combine(seed, k.w);
      return seed;
    }
  };
}

#endif // OFFSET_H
