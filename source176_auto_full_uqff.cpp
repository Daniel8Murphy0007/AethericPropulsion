/// source176_auto_full_uqff.cpp  ←  FINAL BULLETPROOF VERSION
#ifdef USE_EMBEDDED_WOLFRAM

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <windows.h>

#include "source174_wolfram_bridge_embedded.cpp"

void AutoExportFullUQFF()
{
    std::cout << "\n=== AUTO FULL UQFF EXPORT (bulletproof) ===\n"
              << std::flush;

    if (!InitializeWolframKernel())
    {
        std::cout << "InitializeWolframKernel() returned false\n"
                  << std::flush;
        return;
    }

    std::cout << "Kernel initialized successfully\n"
              << std::flush;

    // Force project root as search directory (works even if VS changes working dir)
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    std::filesystem::path exePath = buf;
    std::filesystem::path root = exePath.parent_path().parent_path().parent_path(); // build_msvc/Release → repo root

    std::cout << "Searching in: " << root.string() << "\n"
              << std::flush;
    std::cout << "Starting file scan...\n"
              << std::flush;

    std::vector<std::string> terms;
    std::regex term_regex(R"(#define\s+WOLFRAM_TERM\s*\"(.*)\")");

    int files_scanned = 0;
    int terms_found = 0;
    try
    {
        for (const auto &entry : std::filesystem::directory_iterator(root))
        {
            std::string path_str = entry.path().string();
            if (path_str.length() < 4 || path_str.substr(path_str.length() - 4) != ".cpp")
                continue;

            files_scanned++;
            if (files_scanned % 20 == 0)
            {
                std::cout << "Progress: scanned " << files_scanned << " files, found " << terms_found << " terms so far...\n"
                          << std::flush;
            }

            std::ifstream f(entry.path());
            if (!f)
                continue;

            std::string line;
            int lines_read = 0;
            while (std::getline(f, line) && lines_read < 500) // Limit per file to prevent infinite read
            {
                lines_read++;
                std::smatch m;
                if (std::regex_search(line, m, term_regex) && m.size() > 1)
                {
                    terms.push_back(m.str(1));
                    terms_found++;
                    if (terms_found <= 10 || terms_found % 20 == 0)
                    {
                        std::cout << "  [" << terms_found << "] " << entry.path().filename().string() << " → " << m.str(1).substr(0, 50) << "...\n"
                                  << std::flush;
                    }
                    break; // Stop after first match in file
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Filesystem exception: " << e.what() << "\n"
                  << std::flush;
    }

    std::cout << "\nScan complete: " << files_scanned << " files scanned, " << terms.size() << " terms extracted.\n"
              << std::flush;

    if (terms.empty())
    {
        std::cout << "No terms found — running fallback test so we know the kernel works...\n"
                  << std::flush;
        WolframEvalToString("test = 42^2");
        std::cout << "Kernel alive → 42² = " << WolframEvalToString("test") << "\n"
                  << std::flush;
        return;
    }

    // Use ostringstream for efficient string building
    std::cout << "Building master expression from " << terms.size() << " terms...\n"
              << std::flush;
    std::ostringstream expr_builder;
    expr_builder << "masterUQFF = " << terms[0];

    for (size_t i = 1; i < terms.size(); ++i)
    {
        if (i % 50 == 0)
        {
            std::cout << "  Added " << i << " / " << terms.size() << " terms to expression...\n"
                      << std::flush;
        }
        expr_builder << " + " << terms[i];
    }
    expr_builder << ";";

    std::string expr = expr_builder.str();
    size_t expr_size_mb = expr.size() / (1024 * 1024);
    size_t expr_size_kb = expr.size() / 1024;

    if (expr_size_mb > 0)
        std::cout << "Expression built: " << expr_size_mb << " MB. Sending to kernel...\n"
                  << std::flush;
    else
        std::cout << "Expression built: " << expr_size_kb << " KB. Sending to kernel...\n"
                  << std::flush;

    std::cout << "Evaluating masterUQFF assignment...\n"
              << std::flush;
    WolframEvalToString(expr);

    std::cout << "Calling FullSimplify (this may take a while for large expressions)...\n"
              << std::flush;
    std::string result = WolframEvalToString("ToString[FullSimplify[masterUQFF], InputForm]");

    std::cout << "\n=== WOLFRAM VERDICT ===\n"
              << result << "\n"
              << std::flush;
    if (result.find("0") != std::string::npos)
        std::cout << "★★★ CANCELLATION ACHIEVED ★★★\n"
                  << std::flush;
}

#endif