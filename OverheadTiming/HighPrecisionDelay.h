#ifndef HIGH_PRECISION_DELAY_H
#define HIGH_PRECISION_DELAY_H

// Define EXPORTING_DLL only when building the DLL, not when using it.
#ifdef EXPORTING_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" {
    DLL_EXPORT void HighPrecisionDelay(double milliseconds);
}

#endif // HIGH_PRECISION_DELAY_H