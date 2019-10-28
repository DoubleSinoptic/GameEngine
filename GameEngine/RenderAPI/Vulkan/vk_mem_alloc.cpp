#define VMA_IMPLEMENTATION

template<typename T>
T max(T a, T b)
{
	return a > b ? a : b;
}
template<typename T>
T min(T a, T b)
{
	return a < b ? a: b;
}
#define VMA_MIN(v1, v2)    (min((v1), (v2)))
#define VMA_MAX(v1, v2)    (max((v1), (v2)))
#include "vk_mem_alloc.h"
