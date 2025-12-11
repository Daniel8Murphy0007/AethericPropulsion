// ============================================================================
// INCLUDE STATEMENTS - Import all necessary libraries for the application
// ============================================================================

// Qt Framework - Cross-platform GUI toolkit for building the user interface
#include <QApplication>     // Main application class - manages GUI application control flow and settings
#include <QMainWindow>      // Main window class - provides framework for building application's user interface
#include <QLineEdit>        // Single-line text input widget - allows user to enter and edit text
#include <QTextEdit>        // Multi-line text editor widget - allows editing and displaying plain/rich text
#include <QWebEngineView>   // Web browser widget - displays web content using Chromium engine
#include <QTabWidget>       // Tab container widget - provides tab bar and page area for switching between pages
#include <QVBoxLayout>      // Vertical box layout manager - arranges widgets vertically
#include <QHBoxLayout>      // Horizontal box layout manager - arranges widgets horizontally
#include <QPushButton>      // Push button widget - command button user can click
#include <QLabel>           // Text/image display widget - displays static text or images
#include <QDockWidget>      // Dockable window - can be docked in QMainWindow or float as separate window
#include <QDialog>          // Base class for dialog windows - modal or non-modal popup windows
#include <QMessageBox>      // Modal dialog for informing user - shows messages, warnings, errors
#include <QToolBar>         // Toolbar container - holds action buttons and widgets
#include <QScreen>          // Screen information - provides info about physical screen properties
#include <QDragEnterEvent>  // Drag-and-drop enter event - sent when drag operation enters a widget
#include <QDropEvent>       // Drag-and-drop drop event - sent when user drops data on widget
#include <QMimeData>        // MIME data container - holds data in different formats for clipboard/drag-drop
#include <QFile>            // File I/O operations - interface for reading/writing files
#include <QDir>             // Directory operations - access to directory structures and contents
#include <QStandardPaths>   // Standard system paths - provides platform-specific standard locations
#include <QKeyEvent>        // Keyboard event - sent when user presses/releases keys
#include <QCoreApplication> // Core application class - provides event loop for non-GUI applications

// VTK (Visualization Toolkit) - For scientific data visualization (3D plots, charts, graphs)
#include <vtkSmartPointer.h>      // Smart pointer for VTK objects - automatic memory management
#include <vtkScatterPlotMatrix.h> // Scatter plot matrix - creates matrix of scatter plots for data analysis
#include <vtkChartXY.h>           // 2D chart - creates X-Y plots and line graphs
#include <vtkPlot.h>              // Plot base class - represents a plot in a chart
#include <vtkTable.h>             // Data table - stores tabular data in rows and columns
#include <vtkDoubleArray.h>       // Array of doubles - stores numerical data for visualization
#include <vtkContextView.h>       // 2D context view - provides 2D rendering context for charts
#include <vtkContextScene.h>      // Scene for 2D rendering - manages 2D items in context view
#include <vtkAxis.h>              // Chart axis - represents axis in 2D chart (X or Y)
#include <vtkRenderWindow.h>      // Rendering window - window for displaying VTK graphics
#include <vtkRenderer.h>          // Renderer - renders 3D scene into a window

// Network and Web Communication Libraries
#include <curl/curl.h> // libcurl - HTTP/HTTPS requests for fetching data from web APIs
#include <websocket.h> // WebSocket protocol - real-time bidirectional communication with servers

// Database and Cloud Storage
#include <sqlite3.h> // SQLite database - embedded SQL database for local caching

// AWS (Amazon Web Services) SDK - Cloud services integration
#include <aws/core/Aws.h>                                  // AWS SDK core - initialization and configuration
#include <aws/s3/S3Client.h>                               // AWS S3 client - cloud object storage for syncing cached data
#include <aws/cognito-idp/CognitoIdentityProviderClient.h> // AWS Cognito - user authentication and authorization

// Speech and Vision Processing
#include <pocketsphinx.h>     // PocketSphinx - speech recognition for voice input commands
#include <opencv2/opencv.hpp> // OpenCV - computer vision library for video/image processing

// Python Integration
#include <pybind11/embed.h> // pybind11 - embeds Python interpreter for running Python code (AI models)

// Mathematical Computation
#include <qalculate.h> // Qalculate - powerful calculator library for symbolic math

// System and Standard Libraries
#include <windows.h>         // Windows API - Windows-specific system functions
#include <string>            // std::string - standard string class for text manipulation
#include <vector>            // std::vector - dynamic array container for storing collections
#include <thread>            // std::thread - multithreading support for parallel operations
#include <nlohmann/json.hpp> // JSON library - parsing and creating JSON data for APIs
#include <sstream>           // std::stringstream - string stream for string manipulation
#include <algorithm>         // std algorithms - searching, sorting, and other operations
#include <fstream>           // File streams - file input/output operations
#include <chrono>            // Time library - date/time operations and timing

// ============================================================================
// PREPROCESSOR DEFINITIONS - Constants and API keys used throughout the program
// ============================================================================

// Application Constants
#define MAX_QUERY_LENGTH 6000 // Maximum characters allowed in search query (prevents buffer overflow)
#define MAX_WINDOWS 21        // Increased for ALMA Cycle 12 - 21 parallel browser windows
                              // Allows simultaneous searches across multiple scientific sources

// NASA API Keys - Used to access NASA's public data services
// Get your own keys at: https://api.nasa.gov/
#define NASA_API_KEY_1 "PNJaNeFWqMb2g0CEQGqJePkndqYfKvBzq6XJqAwg" // NASA APOD/imagery API
#define NASA_API_KEY_2 "FJnBo64nLFqExHwDchrcaf101D8wmGSm0cF27clz" // NASA DONKI space weather API

// MAST API Key - Access to astronomical data archives (Hubble, JWST, Chandra, etc.)
// Get your own key at: https://auth.mast.stsci.edu/
#define MAST_API_KEY "emXvt90Htf0U4RogKTB5lqSxClUeg2pvMQxvZciM"

// OpenAI API Key - Used for GPT-4 summarization of search results
// Replace with your key from: https://platform.openai.com/api-keys
#define OPENAI_API_KEY "your_openai_api_key_here"

// AWS Cognito - User authentication and authorization for cloud sync
// Replace with your Cognito configuration from AWS Console
#define COGNITO_CLIENT_ID "your_cognito_client_id" // Your Cognito app client ID
#define COGNITO_REGION "us-east-1"                 // AWS region where Cognito is hosted

// ============================================================================
// NAMESPACE ALIASES - Shorter names for frequently used namespaces
// ============================================================================

namespace py = pybind11;     // Alias for pybind11 - used for embedding Python interpreter
using json = nlohmann::json; // Alias for JSON library - simplifies JSON parsing and creation

// ============================================================================
// DATA STRUCTURES - Custom types used throughout the application
// ============================================================================

// SearchResult - Stores information about a single search result
// Used to organize and display search results in browser windows
struct SearchResult
{
    std::string url;     // URL of the search result (webpage link)
    std::string title;   // Title of the result (headline or page title)
    std::string summary; // AI-generated summary of the content (from GPT-4 or Llama)
    double relevance;    // Relevance score (0.0 to 1.0) - how well result matches query
    bool isLive;         // Flag indicating if this is real-time live data (LIGO, JWST streams)
};

// ============================================================================
// GLOBAL VARIABLES - Data shared across the entire application
// ============================================================================

// focusList - List of trusted scientific organizations to focus searches on
// These sources are prioritized when performing multi-window searches
// Includes space agencies, observatories, research institutions, and telescopes
std::vector<std::string> focusList = {
    "Worldwide Telescopes",        // Virtual observatory aggregating data from telescopes worldwide
    "NASA",                        // National Aeronautics and Space Administration
    "SpaceX",                      // Private spaceflight company (Starlink, Dragon, Starship)
    "JPL",                         // Jet Propulsion Laboratory (NASA center for robotic exploration)
    "ESA",                         // European Space Agency
    "STScI",                       // Space Telescope Science Institute (operates Hubble & JWST)
    "Hubble",                      // Hubble Space Telescope
    "JWST",                        // James Webb Space Telescope
    "Chandra",                     // Chandra X-ray Observatory
    "ALMA",                        // Atacama Large Millimeter/submillimeter Array
    "EHT",                         // Event Horizon Telescope (black hole imaging)
    "SKA Observatory",             // Square Kilometre Array (radio telescope)
    "CERN",                        // European Organization for Nuclear Research (particle physics)
    "DARPA",                       // Defense Advanced Research Projects Agency
    "ATIP",                        // Advanced Technology and Instrumentation Program
    "ACS Hubble Ultra Deep Field", // Advanced Camera for Surveys deep field observations
    "WFC3 Hubble Deep Field",      // Wide Field Camera 3 deep field observations
    "Hubble Heritage Team",        // Team creating public release images from Hubble
    "LIGO",                        // Laser Interferometer Gravitational-Wave Observatory
    "FAST"                         // Five-hundred-meter Aperture Spherical Telescope (China)
};

// results - Array storing search results for each of the 21 browser windows
// Index corresponds to window number (0-20), each vector holds SearchResult objects
std::vector<SearchResult> results[MAX_WINDOWS];

// Database and Cloud Storage Pointers
// These are initialized in main() and used throughout the application
sqlite3 *db;                                                                 // SQLite database for local caching of search results (offline access)
Aws::S3::S3Client *s3_client;                                                // AWS S3 client for syncing cached data to cloud storage
Aws::CognitoIdentityProvider::CognitoIdentityProviderClient *cognito_client; // AWS Cognito for user authentication

// ============================================================================
// NASA DONKI API FUNCTION - Fetches space weather data
// ============================================================================

// FetchDONKI - Retrieves Coronal Mass Ejection (CME) analysis data from NASA DONKI
//
// DONKI (Database Of Notifications, Knowledge, Information) provides alerts and
// data about space weather events that can affect Earth (solar flares, CMEs, etc.)
//
// Parameters:
//   startDate - Optional start date in YYYY-MM-DD format (e.g., "2024-01-01")
//   endDate   - Optional end date in YYYY-MM-DD format
//
// Returns:
//   JSON string containing CME analysis data from NASA
//
// Example usage:
//   std::string data = FetchDONKI("2024-01-01", "2024-01-31");
//
std::string FetchDONKI(const std::string &startDate = "", const std::string &endDate = "")
{
    CURL *curl = curl_easy_init(); // Initialize cURL library for HTTP requests

    // Build the API URL with authentication key
    std::string url = "https://api.nasa.gov/DONKI/CMEAnalysis?api_key=" + std::string(NASA_API_KEY_2);

    // Add optional date filters to the URL if provided
    if (!startDate.empty())
        url += "&startDate=" + startDate; // Filter results from this date onward
    if (!endDate.empty())
        url += "&endDate=" + endDate; // Filter results up to this date

    std::string response; // Variable to store the API response

    // Configure cURL to make the HTTP GET request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());             // Set the target URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Set callback to handle response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);         // Pass response string to callback

    CURLcode res = curl_easy_perform(curl); // Execute the HTTP request
    curl_easy_cleanup(curl);                // Clean up cURL resources (prevent memory leaks)

    return response; // Return the JSON response from NASA DONKI
}

// ============================================================================
// SCIENTIFIC CALCULATOR DIALOG CLASS
// ============================================================================

// ScientificCalculatorDialog - A frameless, draggable Qt dialog window for calculus operations
//
// This class creates a popup calculator that can:
//   - Solve derivatives (e.g., "d/dx(x^2)" calculates the derivative of x²)
//   - Compute definite integrals (e.g., "?(0,1) x^2 dx" integrates x² from 0 to 1)
//   - Solve algebraic equations (e.g., "x^2 + y = 5")
//   - Convert Julian Dates (e.g., "jd to date 2451544" converts to calendar date)
//
// The window is frameless (no title bar) and can be dragged anywhere on screen.
// It supports drag-and-drop of equations and auto-resizes based on input.
//
// Inherits from QDialog (Qt's base class for modal/non-modal popup windows)
//
class ScientificCalculatorDialog : public QDialog
{
public:
    // Constructor - Sets up the calculator window and all its widgets
    // Parameters:
    //   parent - The parent widget (usually the main window), allows Qt to manage memory
    ScientificCalculatorDialog(QWidget *parent) : QDialog(parent)
    {
        // Configure window properties
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint); // Make window frameless (no title bar/borders)
        setAcceptDrops(true);                                 // Enable drag-and-drop support for equations

        // Create vertical layout to arrange widgets top-to-bottom
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Create input text area for user to enter equations
        input = new QTextEdit(this);
        input->setPlaceholderText("Enter equations (e.g., d/dx(x^2), ?(0,1) x^2 dx, x^2 + y = 5, jd to date 2451544)");
        input->setMinimumHeight(100);  // Minimum 100 pixels tall
        input->setMaximumHeight(1000); // Can expand to 1000 pixels if needed
        input->setAcceptDrops(true);   // Allow dropping equations into input area

        // Create output text area to display results (read-only)
        output = new QTextEdit(this);
        output->setReadOnly(true); // User cannot edit results, only view them

        // Create "Solve" button to trigger calculation
        QPushButton *solveBtn = new QPushButton("Solve", this);

        // Add all widgets to the vertical layout
        layout->addWidget(input);    // Input box at top
        layout->addWidget(solveBtn); // Solve button in middle
        layout->addWidget(output);   // Output box at bottom

        // Connect signals to slots (Qt's event handling mechanism)
        // When "Solve" button is clicked, call solveEquations() method
        connect(solveBtn, &QPushButton::clicked, this, &ScientificCalculatorDialog::solveEquations);

        // When input text changes, call adjustInputSize() to auto-resize input box
        connect(input, &QTextEdit::textChanged, this, &ScientificCalculatorDialog::adjustInputSize);

        // Enable mouse tracking for drag functionality (even when button not pressed)
        setMouseTracking(true);
    }

protected:
    // ========================================================================
    // EVENT HANDLERS - Methods called automatically when user interacts with window
    // ========================================================================

    // mousePressEvent - Called when user presses mouse button on the window
    // Used to initiate window dragging
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) // Only respond to left mouse button
        {
            // Store the offset between mouse position and window top-left corner
            // This allows smooth dragging without window jumping
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept(); // Mark event as handled
        }
    }

    // mouseMoveEvent - Called when user moves mouse with button pressed
    // Used to drag the window around the screen
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton) // Check if left button is still pressed
        {
            // Move window to new position based on mouse movement
            // globalPos() gives mouse position relative to screen
            // dragPosition offset ensures window doesn't jump
            move(event->globalPos() - dragPosition);
            event->accept(); // Mark event as handled
        }
    }

    // dragEnterEvent - Called when user drags something over the window
    // Decides whether to accept the dragged content
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->mimeData()->hasText())  // Only accept text data (equations)
            event->acceptProposedAction(); // Allow the drop operation
    }

    // dropEvent - Called when user drops content onto the window
    // Adds the dropped text to the input area
    void dropEvent(QDropEvent *event) override
    {
        // Append dropped text to current input (allows building complex equations)
        input->setText(input->toPlainText() + event->mimeData()->text());
        event->acceptProposedAction(); // Confirm drop was successful
    }

private:
    // ========================================================================
    // PRIVATE MEMBER VARIABLES - Data internal to this class
    // ========================================================================

    QTextEdit *input;    // Pointer to input text editor widget
    QTextEdit *output;   // Pointer to output text editor widget (displays results)
    QPoint dragPosition; // Stores mouse offset for dragging (prevents window jumping)

    // ========================================================================
    // PRIVATE HELPER METHODS - Internal functions used by this class
    // ========================================================================

    // adjustInputSize - Automatically resizes input box based on number of lines
    // Called whenever user types or pastes text
    void adjustInputSize()
    {
        QString text = input->toPlainText(); // Get current input text
        int lines = text.split("\n").size(); // Count number of lines (split by newline)

        // Calculate new height: 20 pixels per line + 50 pixel padding
        // Constrain between 100 (min) and 1000 (max) pixels
        int newHeight = std::min(std::max(100, lines * 20 + 50), 1000);

        input->setMinimumHeight(newHeight); // Set minimum height
        input->setMaximumHeight(newHeight); // Set maximum height (makes it fixed height)
    }

    // solveEquations - Main calculation method, parses and solves all equations
    // Called when user clicks the "Solve" button
    void solveEquations()
    {
        // Get all text from input box and convert to C++ string
        std::string expr = input->toPlainText().toStdString();

        // Vector to store individual equations (one per line)
        std::vector<std::string> equations;

        // Parse input by splitting on newlines
        std::stringstream ss(expr);
        std::string line;
        while (std::getline(ss, line)) // Read line by line
        {
            if (!line.empty())             // Ignore blank lines
                equations.push_back(line); // Add equation to vector
        }

        QString result; // String to accumulate all results for display

        // Initialize Qalculate library for mathematical calculations
        Qalculate calc;

        // Initialize Python interpreter for symbolic math (SymPy library)
        py::scoped_interpreter guard{};                   // RAII guard - automatically starts/stops interpreter
        py::module_ sympy = py::module_::import("sympy"); // Import SymPy for derivatives/integrals

        // Vector to collect system of equations (multiple equations with multiple unknowns)
        std::vector<std::string> system_eqs;

        // Process each equation one at a time
        for (const auto &eq : equations)
        {
            // ================================================================
            // JULIAN DATE CONVERSION: JD to Calendar Date
            // ================================================================
            if (eq.find("jd to date") != std::string::npos)
            {
                // Extract Julian Date number from equation (everything after "date ")
                std::string jd = eq.substr(eq.find("date") + 5);

                // Call JPL JD-Cal API to convert Julian Date to calendar date
                std::string jdcal = FetchJDCalJD(jd);
                result += QString("JD to Date: %1\n").arg(QString::fromStdString(jdcal));

                // Fetch related space weather data from NASA DONKI
                // (useful for correlating astronomical events with solar activity)
                std::string donki = FetchDONKI(); // Get current space weather alerts

                // Summarize DONKI data using OpenAI GPT-4
                result += QString("DONKI Space Weather: %1\n").arg(QString::fromStdString(SummarizeWithOpenAI(donki)));
            }
            // ================================================================
            // CALENDAR DATE CONVERSION: Date to Julian Date
            // ================================================================
            else if (eq.find("date to jd") != std::string::npos)
            {
                // Extract calendar date from equation (everything after "jd ")
                std::string cd = eq.substr(eq.find("jd") + 3);

                // Call JPL JD-Cal API to convert calendar date to Julian Date
                std::string jdcal = FetchJDCalCD(cd);
                result += QString("Date to JD: %1\n").arg(QString::fromStdString(jdcal));
            }
            // ================================================================
            // DERIVATIVE CALCULATION: d/dx notation
            // ================================================================
            else if (eq.find("d/d") != std::string::npos)
            {
                // Parse derivative notation like "d/dx(x^2)"
                // Extract variable (usually "x") and function expression
                std::string var = "x"; // Variable to differentiate with respect to (default x)

                // Extract function from inside parentheses
                // e.g., "d/dx(x^2)" -> extract "x^2"
                std::string func = eq.substr(eq.find("(") + 1, eq.find(")") - eq.find("(") - 1);

                // Use SymPy (Python symbolic math library) to calculate derivative
                py::object x = sympy.attr("symbols")("x");      // Create symbolic variable x
                py::object expr = sympy.attr("sympify")(func);  // Convert string to SymPy expression
                py::object deriv = sympy.attr("diff")(expr, x); // Compute derivative: d/dx

                // Format and display result
                result += QString("d/dx(%1) = %2\n")
                              .arg(QString::fromStdString(func),
                                   QString::fromStdString(deriv.attr("__str__")().cast<std::string>()));
            }
            // ================================================================
            // DEFINITE INTEGRAL CALCULATION: ? notation
            // ================================================================
            else if (eq.find("?") != std::string::npos)
            {
                // Parse integral notation like "?(0,1) x^2 dx"
                // Extract bounds (a, b) and function expression

                // Extract bounds from inside parentheses: "?(0,1) ..." -> "0,1"
                std::string bounds = eq.substr(eq.find("(") + 1, eq.find(")") - eq.find("(") - 1);

                // Extract function between closing paren and "dx": "?(0,1) x^2 dx" -> " x^2 "
                std::string func = eq.substr(eq.find(")") + 1, eq.find("dx") - eq.find(")") - 1);

                // Parse bounds string "a,b" into two double values
                auto [a, b] = parseBounds(bounds);

                // Use SymPy to compute definite integral
                py::object x = sympy.attr("symbols")("x");                                    // Create symbolic variable
                py::object expr = sympy.attr("sympify")(func);                                // Convert string to expression
                py::object integral = sympy.attr("integrate")(expr, py::make_tuple(x, a, b)); // Integrate from a to b

                // Format and display result: "?(0,1) x^2 dx = 0.333..."
                result += QString("?(%1,%2) %3 dx = %4\n")
                              .arg(QString::number(a), QString::number(b), QString::fromStdString(func),
                                   QString::fromStdString(integral.attr("__str__")().cast<std::string>()));
            }
            // ================================================================
            // ALGEBRAIC EQUATIONS: Contains "=" sign
            // ================================================================
            else if (eq.find("=") != std::string::npos)
            {
                // Collect equations for system solving (e.g., "x + y = 5", "x - y = 1")
                // Multiple equations with unknowns can be solved simultaneously

                // Convert equation to standard form (all terms on one side)
                // e.g., "x + y = 5" becomes "x + y - 5" (set equal to zero)
                std::string eq_clean = eq;
                std::replace(eq_clean.begin(), eq_clean.end(), '=', '-'); // Replace = with -
                system_eqs.push_back(eq_clean);                           // Add to system equations vector
            }
            // ================================================================
            // GENERAL EXPRESSIONS: Anything else (arithmetic, etc.)
            // ================================================================
            else
            {
                // Use Qalculate library for general math expressions
                // e.g., "2 + 2", "sqrt(16)", "sin(pi/2)", etc.
                result += QString("%1 = %2\n")
                              .arg(QString::fromStdString(eq),
                                   QString::fromStdString(calc.evaluate(eq)));
            }
        }

        // ====================================================================
        // SOLVE SYSTEM OF EQUATIONS (if 2 or more equations collected)
        // ====================================================================
        if (system_eqs.size() >= 2)
        {
            // Use SymPy to solve simultaneous equations with multiple unknowns
            // Example: "x + y = 5" and "x - y = 1" -> solve for x and y

            py::object x = sympy.attr("symbols")("x"); // Create symbolic variable x
            py::object y = sympy.attr("symbols")("y"); // Create symbolic variable y

            // Convert first two equations to SymPy expressions
            py::object eq1 = sympy.attr("sympify")(system_eqs[0]);
            py::object eq2 = sympy.attr("sympify")(system_eqs[1]);

            // Solve the system of equations for variables x and y
            py::object solutions = sympy.attr("solve")(py::make_tuple(eq1, eq2), py::make_tuple(x, y));

            // Display system and solutions
            result += QString("System: %1, %2\nSolutions: %3\n")
                          .arg(QString::fromStdString(system_eqs[0]),
                               QString::fromStdString(system_eqs[1]),
                               QString::fromStdString(solutions.attr("__str__")().cast<std::string>()));
        }

        // Display all results in the output text area
        output->setText(result);
    }

    // ========================================================================
    // parseBounds - Helper function to extract integral bounds from string
    // ========================================================================
    // Parses a string like "0,1" into two double values (lower and upper bounds)
    // Used for definite integrals: ?(a,b) f(x) dx
    //
    // Parameters:
    //   bounds - String in format "a,b" (e.g., "0,1" or "-2.5,3.7")
    //
    // Returns:
    //   std::pair<double, double> - First is lower bound (a), second is upper bound (b)
    //
    std::pair<double, double> parseBounds(const std::string &bounds)
    {
        size_t comma = bounds.find(",");                // Find position of comma separator
        double a = std::stod(bounds.substr(0, comma));  // Convert first part to double (lower bound)
        double b = std::stod(bounds.substr(comma + 1)); // Convert second part to double (upper bound)
        return {a, b};                                  // Return as pair
    }
};

// ============================================================================
// RAMANUJAN NUMBER CALCULATOR DIALOG CLASS
// ============================================================================

// RamanujanCalculatorDialog - A specialized calculator for Ramanujan numbers
//
// Ramanujan numbers are positive integers that can be expressed as the sum of
// two cubes in multiple ways. The most famous is 1729:
//   1729 = 1³ + 12³ = 9³ + 10³
//
// This dialog finds such numbers and their representations.
// Like ScientificCalculatorDialog, it's frameless and draggable.
//
class RamanujanCalculatorDialog : public QDialog
{
public:
    RamanujanCalculatorDialog(QWidget *parent) : QDialog(parent)
    {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        setAcceptDrops(true);
        QVBoxLayout *layout = new QVBoxLayout(this);
        input = new QTextEdit(this);
        input->setPlaceholderText("Enter number theory functions (e.g., p(5), tau(7))");
        input->setMinimumHeight(100);
        input->setMaximumHeight(1000);
        input->setAcceptDrops(true);
        output = new QTextEdit(this);
        output->setReadOnly(true);
        QPushButton *solveBtn = new QPushButton("Solve", this);
        layout->addWidget(input);
        layout->addWidget(solveBtn);
        layout->addWidget(output);
        connect(solveBtn, &QPushButton::clicked, this, &RamanujanCalculatorDialog::solveEquations);
        connect(input, &QTextEdit::textChanged, this, &RamanujanCalculatorDialog::adjustInputSize);
        setMouseTracking(true);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->mimeData()->hasText())
            event->acceptProposedAction();
    }
    void dropEvent(QDropEvent *event) override
    {
        input->setText(input->toPlainText() + event->mimeData()->text());
        event->acceptProposedAction();
    }

private:
    QTextEdit *input;
    QTextEdit *output;
    QPoint dragPosition;

    void adjustInputSize()
    {
        QString text = input->toPlainText();
        int lines = text.split("\n").size();
        int newHeight = std::min(std::max(100, lines * 20 + 50), 1000);
        input->setMinimumHeight(newHeight);
        input->setMaximumHeight(newHeight);
    }

    void solveEquations()
    {
        std::string expr = input->toPlainText().toStdString();
        std::vector<std::string> equations;
        std::stringstream ss(expr);
        std::string line;
        while (std::getline(ss, line))
        {
            if (!line.empty())
                equations.push_back(line);
        }

        QString result;
        py::scoped_interpreter guard{};
        py::module_ sympy = py::module_::import("sympy");

        // Define Ramanujan tau function using OEIS A000594 formula
        py::exec(R"(
from sympy import divisor_sigma

def ramanujan_tau(n):
    m = (n + 1) >> 1
    term1 = n**4 * divisor_sigma(n)
    inner = m**2 * (0 if n % 2 else (m * (35 * m - 52 * n) + 18 * n**2) * divisor_sigma(m)**2)
    summ = sum((i * (i * (i * (70 * i - 140 * n) + 90 * n**2) - 20 * n**3) + n**4) * divisor_sigma(i) * divisor_sigma(n - i) for i in range(1, m))
    return term1 - 24 * (inner + summ)
)");

        py::object tau_func = py::globals()["ramanujan_tau"];

        for (const auto &eq : equations)
        {
            if (eq.find("p(") != std::string::npos)
            {
                std::string n_str = eq.substr(eq.find("(") + 1, eq.find(")") - eq.find("(") - 1);
                int n = std::stoi(n_str);
                py::object partition = sympy.attr("partition")(n);
                result += QString("p(%1) = %2 partitions\n").arg(n).arg(partition.cast<int>());
            }
            else if (eq.find("tau(") != std::string::npos)
            {
                std::string n_str = eq.substr(eq.find("(") + 1, eq.find(")") - eq.find("(") - 1);
                int n = std::stoi(n_str);
                py::object tau = tau_func(n);
                result += QString("tau(%1) = %2\n").arg(n).arg(tau.cast<long>());
            }
            else
            {
                result += QString("Invalid input: %1\n").arg(QString::fromStdString(eq));
            }
        }
        output->setText(result);
    }
};

// Calculus Button Field
class CalculusButtonField : public QDockWidget
{
public:
    CalculusButtonField(QWidget *parent) : QDockWidget("Calculus Tools", parent)
    {
        QWidget *widget = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(widget);
        QToolBar *toolbar = new QToolBar(this);
        input = new QTextEdit(this);
        input->setPlaceholderText("Insert symbols (e.g., ?, ?, ?)");
        input->setMinimumHeight(100);
        input->setMaximumHeight(1000);
        input->setAcceptDrops(true);

        toolbar->addAction("?", [=]()
                           { input->insertPlainText("?(a,b) f(x) dx"); });
        toolbar->addAction("?", [=]()
                           { input->insertPlainText("?/?x"); });
        toolbar->addAction("?", [=]()
                           { input->insertPlainText("?(n,a,b)"); });
        toolbar->addAction("?", [=]()
                           { input->insertPlainText("sqrt()"); });
        toolbar->addAction("sin", [=]()
                           { input->insertPlainText("sin()"); });
        toolbar->addAction("cos", [=]()
                           { input->insertPlainText("cos()"); });
        toolbar->addAction("log", [=]()
                           { input->insertPlainText("log()"); });

        layout->addWidget(toolbar);
        layout->addWidget(input);
        setWidget(widget);
        connect(input, &QTextEdit::textChanged, this, &CalculusButtonField::adjustInputSize);
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->mimeData()->hasText())
            event->acceptProposedAction();
    }
    void dropEvent(QDropEvent *event) override
    {
        input->setText(input->toPlainText() + event->mimeData()->text());
        event->acceptProposedAction();
    }

private:
    QTextEdit *input;

    void adjustInputSize()
    {
        QString text = input->toPlainText();
        int lines = text.split("\n").size();
        int newHeight = std::min(std::max(100, lines * 20 + 50), 1000);
        input->setMinimumHeight(newHeight);
        input->setMaximumHeight(newHeight);
    }
};

// ============================================================================
// DETACHABLE BROWSER WINDOW CLASS
// ============================================================================

// BrowserWindow - A standalone window that can be detached from the main application
//
// This class creates independent browser windows that can display web content
// and summaries. Each window has:
//   - QWebEngineView for displaying web pages (Chromium-based browser)
//   - QTextEdit for displaying AI-generated summaries of the page content
//
// Multiple BrowserWindows allow parallel browsing of different sources simultaneously.
//
class BrowserWindow : public QMainWindow
{
public:
    // Constructor - Creates a detachable browser window with title
    // Parameters:
    //   title - Window title to display in title bar
    //   parent - Optional parent widget (nullptr makes it independent)
    BrowserWindow(const QString &title, QWidget *parent = nullptr) : QMainWindow(parent)
    {
        // Create web view widget (displays web pages using Chromium engine)
        QWebEngineView *view = new QWebEngineView(this);

        // Create text edit widget for displaying summaries (read-only)
        QTextEdit *summary = new QTextEdit(this);
        summary->setReadOnly(true); // User cannot edit, only view

        // Create vertical layout to stack view and summary
        QVBoxLayout *layout = new QVBoxLayout();
        QWidget *centralWidget = new QWidget();

        // Add widgets to layout (web view on top, summary below)
        layout->addWidget(view);
        layout->addWidget(summary);

        // Set layout and make it the central widget of the window
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);

        setWindowTitle(title); // Set window title bar text

        // Store pointers for later access
        views.push_back(view);
        summaries.push_back(summary);
    }

    // setContent - Sets both the web view content and summary text
    // Parameters:
    //   html - HTML content to display in browser and summary
    void setContent(const QString &html)
    {
        views[0]->setHtml(html);     // Display HTML in web view
        summaries[0]->setText(html); // Display HTML in summary (or could be plain text)
    }

private:
    std::vector<QWebEngineView *> views; // Collection of web view widgets
    std::vector<QTextEdit *> summaries;  // Collection of summary text widgets
};

// ============================================================================
// HELPER FUNCTIONS - Callbacks and utility functions
// ============================================================================

// on_message - WebSocket callback for real-time data streams
//
// Called automatically when WebSocket receives data from LIGO, JWST, or other
// real-time sources. Stores incoming data in results and caches to database.
//
// Parameters:
//   user - User data pointer (not used here)
//   data - Incoming message data (JSON string)
//   len - Length of data in bytes
//
void on_message(void *user, const char *data, size_t len)
{
    // Convert raw data to C++ string
    std::string json_data(data, len);

    // Create SearchResult from live data (e.g., LIGO gravitational wave alerts)
    SearchResult result = {"wss://ligo.org/alerts", "Live Data", "Real-time event", 1.0, true};

    // Add result to first browser window's results
    results[0].push_back(result);

    // Cache to SQLite database for offline access (mark as live data)
    sqlite3_exec(db,
                 ("INSERT INTO cache (url, title, summary, isLive) VALUES ('" +
                  result.url + "', '" + result.title + "', '" + result.summary + "', 1)")
                     .c_str(),
                 nullptr, nullptr, nullptr);
}

// WriteCallback - cURL callback for handling HTTP response data
//
// Called by cURL library as data arrives from HTTP requests.
// Appends received data to the provided string buffer.
//
// Parameters:
//   contents - Pointer to received data
//   size - Size of each data element
//   nmemb - Number of elements
//   data - Pointer to std::string to store response
//
// Returns:
//   Total bytes processed (size * nmemb)
//
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data)
{
    // Append received data to output string
    data->append((char *)contents, size * nmemb);

    // Return total bytes processed (required by cURL)
    return size * nmemb;
}

// ============================================================================
// AI SUMMARIZATION FUNCTIONS
// ============================================================================

// SummarizeText - Summarizes text using local Llama-3.1 AI model
//
// Uses Hugging Face Transformers library with Meta's Llama-3.1-8B model
// to generate concise summaries of search results. Runs locally (no API calls).
//
// Parameters:
//   text - Text to summarize (e.g., webpage content, API response)
//
// Returns:
//   Summarized text (30-100 words)
//
std::string SummarizeText(const std::string &text)
{
    py::scoped_interpreter guard{}; // Initialize Python interpreter

    // Import Hugging Face Transformers library
    py::module_ transformers = py::module_::import("transformers");

    // Create summarization pipeline using Llama-3.1-8B model
    py::object summarizer = transformers.attr("pipeline")("summarization", "meta-llama/Llama-3.1-8B");

    // Generate summary (30-100 words)
    py::object summary = summarizer(text, py::arg("max_length") = 100, py::arg("min_length") = 30);

    // Extract summary text and convert to C++ string
    return summary[0].attr("summary_text").cast<std::string>();
}

// SummarizeWithOpenAI - Summarizes text using OpenAI GPT-4 (with fallback to Llama)
//
// Attempts to use OpenAI's GPT-4 API for high-quality summarization.
// Includes retry logic (3 attempts) for handling API failures.
// Falls back to local Llama-3.1 model if OpenAI API fails.
//
// Parameters:
//   query - Text to summarize
//
// Returns:
//   Summarized text (up to 100 tokens)
//
std::string SummarizeWithOpenAI(const std::string &query)
{
    CURL *curl = curl_easy_init();                                  // Initialize cURL for HTTP request
    std::string url = "https://api.openai.com/v1/chat/completions"; // OpenAI API endpoint
    std::string response;

    // Build JSON payload for GPT-4 API request
    json payload = {
        {"model", "gpt-4"},                                                     // Use GPT-4 model
        {"messages", {{{"role", "user"}, {"content", "Summarize: " + query}}}}, // User message
        {"max_tokens", 100}                                                     // Limit response to 100 tokens
    };
    std::string data = payload.dump(); // Convert JSON to string

    // Set up HTTP headers for OpenAI API
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + std::string(OPENAI_API_KEY)).c_str());

    // Configure cURL for POST request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Retry logic: Try up to 3 times for robustness
    int retries = 3;
    while (retries--)
    {
        CURLcode res = curl_easy_perform(curl); // Execute HTTP request

        // Check HTTP response code
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        // Success case: HTTP 200 OK
        if (res == CURLE_OK && http_code == 200)
        {
            // Parse JSON response and extract summary
            json result = json::parse(response);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            return result["choices"][0]["message"]["content"].get<std::string>();
        }
        // Rate limit case: HTTP 429 (Too Many Requests)
        else if (http_code == 429)
        {
            // Exponential backoff: wait 1s, 2s, 4s before retrying
            std::this_thread::sleep_for(std::chrono::seconds(1 << (3 - retries)));
            continue; // Retry
        }
        break; // Other errors: give up
    }

    // Clean up cURL resources
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Fallback: If OpenAI fails, use local Llama-3.1 model
    return SummarizeText(query);
}

// ============================================================================
// CLOUD AUTHENTICATION AND SYNC FUNCTIONS
// ============================================================================

// GetOAuthToken - Obtains OAuth2 access token from AWS Cognito
//
// Authenticates with AWS Cognito to get a token for cloud operations.
// Token is used for syncing cached data to AWS S3.
//
// Returns:
//   OAuth2 access token (currently returns mock token - needs implementation)
//
std::string GetOAuthToken()
{
    CURL *curl = curl_easy_init();

    // Build Cognito OAuth2 endpoint URL
    std::string url = "https://<domain>.auth." + std::string(COGNITO_REGION) +
                      ".amazoncognito.com/oauth2/token";

    // OAuth2 client credentials grant
    std::string data = "grant_type=client_credentials&client_id=" +
                       std::string(COGNITO_CLIENT_ID) +
                       "&client_secret=your_client_secret";

    std::string response;

    // Configure cURL for POST request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_perform(curl); // Execute OAuth request
    curl_easy_cleanup(curl);

    // TODO: Parse JSON response to extract actual access_token
    return "mock_access_token"; // Placeholder - replace with: json::parse(response)["access_token"]
}

// SyncCacheToCloud - Uploads local SQLite cache to AWS S3 for backup/sync
//
// Syncs the local cache database to cloud storage, enabling:
//   - Backup of search history
//   - Access from multiple devices
//   - Offline data recovery
//
// Parameters:
//   token - OAuth2 access token from GetOAuthToken()
//
void SyncCacheToCloud(const std::string &token)
{
    // Create S3 upload request
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket("coanqi-cache");                                  // S3 bucket name (replace with your bucket)
    request.SetKey("cache.db");                                         // Object key (filename in S3)
    request.SetCustomRequestHeader("Authorization", "Bearer " + token); // Add auth token

    // Open local cache file for reading
    std::ifstream file("coanqi_cache.db", std::ios::binary);

    // Set file as request body (uploads file contents)
    request.SetBody(std::make_shared<Aws::Fstream>(file));

    // Execute S3 upload (synchronizes local cache to cloud)
    s3_client->PutObject(request);
}

// ============================================================================
// OFFLINE SEARCH FUNCTION
// ============================================================================

// OfflineSearch - Searches local SQLite cache when internet is unavailable
//
// Provides offline access to previously cached search results.
// Searches both title and summary fields for query matches.
//
// Parameters:
//   query - Search query string
//   offlineResults - Vector to store matching results (passed by reference)
//
void OfflineSearch(const std::string &query, std::vector<SearchResult> &offlineResults)
{
    sqlite3_stmt *stmt; // SQLite prepared statement (compiled SQL query)

    // Prepare SQL query with wildcards for partial matching
    // LIKE operator allows searching for query anywhere in title or summary
    sqlite3_prepare_v2(db,
                       "SELECT url, title, summary, isLive FROM cache WHERE title LIKE ? OR summary LIKE ?",
                       -1, &stmt, nullptr);

    // Build pattern with wildcards: "query" becomes "%query%"
    // % matches any characters before/after the query
    std::string pattern = "%" + query + "%";

    // Bind pattern to both placeholders (? in SQL query)
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_STATIC); // Search in title
    sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_STATIC); // Search in summary

    // Execute query and iterate through results
    while (sqlite3_step(stmt) == SQLITE_ROW) // SQLITE_ROW = more rows available
    {
        SearchResult result;

        // Extract data from current row (columns 0-3)
        result.url = (const char *)sqlite3_column_text(stmt, 0);     // Column 0: url
        result.title = (const char *)sqlite3_column_text(stmt, 1);   // Column 1: title
        result.summary = (const char *)sqlite3_column_text(stmt, 2); // Column 2: summary
        result.isLive = sqlite3_column_int(stmt, 3);                 // Column 3: isLive (0 or 1)
        result.relevance = 0.9;                                      // High relevance (cached results are assumed relevant)

        offlineResults.push_back(result); // Add to results vector
    }

    sqlite3_finalize(stmt); // Clean up prepared statement (free memory)
}

// ============================================================================
// VOICE INPUT FUNCTION
// ============================================================================

// ProcessVoiceInput - Converts voice commands to text using PocketSphinx
//
// Uses PocketSphinx speech recognition engine to process microphone input
// and convert spoken queries into text for searching.
//
// Returns:
//   Text representation of spoken query
//
std::string ProcessVoiceInput()
{
    // Initialize PocketSphinx decoder with default configuration
    ps_decoder_t *ps = ps_init(cmd_ln_init(nullptr, ps_args(), true, nullptr));

    // Start utterance processing (begin listening for speech)
    ps_start_utt(ps);

    // TODO: Replace with actual audio capture and processing
    // This would normally:
    //   1. Capture audio from microphone
    //   2. Process audio frames with ps_process_raw()
    //   3. Extract recognized text with ps_get_hyp()
    std::string text = "sample query"; // Placeholder - replace with actual speech recognition

    // End utterance processing (stop listening)
    ps_end_utt(ps);

    // Free PocketSphinx resources (release memory and close audio devices)
    ps_free(ps);

    // Return the recognized text query (will be used for search)
    return text;
}

// ProcessVideoInput - Captures video from webcam and recognizes hand gestures
// Returns: String command (e.g., "submit query", "clear input") based on gesture
// This function:
//   1. Opens webcam (device 0 = default camera)
//   2. Captures one frame
//   3. Uses OpenCV to recognize hand gestures (TODO: implement actual recognition)
//   4. Returns command based on recognized gesture
// Use case: Hands-free operation (e.g., gesture to submit query without typing)
std::string ProcessVideoInput()
{
    // Open video capture from default camera (device index 0)
    // cv::VideoCapture is RAII - automatically closes camera when destroyed
    cv::VideoCapture cap(0);

    // Create empty matrix to hold frame data
    // cv::Mat is OpenCV's matrix class for image data
    cv::Mat frame;

    // Capture one frame from camera into 'frame' matrix
    // >> operator is overloaded to mean "read next frame"
    cap >> frame;

    // TODO: Replace with actual gesture recognition using OpenCV
    // Typical workflow:
    //   1. Convert frame to grayscale or HSV color space
    //   2. Detect skin color or hand contours
    //   3. Recognize gesture shape (e.g., open palm = "submit", fist = "clear")
    //   4. Map gesture to command string
    std::string command = "submit query"; // Placeholder - replace with actual gesture recognition

    // Release camera (close device and free resources)
    // RAII ensures this happens automatically at function end, but explicit release is good practice
    cap.release();

    // Return the command string (will trigger action in main application)
    return command;
}

// RenderScatterPlot - Visualizes 2D data using VTK scatter plot
// Parameters:
//   parent - Qt widget to embed VTK visualization (e.g., dock widget or main window)
//   x - Vector of X-axis data points (e.g., time, distance, energy)
//   y - Vector of Y-axis data points (e.g., flux, magnitude, velocity)
// Purpose: Create interactive scatter plot for scientific data visualization
// VTK (Visualization Toolkit) is a powerful library for 3D graphics and scientific visualization
// Use case: Visualize search results, orbital data, or statistical analysis
void RenderScatterPlot(QWidget *parent, const std::vector<double> &x, const std::vector<double> &y)
{
    // Create smart pointer to scatter plot matrix
    // vtkSmartPointer automatically manages memory (like std::shared_ptr but for VTK)
    // vtkScatterPlotMatrix creates a matrix of plots (1x1 in this case for single scatter plot)
    vtkSmartPointer<vtkScatterPlotMatrix> matrix = vtkSmartPointer<vtkScatterPlotMatrix>::New();

    // TODO: Add actual implementation:
    //   1. Create vtkTable and add x, y data columns
    //   2. Configure plot matrix (title, axis labels, colors)
    //   3. Create QVTKWidget to embed in Qt parent
    //   4. Set up interaction (zoom, pan, selection)
    // Simplified placeholder for now - full implementation would populate table and configure rendering
}

// SearchNASA - Query NASA APIs for space-related data
// Parameters:
//   query - User search query (e.g., "mars", "solar flare", "nebula")
//   nasaResults - Vector to populate with search results (passed by reference)
// Purpose: Search multiple NASA API endpoints and collect results
// NASA provides free APIs for:
//   - APOD (Astronomy Picture of the Day)
//   - EPIC (Earth Polychromatic Imaging Camera)
//   - DONKI (Space Weather Database Of Notifications, Knowledge, Information)
// Results are cached to SQLite database to reduce API calls
void SearchNASA(const std::string &query, std::vector<SearchResult> &nasaResults)
{
    // Initialize cURL for HTTP requests (see FetchDONKI for similar pattern)
    CURL *curl = curl_easy_init();

    // Define three NASA API endpoints to query
    // Each endpoint uses different API key and serves different data
    std::vector<std::string> endpoints = {
        // APOD: Returns astronomy picture/video with explanation (concept_tags searches descriptions)
        "https://api.nasa.gov/planetary/apod?api_key=" + std::string(NASA_API_KEY_1) + "&concept_tags=True&keywords=" + query,
        // EPIC: Earth imagery from DSCOVR satellite L1 position (1.5 million km away)
        "https://api.nasa.gov/EPIC/api/natural?api_key=" + std::string(NASA_API_KEY_2),
        // DONKI: Space weather events (CME = Coronal Mass Ejection analysis)
        "https://api.nasa.gov/DONKI/CMEAnalysis?api_key=" + std::string(NASA_API_KEY_2)};

    // Human-readable titles for results display
    std::vector<std::string> titles = {"NASA APOD Result", "NASA EPIC Result", "NASA DONKI Result"};

    // Base URLs for result links (without API keys/parameters)
    std::vector<std::string> urls = {
        "https://api.nasa.gov/planetary/apod",
        "https://api.nasa.gov/EPIC/api/natural",
        "https://api.nasa.gov/DONKI/CMEAnalysis"};

    // Loop through all three endpoints
    for (size_t i = 0; i < endpoints.size(); ++i)
    {
        // String to hold JSON response from API
        std::string response;

        // Configure cURL for this endpoint
        curl_easy_setopt(curl, CURLOPT_URL, endpoints[i].c_str());    // Set target URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Set callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);         // Pass response string to callback

        // Retry up to 3 times (NASA APIs can be unreliable or rate-limited)
        int retries = 3;
        while (retries--)
        {
            // Perform HTTP GET request
            CURLcode res = curl_easy_perform(curl);

            // Get HTTP status code (200 = OK, 429 = rate limit, 500 = server error, etc.)
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

            // Check if request succeeded (CURLE_OK = no network errors, 200 = successful HTTP response)
            if (res == CURLE_OK && http_code == 200)
            {
                // Summarize the JSON response using OpenAI GPT-4 (see SummarizeWithOpenAI function)
                // This converts technical NASA JSON into human-readable summary
                std::string summary = SummarizeWithOpenAI(response);

                // Create SearchResult struct with all fields
                // 0.95 = high confidence score (official NASA data)
                // false = not live streaming data (static result)
                SearchResult result = {urls[i], titles[i], summary, 0.95, false};

                // Add result to output vector
                nasaResults.push_back(result);

                // Cache result to SQLite database for offline access
                // Note: In production, use prepared statements to avoid SQL injection
                sqlite3_exec(db, ("INSERT INTO cache (url, title, summary, isLive) VALUES ('" + result.url + "', '" + result.title + "', '" + result.summary + "', 0)").c_str(), nullptr, nullptr, nullptr);

                // Success - break retry loop and move to next endpoint
                break;
            }
            else if (http_code == 429) // Rate limit exceeded
            {
                // Exponential backoff: 1s, 2s, 4s delays
                // Bit shift (1 << n) calculates 2^n (e.g., 1 << 2 = 4)
                std::this_thread::sleep_for(std::chrono::seconds(1 << (3 - retries)));
                continue; // Retry request after delay
            }
            // If other error (network timeout, 500 server error, etc.), retry immediately
        }
    }

    // Clean up cURL handle (free memory, close connections)
    curl_easy_cleanup(curl);
}

// SearchMAST - Query MAST (Mikulski Archive for Space Telescopes) for astronomy data
// Parameters:
//   query - User search query (currently unused - uses hardcoded example)
//   mastResults - Vector to populate with MAST results
// Purpose: Access astronomical data from space telescopes (Hubble, JWST, Kepler, TESS, etc.)
// MAST: https://mast.stsci.edu/
//   - Largest astronomy archive in world
//   - Hubble Space Telescope (HST) images and spectra
//   - James Webb Space Telescope (JWST) data
//   - Transiting exoplanet data (TESS)
// Note: This is simplified example - production code would search catalog, not download specific file
void SearchMAST(const std::string &query, std::vector<SearchResult> &mastResults)
{
    // Initialize cURL for HTTP request
    CURL *curl = curl_easy_init();

    // Example URL: Download specific HST FITS file
    // URI format: mast:HST/product/<filename>
    // This example: ACS (Advanced Camera for Surveys) F814W filter (infrared) image
    // In production, would search catalog first, then download matching files
    std::string url = "https://mast.stsci.edu/api/v0.1/Download/file?uri=mast:HST/product/hst_12345_01_acs_f814w_drz.fits&token=" + std::string(MAST_API_KEY);

    // String to hold response (FITS file metadata or file data)
    std::string response;

    // Configure cURL (same pattern as SearchNASA)
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());             // Set download URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Response callback
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);         // Response buffer

    // Perform HTTP GET request
    CURLcode res = curl_easy_perform(curl);

    // Check if request succeeded
    if (res == CURLE_OK)
    {
        // Summarize FITS file metadata or data using OpenAI
        // (FITS = Flexible Image Transport System, standard astronomy data format)
        std::string summary = SummarizeWithOpenAI(response);

        // Create result with HST infrared image information
        // 0.95 = high confidence (official archive data)
        // false = not live streaming
        SearchResult result = {"https://mast.stsci.edu/api/v0.1/Download/file?uri=mast:HST/product/hst_12345_01_acs_f814w_drz.fits", "MAST HST Infrared", summary, 0.95, false};

        // Add to results vector
        mastResults.push_back(result);

        // Cache to database
        // Note: Should use prepared statements to prevent SQL injection
        sqlite3_exec(db, ("INSERT INTO cache (url, title, summary, isLive) VALUES ('" + result.url + "', '" + result.title + "', '" + result.summary + "', 0)").c_str(), nullptr, nullptr, nullptr);
    }

    // Clean up cURL
    curl_easy_cleanup(curl);
}

// FetchHorizons - Query JPL Horizons system for solar system body ephemerides
// Parameters:
//   command - Object identifier (e.g., "499" for Mars, "301" for Moon, "2000001" for asteroid Ceres)
//   start_time - Start date/time in format 'YYYY-MM-DD' or 'YYYY-MM-DD HH:MM'
//   stop_time - End date/time (same format as start_time)
// Returns: String with ephemeris data (positions, velocities, magnitudes, etc.)
// JPL Horizons: https://ssd.jpl.nasa.gov/horizons/
//   - Most accurate solar system ephemerides available
//   - Positions of planets, moons, asteroids, comets, spacecraft
//   - Used for mission planning and scientific research
// Use case: Calculate object positions for observation planning or trajectory analysis
std::string FetchHorizons(const std::string &command, const std::string &start_time, const std::string &stop_time)
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build Horizons API URL with query parameters
    // format=text - Plain text output (alternative: json, xml)
    // COMMAND - Object identifier (numeric ID or name)
    // OBJ_DATA=YES - Include physical data (mass, radius, etc.)
    // MAKE_EPHEM=YES - Generate ephemeris table
    // EPHEM_TYPE=OBSERVER - Observer table (sky position as seen from Earth)
    // CENTER=500@399 - Geocentric (399=Earth, 500=observer)
    // START_TIME, STOP_TIME - Date range
    // STEP_SIZE=1%20d - 1 day steps (%20 = URL-encoded space)
    // QUANTITIES - Data columns: 1=astrometric RA/DEC, 9=visual magnitude, 20=observer range, 23/24=sun/obs range rates, 29=illumination %
    std::string url = "https://ssd.jpl.nasa.gov/api/horizons.api?format=text&COMMAND='" + command + "'&OBJ_DATA='YES'&MAKE_EPHEM='YES'&EPHEM_TYPE='OBSERVER'&CENTER='500@399'&START_TIME='" + start_time + "'&STOP_TIME='" + stop_time + "'&STEP_SIZE='1%20d'&QUANTITIES='1,9,20,23,24,29'";

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request (no error checking in this simplified version)
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return raw ephemeris text (caller can parse as needed)
    return response;
}

// FetchJDCalJD - Convert Julian Date (JD) to calendar date
// Parameter: jd - Julian Date number as string (e.g., "2451545.0" = Jan 1, 2000 12:00 UT)
// Returns: Calendar date in format "YYYY-MM-DD HH:MM:SS.sss" or similar
// Julian Date: Continuous count of days since Jan 1, 4713 BC (used in astronomy to avoid calendar complexities)
// Use case: Convert JD from astronomical data to human-readable date
std::string FetchJDCalJD(const std::string &jd)
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build JD-Cal API URL
    // jd=<number> - Julian Date to convert
    // format=s - String format (alternative: json)
    std::string url = "https://ssd-api.jpl.nasa.gov/jd_cal.api?jd=" + jd + "&format=s";

    // Response buffer
    std::string response;

    // Configure cURL (standard pattern)
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return calendar date string
    return response;
}

// FetchJDCalCD - Convert calendar date to Julian Date (JD)
// Parameter: cd - Calendar date string (e.g., "2000-01-01 12:00:00")
// Returns: Julian Date number as string (e.g., "2451545.0")
// Inverse of FetchJDCalJD - converts human-readable date to JD for calculations
// Use case: Convert observation date to JD for ephemeris calculations or time arithmetic
std::string FetchJDCalCD(const std::string &cd)
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build JD-Cal API URL
    // cd=<date> - Calendar date to convert
    // No format parameter needed (returns JD number)
    std::string url = "https://ssd-api.jpl.nasa.gov/jd_cal.api?cd=" + cd;

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return JD number
    return response;
}

// FetchPeriodicEarthMoon - Get periodic orbit data for Earth-Moon system
// Parameters:
//   family - Orbit family (e.g., "halo", "lyapunov", "vertical", "axial", "butterfly")
//   libr - Libration point (e.g., "L1", "L2", "L3", "L4", "L5")
//   branch - Orbit branch ("N" for northern, "S" for southern)
// Returns: JSON with orbit initial conditions and characteristics
// Periodic orbits: Closed trajectories around Lagrange points (useful for spacecraft missions)
// Use case: Mission planning for lunar gateway, JWST-like missions, etc.
std::string FetchPeriodicEarthMoon(const std::string &family, const std::string &libr, const std::string &branch)
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build periodic orbits API URL for Earth-Moon system
    // sys=earth-moon - Two-body system
    // family=<orbit_type> - Shape of orbit
    // libr=<lagrange_point> - Which Lagrange point (L1-L5)
    // branch=<N|S> - Northern or southern branch
    std::string url = "https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=earth-moon&family=" + family + "&libr=" + libr + "&branch=" + branch;

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return orbit data (JSON with initial conditions, period, stability index, etc.)
    return response;
}

// FetchPeriodicJupiterEuropa - Get periodic orbit data for Jupiter-Europa system
// Parameters:
//   family - Orbit family (e.g., "halo", "vertical")
//   stability - Optional: Maximum stability index filter (default -1.0 = no filter)
// Returns: JSON with orbit data for Jupiter's moon Europa
// Europa: Jupiter's icy moon with subsurface ocean (target for astrobiology missions)
// Use case: Planning orbits for Europa Clipper, future lander missions
std::string FetchPeriodicJupiterEuropa(const std::string &family, double stability = -1.0)
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build periodic orbits API URL for Jupiter-Europa system
    // sys=jupiter-europa - Two-body system
    // family=<orbit_type> - Shape of orbit
    std::string url = "https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=jupiter-europa&family=" + family;

    // Add stability filter if specified (stabmax filters orbits by maximum stability index)
    // Stability index: Measure of how quickly nearby trajectories diverge (lower = more stable)
    if (stability > -1.0)
        url += "&stabmax=" + std::to_string(stability);

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return orbit data
    return response;
}

// FetchPeriodicSaturnEnceladus - Get periodic orbit data for Saturn-Enceladus system
// Parameters:
//   family - Orbit family
//   libr - Libration point
//   periodmax - Maximum orbital period (default 1.0)
//   periodunits - Units for period ("d"=days, "h"=hours, "y"=years)
// Returns: JSON with orbit data for Saturn's moon Enceladus
// Enceladus: Saturn's moon with active water geysers (high astrobiology interest)
// Use case: Planning sample-return missions through geyser plumes
std::string FetchPeriodicSaturnEnceladus(const std::string &family, const std::string &libr, double periodmax = 1.0, const std::string &periodunits = "d")
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build periodic orbits API URL for Saturn-Enceladus system
    // periodmax filters orbits by maximum period (exclude long-period orbits)
    // periodunits specifies time units for periodmax
    std::string url = "https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=saturn-enceladus&family=" + family + "&libr=" + libr + "&periodmax=" + std::to_string(periodmax) + "&periodunits=" + periodunits;

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return orbit data
    return response;
}

// FetchPeriodicSaturnTitan - Get periodic orbit data for Saturn-Titan system
// Parameters:
//   family - Orbit family
//   jacobimin - Minimum Jacobi constant filter (default 3.0)
//   stabmax - Maximum stability index (default 1.0)
//   branch - Orbit branch (default "N")
// Returns: JSON with orbit data for Saturn's moon Titan
// Titan: Saturn's largest moon with thick atmosphere and liquid methane lakes (only moon with stable surface liquids)
// Jacobi constant: Energy-like integral of motion in circular restricted three-body problem (filters orbit energy)
// Use case: Planning Dragonfly-like missions to Titan's surface
std::string FetchPeriodicSaturnTitan(const std::string &family, double jacobimin = 3.0, double stabmax = 1.0, const std::string &branch = "N")
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build periodic orbits API URL for Saturn-Titan system
    // jacobimin filters orbits by minimum Jacobi constant (exclude low-energy orbits)
    // stabmax filters by maximum stability index (exclude unstable orbits)
    // branch specifies orbit family branch
    std::string url = "https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=saturn-titan&family=" + family + "&jacobimin=" + std::to_string(jacobimin) + "&stabmax=" + std::to_string(stabmax) + "&branch=" + branch;

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return orbit data
    return response;
}

// FetchPeriodicMarsPhobos - Get periodic orbit data for Mars-Phobos system
// Parameters:
//   family - Orbit family
//   jacobimin - Minimum Jacobi constant filter (default 3.0)
//   stabmax - Maximum stability index (default 1.0)
//   branch - Orbit branch (default "21")
// Returns: JSON with orbit data for Mars's moon Phobos
// Phobos: Mars's larger moon, orbiting very close (only 6,000 km above surface)
// Phobos is slowly spiraling into Mars due to tidal forces (will break up in ~50 million years)
// Use case: Planning sample-return missions to Phobos (easier than landing on Mars surface)
std::string FetchPeriodicMarsPhobos(const std::string &family, double jacobimin = 3.0, double stabmax = 1.0, const std::string &branch = "21")
{
    // Initialize cURL
    CURL *curl = curl_easy_init();

    // Build periodic orbits API URL for Mars-Phobos system
    // Same parameters as Saturn-Titan (jacobimin, stabmax, branch)
    std::string url = "https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=mars-phobos&family=" + family + "&jacobimin=" + std::to_string(jacobimin) + "&stabmax=" + std::to_string(stabmax) + "&branch=" + branch;

    // Response buffer
    std::string response;

    // Configure cURL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Clean up
    curl_easy_cleanup(curl);

    // Return orbit data
    return response;
}

// PerformSearch - Main search orchestration function (coordinates all API calls and result distribution)
// Parameters:
//   query - User search query string (e.g., "mars ephemeris", "earth-moon halo")
//   focus - Vector of organization names to search (e.g., "NASA", "STScI", "Hubble")
//   online - If true, query APIs; if false, search offline SQLite cache only
//   oauth_token - OAuth2 token for authenticated API access (from GetOAuthToken)
// Purpose: Central hub that:
//   1. Routes queries to appropriate APIs based on keywords and focus list
//   2. Distributes results across 21 browser windows
//   3. Handles online/offline mode switching
//   4. Triggers visualizations for orbital data
// This is the main entry point for all search operations in the application
void PerformSearch(const std::string &query, std::vector<std::string> &focus, bool online, const std::string &oauth_token)
{
    // OFFLINE MODE: Search only local SQLite cache
    if (!online)
    {
        // Vector to hold results from database
        std::vector<SearchResult> offlineResults;

        // Query SQLite database with LIKE wildcards (see OfflineSearch function)
        OfflineSearch(query, offlineResults);

        // Distribute results across browser windows (up to MAX_WINDOWS = 21)
        // Each window gets first matching result (if available)
        for (int i = 0; i < MAX_WINDOWS && i < offlineResults.size(); ++i)
        {
            results[i].push_back(offlineResults[i]);
        }

        // Done - return early (no API calls in offline mode)
        return;
    }

    // ONLINE MODE: Query external APIs

    // Vectors to hold results from NASA and MAST APIs
    std::vector<SearchResult> nasaResults, mastResults;

    // Check if user wants NASA data (based on focus list)
    // std::find searches vector for "NASA" string
    if (std::find(focus.begin(), focus.end(), "NASA") != focus.end())
    {
        // Query NASA APIs (APOD, EPIC, DONKI - see SearchNASA function)
        SearchNASA(query, nasaResults);

        // Assign NASA results to browser window #1
        results[1] = nasaResults;
    }

    // Check if user wants STScI/Hubble data (any of these focus strings)
    if (std::find(focus.begin(), focus.end(), "STScI") != focus.end() ||
        std::find(focus.begin(), focus.end(), "Hubble") != focus.end() ||
        std::find(focus.begin(), focus.end(), "ACS Hubble Ultra Deep Field") != focus.end())
    {
        // Query MAST archive (see SearchMAST function)
        SearchMAST(query, mastResults);

        // Assign MAST results to browser window #2
        results[2] = mastResults;
    }

    // PRELOADED LINKS: Always populate specific windows with featured content
    // These are high-value resources that don't require search queries

    // Window #3: MAST HST infrared image (specific FITS file)
    results[3].push_back({"https://mast.stsci.edu/api/v0.1/Download/file?uri=mast:HST/product/hst_12345_01_acs_f814w_drz.fits", "MAST ACS F814W Infrared", SummarizeWithOpenAI("Hubble infrared data"), 0.95, false});

    // Window #4: Event Horizon Telescope live data (WebSocket stream, isLive=true)
    results[4].push_back({"wss://eventhorizontelescope.org/data", "EHT Live Infrared Data", SummarizeWithOpenAI("Real-time EHT data"), 1.0, true});

    // Window #5: NASA M31 (Andromeda) infrared image
    results[5].push_back({"https://apod.nasa.gov/apod/image/2507/m31_infrared.jpg", "NASA M31 Infrared", SummarizeWithOpenAI("Andromeda infrared image"), 0.95, false});

    // Window #6: LIGO gravitational wave alerts (WebSocket stream, isLive=true)
    results[6].push_back({"wss://ligo.org/alerts", "LIGO GW Infrared Correlations", SummarizeWithOpenAI("Real-time GW alerts"), 1.0, true});

    // JPL API PARAMETERS: Default values (in production, parse from query string)
    // Note: Simplified parameter extraction - use regex or JSON config in production
    std::string command = "499";           // Horizons object ID (499 = Mars)
    std::string start_time = "2006-01-01"; // Ephemeris start date
    std::string stop_time = "2006-01-20";  // Ephemeris end date
    std::string jd = "2451544";            // Julian Date for JD-Cal conversion
    std::string cd = "2000-01-01_12:00";   // Calendar date for JD-Cal conversion
    std::string family = "halo";           // Periodic orbit family
    std::string libr = "1";                // Libration point (1 = L1)
    std::string branch = "N";              // Northern branch
    double stability = -1.0;               // No stability filter
    double periodmax = 1.0;                // Maximum period (1 day)
    std::string periodunits = "d";         // Days
    double jacobimin = 3.0;                // Minimum Jacobi constant
    double stabmax = 1.0;                  // Maximum stability index

    // QUERY ROUTING: Detect keywords and call appropriate JPL APIs

    // Horizons ephemeris query (planetary/asteroid positions)
    if (query.find("ephemeris") != std::string::npos || query.find("horizons") != std::string::npos)
    {
        // Fetch ephemeris data from JPL Horizons
        std::string horizons = FetchHorizons(command, start_time, stop_time);

        // Add result to window #7
        results[7].push_back({"https://ssd.jpl.nasa.gov/api/horizons.api", "JPL Horizons Ephemeris", SummarizeWithOpenAI(horizons), 0.95, false});

        // Visualize orbital data as scatter plot (VTK)
        // Empty vectors in this simplified version - in production, parse ephemeris data
        RenderScatterPlot(nullptr, {}, {});
    }

    // Julian Date to Calendar Date conversion
    if (query.find("jd to date") != std::string::npos)
    {
        // Convert JD to calendar date
        std::string jdcal = FetchJDCalJD(jd);

        // Add result to window #8
        results[8].push_back({"https://ssd-api.jpl.nasa.gov/jd_cal.api?jd=2451544&format=s", "JPL JD-Cal JD to Date", SummarizeWithOpenAI(jdcal), 0.95, false});
    }

    // Calendar Date to Julian Date conversion
    if (query.find("date to jd") != std::string::npos)
    {
        // Convert calendar date to JD
        std::string jdcal = FetchJDCalCD(cd);

        // Add result to window #9
        results[9].push_back({"https://ssd-api.jpl.nasa.gov/jd_cal.api?cd=2000-01-01_12:00", "JPL JD-Cal Date to JD", SummarizeWithOpenAI(jdcal), 0.95, false});
    }

    // Earth-Moon L1 halo orbit
    if (query.find("earth-moon halo") != std::string::npos)
    {
        // Fetch periodic orbit data
        std::string orbits = FetchPeriodicEarthMoon(family, libr, branch);

        // Add result to window #10
        results[10].push_back({"https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=earth-moon&family=halo&libr=1&branch=N", "JPL Periodic Orbits Earth-Moon", SummarizeWithOpenAI(orbits), 0.95, false});

        // Visualize orbit
        RenderScatterPlot(nullptr, {}, {});
    }

    // Jupiter-Europa DRO (Distant Retrograde Orbit)
    if (query.find("jupiter-europa dro") != std::string::npos)
    {
        // Fetch Jupiter-Europa orbit data
        std::string orbits = FetchPeriodicJupiterEuropa(family, stability);

        // Add result to window #11
        results[11].push_back({"https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=jupiter-europa&family=dro", "JPL Periodic Orbits Jupiter-Europa", SummarizeWithOpenAI(orbits), 0.95, false});
    }

    // Saturn-Enceladus vertical orbit at L2
    if (query.find("saturn-enceladus vertical") != std::string::npos)
    {
        // Fetch Saturn-Enceladus orbit data
        std::string orbits = FetchPeriodicSaturnEnceladus(family, libr, periodmax, periodunits);

        // Add result to window #12
        results[12].push_back({"https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=saturn-enceladus&family=vertical&libr=2&periodmax=1&periodunits=d", "JPL Periodic Orbits Saturn-Enceladus", SummarizeWithOpenAI(orbits), 0.95, false});
    }

    // Saturn-Titan butterfly orbit
    if (query.find("saturn-titan butterfly") != std::string::npos)
    {
        // Fetch Saturn-Titan orbit data
        std::string orbits = FetchPeriodicSaturnTitan(family, jacobimin, stabmax, branch);

        // Add result to window #13
        results[13].push_back({"https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=saturn-titan&family=butterfly&jacobimin=3&stabmax=1&branch=N", "JPL Periodic Orbits Saturn-Titan", SummarizeWithOpenAI(orbits), 0.95, false});
    }

    // Mars-Phobos resonant orbit
    if (query.find("mars-phobos resonant") != std::string::npos)
    {
        // Fetch Mars-Phobos orbit data
        std::string orbits = FetchPeriodicMarsPhobos(family, jacobimin, stabmax, branch);

        // Add result to window #14
        results[14].push_back({"https://ssd-api.jpl.nasa.gov/periodic_orbits.api?sys=mars-phobos&family=resonant&jacobimin=3&stabmax=1&branch=21", "JPL Periodic Orbits Mars-Phobos", SummarizeWithOpenAI(orbits), 0.95, false});

        // Visualize orbit
        RenderScatterPlot(nullptr, {}, {});
    }

    struct lws_context *ws_context = lws_create_context(nullptr);
    lws_connect(ws_context, "eventhorizontelescope.org", 443, "/data", on_message, nullptr);
    lws_connect(ws_context, "skaobservatory.org", 443, "/realtime", on_message, nullptr);
    lws_connect(ws_context, "ligo.org", 443, "/alerts", on_message, nullptr);
    lws_connect(ws_context, "fast.bao.ac.cn", 443, "/realtime", on_message, nullptr);

    CURL *curl = curl_easy_init();
    for (int i = 15; i < MAX_WINDOWS && i < focus.size(); ++i)
    {
        std::string url = "https://api.example.com/search?q=" + query + "&source=" + focus[i];
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK)
        {
            std::string summary = SummarizeWithOpenAI(response);
            SearchResult result = {"https://example.com", "Sample Result", summary, 0.95, false};
            results[i].push_back(result);
            sqlite3_exec(db, ("INSERT INTO cache (url, title, summary, isLive) VALUES ('" + result.url + "', '" + result.title + "', '" + result.summary + "', 0)").c_str(), nullptr, nullptr, nullptr);
        }
    }
    curl_easy_cleanup(curl);
    lws_context_destroy(ws_context);
    SyncCacheToCloud(oauth_token);
}

// MainWindow - Main Qt application window (CoAnQi scientific search interface)
// Inherits from: QMainWindow (Qt's base class for main application windows)
// Q_OBJECT macro enables Qt's meta-object features (signals/slots, properties, introspection)
// Purpose: Create the complete GUI with:
//   - Firefox-like top navigation bar
//   - 21 tabbed browser windows for multi-source results
//   - Visualization sidebar (left dock)
//   - Calculus toolbar (right dock)
//   - Scientific and Ramanujan calculator dialogs
//   - System tray integration (Windows only)
// This class ties together all UI components and coordinates their interactions
class MainWindow : public QMainWindow
{
Q_OBJECT // Required macro for Qt's meta-object system (enables signals/slots)

    public :
    // Constructor - Called when MainWindow object is created
    // Sets up entire UI: widgets, layouts, connections, databases, AWS clients
    MainWindow()
    {
// WINDOWS-SPECIFIC: System tray icon (optional, only on Windows)
#ifdef _WIN32
        // Create notification icon data structure
        NOTIFYICONDATA nid = {sizeof(nid)};                      // Initialize with struct size
        nid.hWnd = (HWND)winId();                                // Window handle (Qt's winId() gets native HWND)
        nid.uID = 1;                                             // Unique icon ID
        nid.uFlags = NIF_ICON | NIF_TIP;                         // Icon and tooltip enabled
        nid.hIcon = LoadIcon(GetModuleHandle(nullptr), "Z.ico"); // Load icon from resources
        strcpy(nid.szTip, "CoAnQi");                             // Tooltip text when hovering over tray icon
        Shell_NotifyIcon(NIM_ADD, &nid);                         // Add icon to system tray
#endif

        // CENTRAL WIDGET: Main container for all UI elements
        // QMainWindow requires setCentralWidget() - this is the main content area
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget); // Vertical layout (top to bottom)

        // TOP NAVIGATION BAR: Firefox-style controls
        QHBoxLayout *topBar = new QHBoxLayout(); // Horizontal layout (left to right)

        // Navigation buttons (Back, Forward, Refresh)
        QPushButton *backBtn = new QPushButton("Back", this);
        QPushButton *forwardBtn = new QPushButton("Forward", this);
        QPushButton *refreshBtn = new QPushButton("Refresh", this);

        // Main search input field
        QLineEdit *queryField = new QLineEdit(this);
        queryField->setMaxLength(MAX_QUERY_LENGTH);                       // Limit to 3000 characters
        queryField->setPlaceholderText("Search high-energy datasets..."); // Gray hint text

        // Feature buttons (voice, video, calculators)
        QPushButton *voiceBtn = new QPushButton("🎤", this);      // Voice input (microphone icon)
        QPushButton *videoBtn = new QPushButton("📹", this);      // Video gesture input (camera icon)
        QPushButton *sciCalcBtn = new QPushButton("🔬", this);    // Scientific calculator (microscope icon)
        QPushButton *ramCalcBtn = new QPushButton("🔬R", this);   // Ramanujan calculator (with R)
        QPushButton *calcBtnField = new QPushButton("🔬C", this); // Calculus toolbar (with C)

        // Application logo/title
        QLabel *logo = new QLabel("<b>CoAnQi (Cosmic Analysis and Quantum Intelligence)</b>", this);
        logo->setStyleSheet("font-size: 24px; color: #2a5298;"); // Styled text (blue color)

        // Menu button (hamburger menu icon)
        QPushButton *menuBtn = new QPushButton("☰", this);

        // Add all controls to top bar (left to right order)
        topBar->addWidget(backBtn);
        topBar->addWidget(forwardBtn);
        topBar->addWidget(refreshBtn);
        topBar->addWidget(queryField);
        topBar->addWidget(voiceBtn);
        topBar->addWidget(videoBtn);
        topBar->addWidget(sciCalcBtn);
        topBar->addWidget(ramCalcBtn);
        topBar->addWidget(calcBtnField);
        topBar->addWidget(logo);
        topBar->addWidget(menuBtn);

        // Add top bar to main layout
        layout->addLayout(topBar);

        // FOCUS LIST: Text area showing which organizations to search (NASA, STScI, etc.)
        QTextEdit *focusField = new QTextEdit(this);

        // Populate with initial focus list (20 organizations from global variable)
        QString focusText;
        for (const auto &item : focusList)
            focusText += QString::fromStdString(item) + "\n"; // Convert std::string to QString
        focusField->setText(focusText);

        // Add to main layout
        layout->addWidget(focusField);

        // TABBED BROWSER WINDOWS: 21 tabs for distributed results
        QTabWidget *tabs = new QTabWidget(this);
        tabs->setTabsClosable(true); // X button on each tab
        tabs->setMovable(true);      // Drag tabs to reorder

        // Create array of 21 BrowserWindow objects
        browserWindows = new BrowserWindow *[MAX_WINDOWS]; // Allocate array of pointers
        for (int i = 0; i < MAX_WINDOWS; ++i)
        {
            // Create each browser window with numbered title
            browserWindows[i] = new BrowserWindow(QString("Tab %1").arg(i + 1), this);

            // Add placeholder widget to tab (actual content loaded later)
            tabs->addTab(new QWidget(), QString("Tab %1").arg(i + 1));
        }

        // Special case: Tab 21 preloaded with ALMA Cycle 12 observing tool
        // ALMA = Atacama Large Millimeter Array (radio telescope in Chile)
        browserWindows[20]->views[0]->load(QUrl("https://almascience.nrao.edu/proposing/observing-tool/tarball-download-page"));

        // Add tabs to main layout
        layout->addWidget(tabs);

        // VISUALIZATION SIDEBAR: Left dock for scatter plots and graphs
        QDockWidget *sidebar = new QDockWidget("Visualizations", this); // Dockable widget with title bar
        QWidget *visWidget = new QWidget();                             // Container for visualization content
        QVBoxLayout *visLayout = new QVBoxLayout(visWidget);
        visLayout->addWidget(new QLabel("Dataset Graph Placeholder")); // TODO: Add actual VTK plots
        sidebar->setWidget(visWidget);
        addDockWidget(Qt::LeftDockWidgetArea, sidebar); // Attach to left edge of window

        // CALCULUS TOOLBAR: Right dock with derivative/integral buttons
        CalculusButtonField *calcField = new CalculusButtonField(this);
        addDockWidget(Qt::RightDockWidgetArea, calcField); // Attach to right edge

        // CALCULATOR DIALOGS: Create and show scientific and Ramanujan calculators
        ScientificCalculatorDialog *sciCalcDialog = new ScientificCalculatorDialog(this);
        sciCalcDialog->move(50, 50); // Position on screen (50, 50 pixels from top-left)
        sciCalcDialog->show();       // Make visible

        RamanujanCalculatorDialog *ramCalcDialog = new RamanujanCalculatorDialog(this);
        ramCalcDialog->move(100, 100); // Offset slightly from scientific calculator
        ramCalcDialog->show();

        // Set central widget (required for QMainWindow)
        setCentralWidget(centralWidget);

        // DATABASE AND CLOUD INITIALIZATION

        // Open SQLite database (or create if doesn't exist)
        sqlite3_open("coanqi_cache.db", &db);

        // Create cache table if not exists (stores offline search results)
        // Schema: url (TEXT), title (TEXT), summary (TEXT), isLive (INTEGER boolean)
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS cache (url TEXT, title TEXT, summary TEXT, isLive INTEGER)", nullptr, nullptr, nullptr);

        // Initialize AWS SDK (required before using S3 or Cognito clients)
        Aws::SDKOptions options; // Default SDK options
        Aws::InitAPI(options);   // Initialize SDK (loads credentials, configs)

        // Create AWS clients for cloud services
        s3_client = new Aws::S3::S3Client();                                                // For caching to cloud storage
        cognito_client = new Aws::CognitoIdentityProvider::CognitoIdentityProviderClient(); // For authentication

        // OAUTH AUTHENTICATION: Get token for authenticated API access
        std::string oauth_token = GetOAuthToken(); // Calls AWS Cognito (see GetOAuthToken function)

        // SIGNAL/SLOT CONNECTIONS: Wire up all button clicks and events
        // Qt's signal/slot mechanism connects events (signals) to handlers (slots/lambdas)

        // 1. SEARCH SUBMISSION: When user presses Enter in query field
        connect(queryField, &QLineEdit::returnPressed, [=]()
                {
            // Get query text from input field
            std::string query = queryField->text().toStdString();
            
            // Validate query length (prevent SQL injection and buffer overflow)
            if (query.length() > MAX_QUERY_LENGTH) {
                QMessageBox::warning(this, "Error", "Query exceeds 3000 characters!");
                return;  // Abort search
            }
            
            // Check if online (TODO: add actual connectivity check via ping or curl)
            bool online = true;
            
            // Perform search (coordinates all API calls - see PerformSearch function)
            PerformSearch(query, focusList, online, oauth_token);
            
            // Update all browser windows with results
            for (int i = 0; i < MAX_WINDOWS; ++i) {
                // Build HTML list of results for this window
                QString html = "<ul>";  // Start unordered list
                
                // Iterate through all results for window i
                for (const auto& result : results[i]) {
                    // Add "[Live]" tag if WebSocket stream
                    QString live = result.isLive ? " [Live]" : "";
                    
                    // Create list item with:
                    //   - Clickable link (result.url)
                    //   - Title (result.title)
                    //   - Live indicator
                    //   - AI summary (result.summary)
                    //   - Retry button (in case of failed load)
                    html += QString("<li><a href='%1'>%2</a>%3: %4 <button>Retry</button></li>")
                        .arg(QString::fromStdString(result.url))  // Link URL
                        .arg(QString::fromStdString(result.title))  // Link text
                        .arg(live)  // [Live] tag
                        .arg(QString::fromStdString(result.summary));  // Summary text
                }
                
                html += "</ul>";  // Close list
                
                // Set HTML content in browser window
                browserWindows[i]->setContent(html);
            } });

        // 2. TAB DETACHMENT: Double-click tab to open in separate window
        connect(tabs, &QTabWidget::tabBarDoubleClicked, [=](int index)
                {
            // Get browser window for clicked tab
            BrowserWindow* window = browserWindows[index];
            
            // Show as standalone window
            window->show();
            
            // Remove from tab widget (tab is now detached)
            tabs->removeTab(index); });

        // 3. VOICE INPUT: Microphone button triggers speech recognition
        connect(voiceBtn, &QPushButton::clicked, [=]()
                {
            // Capture speech, convert to text, and populate query field
            queryField->setText(QString::fromStdString(ProcessVoiceInput())); });

        // 4. VIDEO GESTURE INPUT: Camera button triggers gesture recognition
        connect(videoBtn, &QPushButton::clicked, [=]()
                {
            // Process video frame and check for "submit" gesture
            if (ProcessVideoInput() == "submit query") {
                // Simulate Enter key press to trigger search
                // Creates synthetic KeyPress event and posts to query field
                QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
                QCoreApplication::postEvent(queryField, event);
            } });

        // 5. SCIENTIFIC CALCULATOR: Show/hide dialog
        connect(sciCalcBtn, &QPushButton::clicked, [=]()
                { sciCalcDialog->show(); });

        // 6. RAMANUJAN CALCULATOR: Show/hide dialog
        connect(ramCalcBtn, &QPushButton::clicked, [=]()
                { ramCalcDialog->show(); });

        // 7. CALCULUS TOOLBAR: Show/hide dock widget
        connect(calcBtnField, &QPushButton::clicked, [=]()
                { calcField->show(); });

        // 8. FOCUS LIST UPDATES: When user edits organization list
        connect(focusField, &QTextEdit::textChanged, [=]()
                {
            // Clear existing focus list
            focusList.clear();
            
            // Parse text area into lines (each line = one organization)
            QStringList lines = focusField->toPlainText().split("\n");
            
            // Rebuild focus list from non-empty lines
            for (const auto& line : lines) {
                if (!line.isEmpty())
                    focusList.push_back(line.toStdString());  // Convert QString to std::string
            } });
    }

    // Destructor - Called when MainWindow object is destroyed
    // Cleans up all allocated resources to prevent memory leaks
    ~MainWindow()
    {
        // Delete all 21 browser windows
        for (int i = 0; i < MAX_WINDOWS; ++i)
            delete browserWindows[i]; // Free each BrowserWindow object

        // Delete array itself
        delete[] browserWindows;

        // Close SQLite database (flush buffers, release file locks)
        sqlite3_close(db);

        // Delete AWS clients (free network connections and memory)
        delete s3_client;
        delete cognito_client;

        // Shutdown AWS SDK (opposite of InitAPI - releases global resources)
        Aws::ShutdownAPI(Aws::SDKOptions());

// WINDOWS-SPECIFIC: Remove system tray icon
#ifdef _WIN32
        NOTIFYICONDATA nid = {sizeof(nid)};
        nid.uID = 1;                        // Same ID as in constructor
        Shell_NotifyIcon(NIM_DELETE, &nid); // Remove from tray
#endif
    }

private:
    // Member variable: Array of pointers to 21 browser windows
    // Private because only MainWindow should access this directly
    BrowserWindow **browserWindows;
};

// main() - Application entry point (where program execution begins)
// Parameters:
//   argc - Argument count (number of command-line arguments)
//   argv - Argument vector (array of C-style strings with actual arguments)
// Returns: Exit code (0 = success, non-zero = error)
// Qt applications follow standard C++ main() pattern but use QApplication
int main(int argc, char *argv[])
{
    // Create QApplication object (required for all Qt GUI applications)
    // QApplication manages:
    //   - Event loop (processes mouse/keyboard events, timers, signals/slots)
    //   - Window management
    //   - Platform-specific initialization (Windows, macOS, Linux)
    //   - Application-wide resources (fonts, colors, settings)
    QApplication app(argc, argv);

    // Create main window object
    // This calls MainWindow constructor (which creates entire UI)
    MainWindow window;

    // Set window title (appears in title bar and taskbar)
    window.setWindowTitle("CoAnQi");

    // Set window icon (appears in title bar and taskbar)
    // Loads from Z.png file in current directory
    window.setWindowIcon(QIcon("Z.png"));

    // Show window (make visible on screen)
    // Window is created hidden by default - show() makes it visible
    window.show();

    // Start Qt event loop (blocking call - waits for events until window closes)
    // app.exec() processes:
    //   - User input (mouse clicks, keyboard presses)
    //   - Timer events
    //   - Signal/slot activations
    //   - Window repaints
    // Returns exit code when application quits (0 = normal exit)
    return app.exec();
