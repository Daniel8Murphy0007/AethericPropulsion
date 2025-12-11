// source178_grok_api.cpp
// Grok AI Integration for UQFF Error Diagnostics and Code Assistance
// xAI API Implementation using Qt6 QNetworkAccessManager
// Integration Date: 2025-11-22 18:50 (Phase 23: AI Integration)

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QDebug>
#include <QByteArray>
#include <QUrl>
#include <QString>
#include <iostream>

// Function to call xAI Grok API for chat completions
// Returns the AI response as QString; empty on error
QString callGrokAPI(const QString &prompt)
{
    // Retrieve API key from environment (set via: $env:XAI_API_KEY="your_key")
    QString apiKey = qgetenv("XAI_API_KEY");
    if (apiKey.isEmpty())
    {
        qWarning() << "[Grok API] XAI_API_KEY not set. Set environment variable XAI_API_KEY.";
        std::cout << "[Grok API] WARNING: XAI_API_KEY environment variable not found.\n";
        std::cout << "[Grok API] Set with PowerShell: $env:XAI_API_KEY=\"your_key_here\"\n";
        std::cout << "[Grok API] Get your free API key at: https://x.ai/api\n"
                  << std::flush;
        return "";
    }

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://api.x.ai/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + apiKey).toUtf8());

    // Build JSON payload
    QJsonObject payload;
    payload["model"] = "grok-2-1212"; // Use grok-2-1212 model (latest available as of Dec 2024)

    QJsonArray messages;

    // System message - AI personality and context
    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = "You are Grok, a highly intelligent AI from xAI with expertise in C++ physics simulations. "
                               "You are assisting with the UQFF (Unified Quantum Field Framework) project - a 91,384-line C++20 codebase "
                               "implementing 894 PhysicsTerm classes for quantum field unity calculations. "
                               "Provide concise, accurate explanations for compilation errors, suggest fixes, and explain physics equations. "
                               "Focus on MSVC 14.44+, Qt6, ANTLR4, SymEngine, and Wolfram WSTP integration issues.";
    messages.append(systemMessage);

    // User message - the actual prompt
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = prompt;
    messages.append(userMessage);

    payload["messages"] = messages;
    payload["stream"] = false;    // Non-streaming for simplicity
    payload["temperature"] = 0.7; // Balanced creativity vs accuracy
    payload["max_tokens"] = 2048; // Allow detailed responses

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    std::cout << "[Grok API] Sending request to xAI API...\n"
              << std::flush;

    // Send POST request
    QNetworkReply *reply = manager.post(request, data);

    // Wait for response (synchronous - use signals/slots for async in production)
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString responseContent;
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseData = reply->readAll();
        QJsonDocument respDoc = QJsonDocument::fromJson(responseData);

        if (!respDoc.isNull() && respDoc.isObject())
        {
            QJsonObject respObj = respDoc.object();

            // Extract AI response from choices array
            if (respObj.contains("choices") && respObj["choices"].isArray())
            {
                QJsonArray choices = respObj["choices"].toArray();
                if (!choices.isEmpty())
                {
                    QJsonObject choice = choices[0].toObject();
                    if (choice.contains("message") && choice["message"].isObject())
                    {
                        QJsonObject message = choice["message"].toObject();
                        if (message.contains("content"))
                        {
                            responseContent = message["content"].toString();
                        }
                    }
                }
            }

            // Log usage for tracking (tokens, cost estimation)
            if (respObj.contains("usage"))
            {
                QJsonObject usage = respObj["usage"].toObject();
                int promptTokens = usage["prompt_tokens"].toInt();
                int completionTokens = usage["completion_tokens"].toInt();
                int totalTokens = usage["total_tokens"].toInt();

                std::cout << "[Grok API] Token Usage - Prompt: " << promptTokens
                          << ", Completion: " << completionTokens
                          << ", Total: " << totalTokens << "\n"
                          << std::flush;

                qDebug() << "[Grok API] Usage - Prompt tokens:" << promptTokens
                         << "Completion tokens:" << completionTokens
                         << "Total tokens:" << totalTokens;
            }

            std::cout << "[Grok API] Response received successfully.\n"
                      << std::flush;
        }
        else
        {
            qWarning() << "[Grok API] Invalid JSON response from xAI API.";
            std::cout << "[Grok API] ERROR: Invalid JSON response.\n"
                      << std::flush;
        }
    }
    else
    {
        // Handle API errors
        qWarning() << "[Grok API] Error:" << reply->errorString();
        std::cout << "[Grok API] Network Error: " << reply->errorString().toStdString() << "\n"
                  << std::flush;

        // Parse error JSON for detailed diagnostics
        QByteArray errorData = reply->readAll();
        QJsonDocument errorDoc = QJsonDocument::fromJson(errorData);
        if (!errorDoc.isNull())
        {
            qWarning() << "[Grok API] Error details:" << errorDoc.toJson(QJsonDocument::Compact);
            std::cout << "[Grok API] Error details: " << errorDoc.toJson(QJsonDocument::Compact).toStdString() << "\n"
                      << std::flush;
        }
    }

    reply->deleteLater();
    return responseContent;
}

// Convenience wrapper for C++ error diagnostics
QString diagnoseCompilationError(const QString &errorMessage, const QString &sourceFile = "", int lineNumber = -1)
{
    QString prompt = "C++ Compilation Error Diagnostic:\n\n";
    prompt += "Error Message: " + errorMessage + "\n";

    if (!sourceFile.isEmpty())
    {
        prompt += "Source File: " + sourceFile + "\n";
    }

    if (lineNumber > 0)
    {
        prompt += "Line Number: " + QString::number(lineNumber) + "\n";
    }

    prompt += "\nContext: This is from the UQFF (Unified Quantum Field Framework) project, a C++20 codebase with:\n";
    prompt += "- Compiler: MSVC 14.44+ (Visual Studio 2022)\n";
    prompt += "- Dependencies: Qt6, ANTLR4, SymEngine, Wolfram WSTP\n";
    prompt += "- Physics: 894 PhysicsTerm classes for quantum field calculations\n\n";
    prompt += "Please explain the error and suggest a fix.";

    return callGrokAPI(prompt);
}

// Wrapper for physics equation assistance
QString explainPhysicsEquation(const QString &equationName, const QString &equationCode = "")
{
    QString prompt = "Physics Equation Explanation:\n\n";
    prompt += "Equation: " + equationName + "\n\n";

    if (!equationCode.isEmpty())
    {
        prompt += "C++ Implementation:\n" + equationCode + "\n\n";
    }

    prompt += "Context: This is from the UQFF (Unified Quantum Field Framework) - a quantum field unity theory.\n";
    prompt += "Please explain the physics behind this equation and its role in field unification.";

    return callGrokAPI(prompt);
}

// Wrapper for code review and optimization suggestions
QString reviewPhysicsCode(const QString &codeSnippet, const QString &concernedAspect = "performance")
{
    QString prompt = "C++ Code Review for Physics Simulation:\n\n";
    prompt += "Code:\n" + codeSnippet + "\n\n";
    prompt += "Review Focus: " + concernedAspect + "\n\n";
    prompt += "Context: UQFF quantum field calculations with C++20, MSVC optimization (/Os /GL /LTCG).\n";
    prompt += "Please review for correctness, performance, and suggest improvements.";

    return callGrokAPI(prompt);
}

// Test function to verify Grok API connectivity
void testGrokAPI()
{
    std::cout << "\n=== Testing Grok API Connectivity ===\n"
              << std::flush;

    QString testPrompt = "Hello Grok! Please confirm you can assist with C++ physics simulations. "
                         "Respond with a brief message acknowledging your capabilities.";

    QString response = callGrokAPI(testPrompt);

    if (!response.isEmpty())
    {
        std::cout << "\n[Grok API Test] SUCCESS!\n"
                  << std::flush;
        std::cout << "Grok Response:\n"
                  << response.toStdString() << "\n"
                  << std::flush;
    }
    else
    {
        std::cout << "\n[Grok API Test] FAILED - No response received.\n"
                  << std::flush;
        std::cout << "Check:\n";
        std::cout << "1. XAI_API_KEY environment variable is set\n";
        std::cout << "2. Internet connection is active\n";
        std::cout << "3. API key is valid (get free key at https://x.ai/api)\n"
                  << std::flush;
    }

    std::cout << "=====================================\n\n"
              << std::flush;
}
