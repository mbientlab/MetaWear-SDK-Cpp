/**
 * @copyright MbientLab License
 * @file dllmarker.h
 * @brief Macros for identifying exported functions
 */
#pragma once

#if defined _WIN32 || defined __CYGWIN__
#define METAWEAR_HELPER_DLL_IMPORT __declspec(dllimport)
#define METAWEAR_HELPER_DLL_EXPORT __declspec(dllexport)
#define METAWEAR_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define METAWEAR_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define METAWEAR_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define METAWEAR_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define METAWEAR_HELPER_DLL_IMPORT
#define METAWEAR_HELPER_DLL_EXPORT
#define METAWEAR_HELPER_DLL_LOCAL
#endif
#endif

#if defined _WINDLL || defined METAWEAR_DLL // defined if METAWEAR is compiled as a DLL
/** Indicates the function should be exported to the symbol table  */
#ifdef METAWEAR_DLL_EXPORTS // defined if we are building the METAWEAR DLL (instead of using it)
#define METAWEAR_API METAWEAR_HELPER_DLL_EXPORT
#else
#define METAWEAR_API METAWEAR_HELPER_DLL_IMPORT
#endif // METAWEAR_DLL_EXPORTS
#define METAWEAR_LOCAL METAWEAR_HELPER_DLL_LOCAL
#else // METAWEAR_DLL is not defined: this means METAWEAR is a static lib.
#define METAWEAR_API
#define METAWEAR_LOCAL
#endif // METAWEAR_DLL

