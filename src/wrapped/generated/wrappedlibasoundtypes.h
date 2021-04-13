/*******************************************************************
 * File automatically generated by rebuild_wrappers.py (v1.3.0.10) *
 *******************************************************************/
#ifndef __wrappedlibasoundTYPES_H_
#define __wrappedlibasoundTYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef int64_t (*iFp_t)(void*);
typedef void* (*pFppp_t)(void*, void*, void*);
typedef int64_t (*iFpipp_t)(void*, int64_t, void*, void*);
typedef int64_t (*iFpppp_t)(void*, void*, void*, void*);
typedef void* (*pFpipL_t)(void*, int64_t, void*, uintptr_t);

#define SUPER() ADDED_FUNCTIONS() \
	GO(snd_dlclose, iFp_t) \
	GO(snd_lib_error_set_handler, iFp_t) \
	GO(snd_dlsym, pFppp_t) \
	GO(snd_async_add_handler, iFpipp_t) \
	GO(snd_async_add_pcm_handler, iFpppp_t) \
	GO(snd_dlopen, pFpipL_t)

#endif // __wrappedlibasoundTYPES_H_