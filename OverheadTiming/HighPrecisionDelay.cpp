#include "HighPrecisionDelay.h"
#include <windows.h>
#include <timeapi.h>
#include <mmsystem.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void HighPrecisionDelay(double milliseconds) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    QueryPerformanceCounter(&end);

    double elapsedTime = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
    double remainingTime = milliseconds - elapsedTime;
    if (remainingTime > 0) {
        Sleep((DWORD)remainingTime);
    }
}

// Other alternatives

// TimeBeginPeriod and TimeEndPeriod
//void HighPrecisionDelay(double milliseconds) {
//    timeBeginPeriod(1);  // Set the system timer resolution to 1 ms
//
//    LARGE_INTEGER frequency;
//    LARGE_INTEGER start, end;
//
//    QueryPerformanceFrequency(&frequency);
//    QueryPerformanceCounter(&start);
//
//    double elapsedTime;
//    do {
//        QueryPerformanceCounter(&end);
//        elapsedTime = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
//    } while (elapsedTime < milliseconds);
//
//    timeEndPeriod(1);  // Reset the timer resolution
//}

// Busy Waiting (Spin Lock)
//void HighPrecisionDelay(double milliseconds) {
//    LARGE_INTEGER frequency;
//    LARGE_INTEGER start, end;
//
//    QueryPerformanceFrequency(&frequency);
//    QueryPerformanceCounter(&start);
//
//    do {
//        QueryPerformanceCounter(&end);
//    } while ((end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart < milliseconds);
//}

// High-Resolution Timers (Multimedia Timers)
//void CALLBACK TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
//    // Signal that the timer has completed using a user-defined event or similar mechanism
//    SetEvent((HANDLE)dwUser);
//}
//
//void HighPrecisionDelay(double milliseconds) {
//    // Ensure the timer resolution is set to the highest possible
//    TIMECAPS tc;
//    if (timeGetDevCaps(&tc, sizeof(tc)) == TIMERR_NOERROR) {
//        timeBeginPeriod(tc.wPeriodMin);
//    }
//
//    // Create an event for signaling the end of the timer
//    HANDLE hTimerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//
//    // Set up the timer with the desired delay time
//    UINT timerId = timeSetEvent(
//        (UINT)milliseconds, // Delay time in ms
//        0,                  // Minimum timer resolution
//        TimerCallback,      // Callback function
//        (DWORD_PTR)hTimerEvent,  // User data passed to the callback function
//        TIME_ONESHOT        // Type of the timer; oneshot runs the timer once
//    );
//
//    if (timerId != 0) {
//        // Wait for the timer event to be signaled within the message loop
//        MSG msg;
//        while (GetMessage(&msg, NULL, 0, 0)) {
//            if (msg.message == WM_QUIT)
//                break;
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//            if (WaitForSingleObject(hTimerEvent, 0) == WAIT_OBJECT_0) {
//                // Event was signaled, break the loop
//                break;
//            }
//        }
//        // Destroy the timer event after use
//        timeKillEvent(timerId);
//    }
//
//    // Reset the timer resolution
//    timeEndPeriod(tc.wPeriodMin);
//
//    // Clean up
//    CloseHandle(hTimerEvent);
//}