#include <string>
inline int caselessCompare(const char *a, const char *b, size_t len) {
#ifdef _WIN32
  return _strnicmp(a, b, len);
#else
  return strncasecmp(a, b, len);
#endif
}

inline int caselessCompare(const char *a, const char *b) {
#ifdef _WIN32
  return _stricmp(a, b);
#else
  return strcasecmp(a, b);
#endif
}
