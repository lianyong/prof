#pragma once

#define PROF_API			__attribute__((__no_instrument_function__))
#define PROF_CONSTRCUCTOR   __attribute__((constructor))
#define PROF_DESSTRCUCTOR	__attribute__((destructor))
#define PROF_INLINE			__attribute__((always_inline))

#define EXTERN_DECLARE					extern 
#define EXTERN_DECLARE_C				extern "C" 
#define EXTERN_DECLARE_CPP              extern 

#define COMMON_FATAL_ERROR  "some fatal error"
#define YERR(fmt,...)  fprintf(stderr, "%s: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)
#define YOUT(fmt,...)  fprintf(stdout, "%s: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)