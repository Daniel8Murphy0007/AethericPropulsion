// source174_wolfram_bridge_embedded.cpp
// Wolfram WSTP (Wolfram Symbolic Transfer Protocol) Bridge - Embedded Integration
// Provides symbolic math evaluation via Wolfram Engine/Mathematica
// Required by: source176_auto_full_uqff.cpp, source200_cosmic_quantum_egg.cpp
// Copyright © 2025 Daniel T. Murphy

#ifndef SOURCE174_WOLFRAM_BRIDGE_EMBEDDED_CPP
#define SOURCE174_WOLFRAM_BRIDGE_EMBEDDED_CPP

#include <string>
#include <iostream>

// Conditional compilation - only active if Wolfram Engine libraries available
#ifdef USE_EMBEDDED_WOLFRAM

// Include Wolfram WSTP headers (Wolfram Engine 14.3 installed)
#include "wstp.h"
#include "WolframLibrary.h"

// WSTP connection for Wolfram Engine communication
static WSLINK wstpLink = nullptr;

// Initialize Wolfram WSTP connection
bool WolframInit()
{
    std::cout << "[source174] Wolfram WSTP bridge initializing..." << std::endl;

    // Initialize WSTP with Wolfram Engine 14.3
    int argc = 0;
    char **argv = nullptr;
    int err;

    wstpLink = WSInitialize(nullptr);
    if (wstpLink == nullptr)
    {
        std::cerr << "[source174] ERROR: Failed to initialize WSTP" << std::endl;
        return false;
    }

    // Open link to Wolfram Kernel
    const char *linkArgs[] = {"-linkname", "\"C:\\Program Files\\Wolfram Research\\Wolfram Engine\\14.3\\WolframKernel.exe\" -wstp", nullptr};
    wstpLink = WSOpenArgcArgv(wstpLink, 2, (char **)linkArgs, &err);

    if (wstpLink == nullptr || err != WSEOK)
    {
        std::cerr << "[source174] ERROR: Failed to open WSTP link to Wolfram Kernel" << std::endl;
        if (wstpLink)
        {
            const char *errMsg = WSErrorMessage(wstpLink);
            if (errMsg)
            {
                std::cerr << "[source174] WSTP Error: " << errMsg << std::endl;
            }
        }
        return false;
    }

    std::cout << "[source174] Wolfram Engine 14.3 WSTP connection established" << std::endl;
    return true;
}

// Evaluate Wolfram Language expression and return string result
std::string WolframEvalToString(const std::string &expression)
{
    if (wstpLink == nullptr)
    {
        return "[ERROR: WSTP not initialized]";
    }

    // Send evaluation packet to Wolfram Kernel
    WSPutFunction(wstpLink, "EvaluatePacket", 1);
    WSPutFunction(wstpLink, "ToString", 1);
    WSPutString(wstpLink, expression.c_str());
    WSEndPacket(wstpLink);
    WSFlush(wstpLink);

    // Wait for and retrieve result
    int pkt;
    while ((pkt = WSNextPacket(wstpLink)) && pkt != RETURNPKT)
    {
        WSNewPacket(wstpLink);
    }

    const char *result = nullptr;
    if (!WSGetString(wstpLink, &result))
    {
        const char *errMsg = WSErrorMessage(wstpLink);
        return std::string("[ERROR: ") + (errMsg ? errMsg : "Unknown WSTP error") + "]";
    }

    std::string output(result);
    WSReleaseString(wstpLink, result);

    return output;
}

// Evaluate Wolfram expression and return double
double WolframEvalToDouble(const std::string &expression)
{
    if (wstpLink == nullptr)
    {
        std::cerr << "[source174] ERROR: WSTP not initialized" << std::endl;
        return 0.0;
    }

    // Send evaluation packet to Wolfram Kernel
    WSPutFunction(wstpLink, "EvaluatePacket", 1);
    WSPutFunction(wstpLink, "N", 1); // Force numerical evaluation
    WSPutString(wstpLink, expression.c_str());
    WSEndPacket(wstpLink);
    WSFlush(wstpLink);

    // Wait for and retrieve result
    int pkt;
    while ((pkt = WSNextPacket(wstpLink)) && pkt != RETURNPKT)
    {
        WSNewPacket(wstpLink);
    }

    double result = 0.0;
    if (!WSGetReal64(wstpLink, &result))
    {
        // Try getting as string and parsing
        const char *strResult = nullptr;
        if (WSGetString(wstpLink, &strResult))
        {
            result = std::atof(strResult);
            WSReleaseString(wstpLink, strResult);
        }
        else
        {
            const char *errMsg = WSErrorMessage(wstpLink);
            std::cerr << "[source174] ERROR: " << (errMsg ? errMsg : "Unknown WSTP error") << std::endl;
        }
    }

    return result;
}

// Close Wolfram WSTP connection
void WolframShutdown()
{
    if (wstpLink != nullptr)
    {
        WSClose(wstpLink);
        WSDeinitialize(wstpLink);
        wstpLink = nullptr;
        std::cout << "[source174] Wolfram WSTP connection closed" << std::endl;
    }
}

// Wrapper function for compatibility with source176
void WolframEmbeddedBridge()
{
    if (!WolframInit())
    {
        std::cerr << "[source174] ERROR: Wolfram initialization failed" << std::endl;
        return;
    }

    std::cout << "[source174] Wolfram Embedded Bridge ready" << std::endl;
    std::cout << "[source174] Connected to Wolfram Engine 14.3" << std::endl;
    std::cout << "[source174] WSTP symbolic evaluation active" << std::endl;
}

#else

// Stub implementation when Wolfram not available
std::string WolframEvalToString(const std::string &expression)
{
    return "[Wolfram not available - compile with -DUSE_EMBEDDED_WOLFRAM=ON]";
}

double WolframEvalToDouble(const std::string &expression)
{
    return 0.0;
}

void WolframEmbeddedBridge()
{
    std::cout << "[source174] Wolfram bridge disabled - USE_EMBEDDED_WOLFRAM not defined" << std::endl;
}

bool WolframInit() { return false; }
void WolframShutdown() {}

#endif // USE_EMBEDDED_WOLFRAM

#endif // SOURCE174_WOLFRAM_BRIDGE_EMBEDDED_CPP
