#ifndef SIMDJSON_IMPLEMENTATION_DETECTION_H
#define SIMDJSON_IMPLEMENTATION_DETECTION_H

#include "simdjson/common_defs.h"

//
// First, figure out which implementations can be run. Doing it here makes it so we don't have to worry about the order
// in which we include them.
//

#ifndef SIMDJSON_IMPLEMENTATION_ARM64
#define SIMDJSON_IMPLEMENTATION_ARM64 (SIMDJSON_IS_ARM64)
#endif
#define SIMDJSON_CAN_ALWAYS_RUN_ARM64 SIMDJSON_IMPLEMENTATION_ARM64 && SIMDJSON_IS_ARM64

// Default Icelake to on if this is x86-64. Even if we're not compiled for it, it could be selected
// at runtime.
#ifndef SIMDJSON_IMPLEMENTATION_ICELAKE
#define SIMDJSON_IMPLEMENTATION_ICELAKE ((SIMDJSON_IS_X86_64) && (SIMDJSON_AVX512_ALLOWED) && (SIMDJSON_COMPILER_SUPPORTS_VBMI2))
#endif

#ifdef _MSC_VER
// To see why  (__BMI__) && (__PCLMUL__) && (__LZCNT__) are not part of this next line, see
// https://github.com/simdjson/simdjson/issues/1247
#define SIMDJSON_CAN_ALWAYS_RUN_ICELAKE ((SIMDJSON_IMPLEMENTATION_ICELAKE) && (__AVX2__) && (__AVX512F__) && (__AVX512DQ__) && (__AVX512CD__) && (__AVX512BW__) && (__AVX512VL__) && (__AVX512VBMI2__))
#else
#define SIMDJSON_CAN_ALWAYS_RUN_ICELAKE ((SIMDJSON_IMPLEMENTATION_ICELAKE) && (__AVX2__) && (__BMI__) && (__PCLMUL__) && (__LZCNT__) && (__AVX512F__) && (__AVX512DQ__) && (__AVX512CD__) && (__AVX512BW__) && (__AVX512VL__) && (__AVX512VBMI2__))
#endif

// Default Haswell to on if this is x86-64. Even if we're not compiled for it, it could be selected
// at runtime.
#ifndef SIMDJSON_IMPLEMENTATION_HASWELL
#if SIMDJSON_CAN_ALWAYS_RUN_ICELAKE
// if icelake is always available, never enable haswell.
#define SIMDJSON_IMPLEMENTATION_HASWELL 0
#else
#define SIMDJSON_IMPLEMENTATION_HASWELL SIMDJSON_IS_X86_64
#endif
#endif
#ifdef _MSC_VER
// To see why  (__BMI__) && (__PCLMUL__) && (__LZCNT__) are not part of this next line, see
// https://github.com/simdjson/simdjson/issues/1247
#define SIMDJSON_CAN_ALWAYS_RUN_HASWELL ((SIMDJSON_IMPLEMENTATION_HASWELL) && (SIMDJSON_IS_X86_64) && (__AVX2__))
#else
#define SIMDJSON_CAN_ALWAYS_RUN_HASWELL ((SIMDJSON_IMPLEMENTATION_HASWELL) && (SIMDJSON_IS_X86_64) && (__AVX2__) && (__BMI__) && (__PCLMUL__) && (__LZCNT__))
#endif

// Default Westmere to on if this is x86-64.
#ifndef SIMDJSON_IMPLEMENTATION_WESTMERE
#if SIMDJSON_CAN_ALWAYS_RUN_ICELAKE || SIMDJSON_CAN_ALWAYS_RUN_HASWELL
// if icelake or haswell are always available, never enable westmere.
#define SIMDJSON_IMPLEMENTATION_WESTMERE 0
#else
#define SIMDJSON_IMPLEMENTATION_WESTMERE SIMDJSON_IS_X86_64
#endif
#endif
#define SIMDJSON_CAN_ALWAYS_RUN_WESTMERE (SIMDJSON_IMPLEMENTATION_WESTMERE && SIMDJSON_IS_X86_64 && __SSE4_2__ && __PCLMUL__)

#ifndef SIMDJSON_IMPLEMENTATION_PPC64
#define SIMDJSON_IMPLEMENTATION_PPC64 (SIMDJSON_IS_PPC64 && SIMDJSON_IS_PPC64_VMX)
#endif
#define SIMDJSON_CAN_ALWAYS_RUN_PPC64 SIMDJSON_IMPLEMENTATION_PPC64 && SIMDJSON_IS_PPC64 && SIMDJSON_IS_PPC64_VMX

// Default Fallback to on unless a builtin implementation has already been selected.
#ifndef SIMDJSON_IMPLEMENTATION_FALLBACK
#if SIMDJSON_CAN_ALWAYS_RUN_ARM64 || SIMDJSON_CAN_ALWAYS_RUN_ICELAKE || SIMDJSON_CAN_ALWAYS_RUN_HASWELL || SIMDJSON_CAN_ALWAYS_RUN_WESTMERE || SIMDJSON_CAN_ALWAYS_RUN_PPC64
// if anything at all except fallback can always run, then disable fallback.
#define SIMDJSON_IMPLEMENTATION_FALLBACK 0
#else
#define SIMDJSON_IMPLEMENTATION_FALLBACK 1
#endif
#endif
#define SIMDJSON_CAN_ALWAYS_RUN_FALLBACK SIMDJSON_IMPLEMENTATION_FALLBACK

#endif // SIMDJSON_IMPLEMENTATION_DETECTION_H