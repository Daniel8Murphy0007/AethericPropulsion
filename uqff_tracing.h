/**
 * ================================================================================================
 * uqff_tracing.h - UQFF Tracing System for MAIN_1_CoAnQi
 * ================================================================================================
 *
 * Comprehensive tracing system for tracking:
 *   - Physics calculations and term evaluations
 *   - System simulations and iterations
 *   - Performance metrics and bottlenecks
 *   - Module registration and initialization
 *   - Wolfram integration calls
 *   - Cross-module communications
 *
 * Integrates with VerboseLogger for unified logging/tracing output
 *
 * Author: Daniel T. Murphy
 * Date: December 1, 2025 (Last Updated: December 4, 2025)
 * Status: ACTIVE - Used by MAIN_1_CoAnQi.exe (runtime verified Dec 4 @ 17:40:40)
 * ================================================================================================
 */

#ifndef UQFF_TRACING_H
#define UQFF_TRACING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <vector>
#include <map>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Trace levels matching OpenTelemetry conventions
enum class TraceLevel
{
    TRACE_DEBUG = 0,
    TRACE_INFO = 1,
    TRACE_WARN = 2,
    TRACE_ERROR = 3,
    TRACE_FATAL = 4
};

// Span types for different operations
enum class SpanType
{
    SYSTEM_CALCULATION,
    PHYSICS_TERM_EVAL,
    MODULE_INIT,
    WOLFRAM_CALL,
    OPTIMIZATION,
    VALIDATION,
    CROSS_MODULE_COMM,
    SIMULATION_STEP,
    STATISTICAL_ANALYSIS
};

/**
 * TraceSpan - Represents a single traced operation
 * Automatically measures duration and logs on destruction (RAII pattern)
 */
class TraceSpan
{
private:
    string spanName;
    SpanType spanType;
    chrono::high_resolution_clock::time_point startTime;
    map<string, string> attributes;
    bool completed;
    ofstream *traceFile;

    string spanTypeToString(SpanType type) const
    {
        switch (type)
        {
        case SpanType::SYSTEM_CALCULATION:
            return "SYSTEM_CALC";
        case SpanType::PHYSICS_TERM_EVAL:
            return "PHYSICS_TERM";
        case SpanType::MODULE_INIT:
            return "MODULE_INIT";
        case SpanType::WOLFRAM_CALL:
            return "WOLFRAM_CALL";
        case SpanType::OPTIMIZATION:
            return "OPTIMIZATION";
        case SpanType::VALIDATION:
            return "VALIDATION";
        case SpanType::CROSS_MODULE_COMM:
            return "CROSS_MODULE";
        case SpanType::SIMULATION_STEP:
            return "SIMULATION";
        case SpanType::STATISTICAL_ANALYSIS:
            return "STATISTICS";
        default:
            return "UNKNOWN";
        }
    }

public:
    TraceSpan(const string &name, SpanType type, ofstream *file = nullptr)
        : spanName(name), spanType(type), completed(false), traceFile(file)
    {
        startTime = chrono::high_resolution_clock::now();

        if (traceFile && traceFile->is_open())
        {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);
            *traceFile << "[SPAN_START] " << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                       << " | " << spanTypeToString(spanType) << " | " << spanName << endl;
        }
    }

    ~TraceSpan()
    {
        if (!completed)
        {
            end();
        }
    }

    // Add attributes to the span
    void setAttribute(const string &key, const string &value)
    {
        attributes[key] = value;
    }

    void setAttribute(const string &key, double value)
    {
        ostringstream oss;
        oss << scientific << setprecision(6) << value;
        attributes[key] = oss.str();
    }

    void setAttribute(const string &key, int value)
    {
        attributes[key] = to_string(value);
    }

    // Mark span as complete
    void end()
    {
        if (completed)
            return;

        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

        if (traceFile && traceFile->is_open())
        {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);

            *traceFile << "[SPAN_END] " << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                       << " | " << spanTypeToString(spanType) << " | " << spanName
                       << " | Duration: " << duration.count() << " µs";

            if (!attributes.empty())
            {
                *traceFile << " | Attributes: {";
                bool first = true;
                for (const auto &attr : attributes)
                {
                    if (!first)
                        *traceFile << ", ";
                    *traceFile << attr.first << "=" << attr.second;
                    first = false;
                }
                *traceFile << "}";
            }

            *traceFile << endl;
        }

        completed = true;
    }

    // Get duration in microseconds
    long long getDurationMicroseconds() const
    {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::microseconds>(now - startTime).count();
    }
};

/**
 * UQFFTracer - Main tracing orchestrator
 * Thread-safe singleton pattern for global access
 */
class UQFFTracer
{
private:
    ofstream traceFile;
    bool enabled;
    TraceLevel minLevel;

#ifdef _WIN32
    CRITICAL_SECTION cs;
#endif

    UQFFTracer() : enabled(false), minLevel(TraceLevel::TRACE_INFO)
    {
#ifdef _WIN32
        InitializeCriticalSection(&cs);
#endif
    }

    ~UQFFTracer()
    {
        if (traceFile.is_open())
        {
            traceFile.close();
        }
#ifdef _WIN32
        DeleteCriticalSection(&cs);
#endif
    }

    void lock()
    {
#ifdef _WIN32
        EnterCriticalSection(&cs);
#else
        // Placeholder for non-Windows
#endif
    }

    void unlock()
    {
#ifdef _WIN32
        LeaveCriticalSection(&cs);
#else
        // Placeholder for non-Windows
#endif
    }

public:
    // Singleton instance
    static UQFFTracer &getInstance()
    {
        static UQFFTracer instance;
        return instance;
    }

    // Initialize tracing
    void initialize(const string &filename = "uqff_trace.log", TraceLevel level = TraceLevel::TRACE_INFO)
    {
        lock();

        if (traceFile.is_open())
        {
            traceFile.close();
        }

        traceFile.open(filename, ios::out | ios::app);
        if (traceFile.is_open())
        {
            enabled = true;
            minLevel = level;

            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);

            traceFile << "\n\n========================================" << endl;
            traceFile << "UQFF TRACING SESSION STARTED" << endl;
            traceFile << "Time: " << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S") << endl;
            traceFile << "========================================\n"
                      << endl;
        }

        unlock();
    }

    // Check if tracing is enabled
    bool isEnabled() const { return enabled; }

    // Create a new span
    unique_ptr<TraceSpan> createSpan(const string &name, SpanType type)
    {
        if (!enabled)
            return nullptr;
        return make_unique<TraceSpan>(name, type, &traceFile);
    }

    // Log a trace event
    void logEvent(const string &message, TraceLevel level = TraceLevel::TRACE_INFO)
    {
        if (!enabled || level < minLevel)
            return;

        lock();

        if (traceFile.is_open())
        {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);

            string levelStr;
            switch (level)
            {
            case TraceLevel::TRACE_DEBUG:
                levelStr = "DEBUG";
                break;
            case TraceLevel::TRACE_INFO:
                levelStr = "INFO";
                break;
            case TraceLevel::TRACE_WARN:
                levelStr = "WARN";
                break;
            case TraceLevel::TRACE_ERROR:
                levelStr = "ERROR";
                break;
            case TraceLevel::TRACE_FATAL:
                levelStr = "FATAL";
                break;
            }

            traceFile << "[" << levelStr << "] "
                      << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                      << " | " << message << endl;
        }

        unlock();
    }

    // Log a performance metric
    void logMetric(const string &metricName, double value, const string &unit = "")
    {
        if (!enabled)
            return;

        lock();

        if (traceFile.is_open())
        {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);

            traceFile << "[METRIC] " << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S")
                      << " | " << metricName << " = " << scientific << setprecision(6) << value;

            if (!unit.empty())
            {
                traceFile << " " << unit;
            }

            traceFile << endl;
        }

        unlock();
    }

    // Shutdown tracing
    void shutdown()
    {
        lock();

        if (traceFile.is_open())
        {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);

            traceFile << "\n========================================" << endl;
            traceFile << "UQFF TRACING SESSION ENDED" << endl;
            traceFile << "Time: " << put_time(localtime(&now_c), "%Y-%m-%d %H:%M:%S") << endl;
            traceFile << "========================================\n"
                      << endl;

            traceFile.close();
            enabled = false;
        }

        unlock();
    }

    // Delete copy constructor and assignment operator
    UQFFTracer(const UQFFTracer &) = delete;
    UQFFTracer &operator=(const UQFFTracer &) = delete;
};

// Convenience macros for tracing
#define TRACE_INIT(filename) UQFFTracer::getInstance().initialize(filename)
#define TRACE_SHUTDOWN() UQFFTracer::getInstance().shutdown()
#define TRACE_EVENT(msg, level) UQFFTracer::getInstance().logEvent(msg, level)
#define TRACE_METRIC(name, value, unit) UQFFTracer::getInstance().logMetric(name, value, unit)
#define TRACE_SPAN(name, type) auto span_##__LINE__ = UQFFTracer::getInstance().createSpan(name, type)

#endif // UQFF_TRACING_H
