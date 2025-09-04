#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <limits>
#include <map>
#include <set>
#include <cctype>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

// Navigation system
enum PageType
{
    MAIN_MENU,
    STAFF_MENU,
    ORDER_ENTRY,
    ITEM_CATEGORY_SELECTION,
    ITEM_SELECTION,
    CART_VIEW,
    PAYMENT_GATEWAY,
    DISCOUNT_SELECTION,
    ADVANCED_DISCOUNT,
    REPORT_VIEW,
    TAX_CONFIG
};

// Order flow step enumeration
enum OrderStep
{
    ITEM_SELECTION_STEP,
    CART_REVIEW_STEP,
    PAYMENT_STEP
};

struct NavigationPage
{
    PageType type;
    string description;
    int contextData; // Can store additional context like customer ID, category index, etc.
};

vector<NavigationPage> navigationStack;

struct item
{
    string name, category;
    int code, stock = 0;
    double price = 0.00;
};

struct Customer
{
    string name;
    int id;
};

struct Order
{
    int orderId;
    Customer customer;
    vector<pair<int, int>> items; // <item code, quantity>
    double subtotal, tax, serviceCharge, total;
    double discountAmount = 0.0;
    double discountRate = 0.0;
    string discountCode = "";
    string discountType = ""; // "percentage" or "fixed"
    string status;
    string paymentMethod;
    string transactionId;
};

const double SERVICE_CHARGE_RATE = 0.06;
const double TAX_RATE = 0.10;
const int MAX_CATEGORIES = 5;
const int MAX_ITEMS_PER_CATEGORY = 4;

int dailySales[MAX_CATEGORIES] = {0};
double dailyRevenue = 0.0;
int totalCustomers = 0;
int invoiceNumber = 1;

vector<item> catalog;
map<string, string> staffidpass = {{"admin", "123456"}};

// Enhanced discount system with advanced features
map<string, double> promoCodes = {
    {"SAVE5", 0.05},     // 5% discount
    {"SAVE10", 0.10},    // 10% discount
    {"SAVE15", 0.15},    // 15% discount
    {"WELCOME20", 0.20}, // 20% welcome discount
    {"STUDENT", 0.12},   // 12% student discount
    {"SENIOR", 0.15},    // 15% senior citizen discount
    {"MEMBER", 0.08},    // 8% member discount
    {"LOYALTY", 0.25},   // 25% loyalty program discount
    {"WEEKEND", 0.18},   // 18% weekend special
    {"FLASH30", 0.30}    // 30% flash sale
};

// Fixed amount discounts with minimum purchase requirements
struct FixedDiscount
{
    double amount;
    double minPurchase;
    string description;
};

map<string, FixedDiscount> fixedDiscounts = {
    {"FIXED5", {5.00, 25.00, "RM 5 off orders above RM 25"}},
    {"FIXED10", {10.00, 50.00, "RM 10 off orders above RM 50"}},
    {"FIXED20", {20.00, 100.00, "RM 20 off orders above RM 100"}},
    {"NEWBIE", {3.00, 15.00, "RM 3 off for new customers (min RM 15)"}},
    {"BIG50", {50.00, 200.00, "RM 50 off orders above RM 200"}}};

// Tiered discount system - spend more, save more
struct TieredDiscount
{
    double minAmount;
    double discountRate;
    string description;
};

vector<TieredDiscount> tieredDiscounts = {
    {50.00, 0.05, "5% off orders RM 50+"},
    {100.00, 0.10, "10% off orders RM 100+"},
    {200.00, 0.15, "15% off orders RM 200+"},
    {300.00, 0.20, "20% off orders RM 300+"},
    {500.00, 0.25, "25% off orders RM 500+"}};

// Combo discount - buy multiple categories
struct ComboDiscount
{
    vector<string> requiredCategories;
    double discountRate;
    string description;
};

vector<ComboDiscount> comboDiscounts = {
    {{"PERSONAL CARE", "HEALTH SUPPLEMENT"}, 0.12, "12% off when buying Personal Care + Health Supplement"},
    {{"PERSONAL CARE", "TOOL"}, 0.08, "8% off when buying Personal Care + Tools"},
    {{"HEALTH SUPPLEMENT", "TOOL"}, 0.10, "10% off when buying Health Supplement + Tools"}};

// Time-based discounts (Happy Hour)
struct TimeDiscount
{
    int startHour;
    int endHour;
    double discountRate;
    string description;
};

vector<TimeDiscount> timeDiscounts = {
    {9, 11, 0.15, "15% Morning Special (9 AM - 11 AM)"},
    {14, 16, 0.12, "12% Afternoon Delight (2 PM - 4 PM)"},
    {20, 22, 0.18, "18% Night Owl Special (8 PM - 10 PM)"}};

// Tax configuration
struct TaxConfig
{
    double standardRate = 0.10; // 10% standard tax
    double reducedRate = 0.05;  // 5% reduced tax for certain items
    double zeroRate = 0.00;     // 0% tax for tax-exempt items
    bool serviceChargeEnabled = true;
    double serviceChargeRate = 0.06;
} taxConfig;

vector<Order> orderHistory;
vector<Customer> customers;

map<string, int> categoryToIndex = {
    {"PERSONAL CARE", 0},
    {"HEALTH SUPPLEMENT", 1},
    {"TOOL", 2}};

// Forward declarations for functions used before their definitions
vector<item> filterItemsByCategory(const vector<item> &items, string &chosenCategory);
void applyDiscount(Order &order);
void enhancedDiscountSystem(Order &order);
void calculateTaxAndCharges(Order &order);
void displayDiscountMenu();
void displayOffersAndPromotions();
void configureTaxSettings();
void viewCartWithOrder(const Order &order, const vector<item> &items);
double calculateBestTieredDiscount(double orderTotal);
double calculateComboDiscount(const Order &order, const vector<item> &catalog);
double calculateTimeBasedDiscount();
bool isDiscountValid(const string &code, double orderTotal);
void showAdvancedDiscountOptions(Order &order, const vector<item> &catalog);
void saveOrderHistory(const vector<Order> &orders, const string &filename);
void loadOrderHistory(vector<Order> &orders, const string &filename);
void loadItems(vector<item> &items, const string &filename);
void saveTaxConfig(const TaxConfig &config, const string &filename);
void loadTaxConfig(TaxConfig &config, const string &filename);
void returnToPreviousPage();

// Step-based navigation function
bool returnToPreviousStep(OrderStep &currentStep);

// Navigation system functions
void pushPage(PageType type, const string &description, int contextData = 0);
void popPage();
void clearNavigationStack();
bool canGoBack();

string centerText(const string &text, int width)
{
    if (text.length() >= width)
        return text.substr(0, width);
    int totalPadding = width - text.length();
    int left = totalPadding / 2;
    int right = totalPadding - left;
    return string(left, ' ') + text + string(right, ' ');
}

string leftAlign(const string &text, int width)
{
    if (text.length() >= width)
        return text.substr(0, width);
    return text + string(width - text.length(), ' ');
}

string rightAlign(const string &text, int width)
{
    if (text.length() >= width)
        return text.substr(0, width);
    return string(width - text.length(), ' ') + text;
}

// Helper functions for creating beautiful boxes
string createBoxLine(char left, char middle, char right, const vector<int> &widths)
{
    string line = "";
    line += left;
    for (size_t i = 0; i < widths.size(); i++)
    {
        line += string(widths[i], '='); // Use regular '=' instead of Unicode
        if (i < widths.size() - 1)
            line += middle;
    }
    line += right;
    return line;
}

string createDataLine(const vector<string> &data, const vector<int> &widths)
{
    string line = "|";
    for (size_t i = 0; i < data.size() && i < widths.size(); i++)
    {
        line += centerText(data[i], widths[i]);
        line += "|";
    }
    return line;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls"); // Clear screen for Windows
#elif defined(__APPLE__) || defined(__MACH__)
    system("clear"); // Clear screen for macOS
#else
    // Fallback for other systems - use ANSI escape codes
    cout << "\033[2J\033[1;1H";
#endif
    cout.flush(); // Ensure the clear operation is completed
}

void logoscreen()
{
    cout << "         __  _      __ _____        " << endl;
    cout << "  /\\  /\\/__\\/\\_    / //__   \\/\\  /\\ " << endl;
    cout << " / /_/ /_\\ //_\\\\  / /   / /\\/ /_/ / " << endl;
    cout << "/ __  //__/  _  \\/ /___/ / / __  /  " << endl;
    cout << "\\/ /_/\\__/\\_/ \\_/\\____/\\/  \\/ /_/   " << endl;
    cout << "                                    " << endl;
    cout << "        __    ___    __             " << endl;
    cout << "       / _\\  /   \\/\\ \\ \\            " << endl;
    cout << "       \\ \\  / /\\ /  \\/ /            " << endl;
    cout << "       _\\ \\/ /_// /\\  /             " << endl;
    cout << "       \\__/___,'\\_\\ \\/              " << endl;
    cout << "                                    " << endl;
    cout << "       ___          ___             " << endl;
    cout << "      / __\\  /\\  /\\/   \\            " << endl;
    cout << "     /__\\// / /_/ / /\\ /            " << endl;
    cout << "    / \\/  \\/ __  / /_//             " << endl;
    cout << "    \\_____/\\/ /_/___,'              " << endl;
    cout << "                                    " << endl;
}

// Enhanced input helper: read integer within a range with strict validation
int getIntInRange(const string &prompt, int min, int max)
{
    string input;
    int value;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty())
        {
            cout << "ERROR: Input cannot be empty. Enter a number between " << min << " and " << max << ".\n";
            continue;
        }

        // Check if input contains only digits (and optional minus sign at start)
        bool isValid = true;
        for (size_t i = 0; i < input.length(); i++)
        {
            if (i == 0 && input[i] == '-' && input.length() > 1)
                continue; // Allow negative sign at start
            if (!isdigit(input[i]))
            {
                isValid = false;
                break;
            }
        }

        if (!isValid)
        {
            cout << "ERROR: Invalid input. Please enter only numbers between " << min << " and " << max << ".\n";
            continue;
        }

        // Convert to integer and check range
        try
        {
            value = stoi(input);
            if (value >= min && value <= max)
            {
                return value;
            }
            else
            {
                cout << "ERROR: Number out of range. Enter a number between " << min << " and " << max << ".\n";
            }
        }
        catch (const exception &)
        {
            cout << "ERROR: Invalid number. Enter a number between " << min << " and " << max << ".\n";
        }
    }
}

// Enhanced input validation with error messages for positive doubles
double getPositiveDouble(const string &prompt)
{
    string input;
    double value;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty())
        {
            cout << "ERROR: Input cannot be empty. Please enter a positive number.\n";
            continue;
        }

        // Check if input is a valid number format
        bool isValid = true;
        bool hasDecimal = false;
        for (size_t i = 0; i < input.length(); i++)
        {
            if (input[i] == '.' && !hasDecimal)
            {
                hasDecimal = true;
                continue;
            }
            if (!isdigit(input[i]))
            {
                isValid = false;
                break;
            }
        }

        if (!isValid)
        {
            cout << "ERROR: Invalid input. Please enter only positive numbers (e.g., 12.50).\n";
            continue;
        }

        // Convert to double and check if positive
        try
        {
            value = stod(input);
            if (value > 0)
            {
                return value;
            }
            else
            {
                cout << "ERROR: Please enter a positive number greater than 0.\n";
            }
        }
        catch (const exception &)
        {
            cout << "ERROR: Invalid number format. Please enter a positive number.\n";
        }
    }
}

int getPositiveInt(const string &prompt)
{
    string input;
    int value;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty())
        {
            cout << "ERROR: Input cannot be empty. Please enter a positive integer.\n";
            continue;
        }

        // Check if input contains only digits
        bool isValid = true;
        for (char c : input)
        {
            if (!isdigit(c))
            {
                isValid = false;
                break;
            }
        }

        if (!isValid)
        {
            cout << "ERROR: Invalid input. Please enter only positive integers (e.g., 1, 2, 3).\n";
            continue;
        }

        // Convert to integer and check if positive
        try
        {
            value = stoi(input);
            if (value > 0)
            {
                return value;
            }
            else
            {
                cout << "ERROR: Please enter a positive integer greater than 0.\n";
            }
        }
        catch (const exception &)
        {
            cout << "ERROR: Invalid integer. Please enter a positive integer.\n";
        }
    }
}

// Function to get limited character input with validation
string getLimitedInput(const string &prompt, int maxLength, const string &fieldName = "Input")
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        if (input.empty())
        {
            cout << "Error: " << fieldName << " cannot be empty. Please try again.\n";
            continue;
        }

        if (input.length() > maxLength)
        {
            cout << "Error: " << fieldName << " cannot exceed " << maxLength << " characters. Please try again.\n";
            continue;
        }

        // Check for invalid characters (optional - you can customize this)
        bool hasInvalidChar = false;
        for (char c : input)
        {
            if (c < 32 || c > 126)
            { // Only allow printable ASCII characters
                hasInvalidChar = true;
                break;
            }
        }

        if (hasInvalidChar)
        {
            cout << "Error: " << fieldName << " contains invalid characters. Please use only letters, numbers, and basic symbols.\n";
            continue;
        }

        return input;
    }
}

int display_MainMenu()
{
    cout << "\n";
    cout << "+========================================+\n";
    cout << "|" << centerText("MAIN MENU", 42) << "|\n";
    cout << "+========================================+\n";
    cout << "|                                        |\n";
    cout << "|   1. Order Entry                       |\n";
    cout << "|                                        |\n";
    cout << "|   2. View Offers & Discounts           |\n";
    cout << "|                                        |\n";
    cout << "|   3. Staff Entry                       |\n";
    cout << "|                                        |\n";
    cout << "|   4. Exit System                       |\n";
    cout << "|                                        |\n";
    cout << "+========================================+\n";
    cout << "+----------------------------------------+\n";
    cout << "|" << centerText("Please select an option (1-4):", 40) << "|\n";
    cout << "+----------------------------------------+\n";
    return getIntInRange("> Your choice: ", 1, 4);
}

void displayItems(const vector<item> &items)
{
    clearScreen();
    cout << "\n";
    cout << "+============================================================================================+\n";
    cout << "|" << centerText("PRODUCT CATALOG", 98) << "|\n";
    cout << "+===========================+==========+===========================+==============+==========+\n";
    cout << "|" << centerText("CATEGORIES", 27) << "|" << centerText("ID", 10) << "|" << centerText("ITEM NAME", 27) << "|" << centerText("PRICE", 14) << "|" << centerText("STOCK", 10) << "|\n";
    cout << "+===========================+==========+===========================+==============+==========+\n";

    if (items.empty())
    {
        cout << "|" << centerText("ITEM: No items available", 91) << "|\n";
        cout << "+===========================================================================================+\n";
        return;
    }

    for (const auto &item : items)
    {
        ostringstream oss;
        oss << "RM " << fixed << setprecision(2) << item.price;
        string priceStr = oss.str();

        // Enhanced stock status indicator
        string stockStatus = to_string(item.stock);
        if (item.stock == 0)
        {
            stockStatus = "ERROR: OUT";
        }
        else if (item.stock < 5)
        {
            stockStatus = "WARNING: " + stockStatus + " LOW";
        }
        else if (item.stock < 10)
        {
            stockStatus = "🟡 " + stockStatus;
        }
        else
        {
            stockStatus = "SUCCESS: " + stockStatus;
        }

        cout << "|" << centerText(item.category, 27)
             << "|" << centerText(to_string(item.code), 10)
             << "|" << centerText(item.name, 27)
             << "|" << centerText(priceStr, 14)
             << "|" << centerText(stockStatus, 11)
             << "|\n";
    }
    cout << "+===========================+==========+===========================+==============+==========+\n";
}

int getItemID(const vector<item> &items)
{
    int code;
    while (true)
    {
        cout << "Enter item ID: ";
        if (cin >> code)
        {
            for (const auto &item : items)
            {
                if (item.code == code)
                {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return code;
                }
            }
            cout << "Item not found, please enter the correct item ID." << endl;
        }
        else
        {
            cout << "Invalid input, please enter a valid item ID." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int getQuantity()
{
    int quantity;
    while (true)
    {
        cout << "ITEM: Enter the quantity: ";
        if (cin >> quantity && quantity > 0 && quantity <= 100) // reasonable limit
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return quantity;
        }
        else
        {
            if (quantity > 100)
            {
                cout << "ERROR: Quantity too large. Maximum 100 items per order.\n";
            }
            else
            {
                cout << "ERROR: Invalid quantity. Please enter a positive number (1-100).\n";
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

char getYesNo()
{
    string input;
    while (true)
    {
        cout << "Enter Y/N: ";
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty())
        {
            cout << "ERROR: Input cannot be empty. Please enter 'Y' or 'N': ";
            continue;
        }

        // Check if input is exactly one character
        if (input.length() != 1)
        {
            cout << "ERROR: Please enter only one character (Y or N): ";
            continue;
        }

        char choice = toupper(input[0]);
        if (choice == 'Y' || choice == 'N')
        {
            return choice;
        }
        else
        {
            cout << "ERROR: Invalid input. Please enter only 'Y' or 'N': ";
        }
    }
}

// Function to get a single character choice with validation
char getCharChoice(const string &prompt, const string &validChars)
{
    string input;
    while (true)
    {
        cout << prompt;
        getline(cin, input);

        // Remove leading and trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Check if input is empty
        if (input.empty())
        {
            cout << "ERROR: Input cannot be empty. Please enter one of: " << validChars << "\n";
            continue;
        }

        // Check if input is exactly one character
        if (input.length() != 1)
        {
            cout << "ERROR: Please enter only one character from: " << validChars << "\n";
            continue;
        }

        char choice = tolower(input[0]);

        // Check if the character is in the valid choices
        string lowerValidChars = validChars;
        transform(lowerValidChars.begin(), lowerValidChars.end(), lowerValidChars.begin(), ::tolower);

        if (lowerValidChars.find(choice) != string::npos)
        {
            return choice;
        }
        else
        {
            cout << "ERROR: Invalid choice. Please enter one of: " << validChars << "\n";
        }
    }
}

int getItemCode(const vector<item> &items, int code)
{
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i].code == code)
        {
            return i;
        }
    }
    return -1;
}

void initializeCatalog()
{
    // Load catalog from items.txt file
    loadItems(catalog, "items.txt");
}

void saveItems(const vector<item> &items, const string &filename)
{
    ofstream fout(filename);
    if (!fout.is_open())
    {
        cout << "WARNING:  Error: Could not save to " << filename << ". Check file permissions.\n";
        return;
    }

    for (const auto &it : items)
    {
        fout << it.name << ',' << it.category << ',' << it.code << ',' << it.stock << ',' << it.price << '\n';
    }

    if (fout.good())
    {
        cout << "SUCCESS: Data saved successfully to " << filename << "\n";
    }
    else
    {
        cout << "WARNING:  Warning: There may have been an error saving to " << filename << "\n";
    }
    fout.close();
}

void loadItems(vector<item> &items, const string &filename)
{
    items.clear();
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << "Error: Could not open " << filename << " for reading. Check if the file exists and is accessible.\n";
        return;
    }
    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        item it;
        string stockStr, priceStr, codeStr;
        getline(ss, it.name, ',');
        getline(ss, it.category, ',');
        getline(ss, codeStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, priceStr, ',');
        try
        {
            it.code = stoi(codeStr);
            it.stock = stoi(stockStr);
            it.price = stod(priceStr);
            items.push_back(it);
        }
        catch (...)
        {
            cout << "Error: Invalid item data in file: " << line << endl;
        }
    }
    fin.close();
}

void saveTaxConfig(const TaxConfig &config, const string &filename)
{
    ofstream fout(filename);
    if (!fout.is_open())
    {
        cout << "WARNING:  Warning: Could not save tax configuration to " << filename << "\n";
        return;
    }

    // Save tax configuration in simple format
    fout << config.standardRate << "\n";
    fout << config.serviceChargeEnabled << "\n";
    fout << config.serviceChargeRate << "\n";

    if (fout.good())
    {
        cout << "SUCCESS: Tax configuration saved successfully to " << filename << "\n";
    }
    else
    {
        cout << "WARNING:  Warning: There may have been an error saving tax configuration to " << filename << "\n";
    }
    fout.close();
}

void loadTaxConfig(TaxConfig &config, const string &filename)
{
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << "INFO:  No existing tax configuration found. Using default settings.\n";
        return;
    }

    string line;
    try
    {
        // Load standard tax rate
        if (getline(fin, line))
        {
            config.standardRate = stod(line);
        }

        // Load service charge enabled status
        if (getline(fin, line))
        {
            config.serviceChargeEnabled = (line == "1");
        }

        // Load service charge rate
        if (getline(fin, line))
        {
            config.serviceChargeRate = stod(line);
        }

        cout << "SUCCESS: Tax configuration loaded successfully from " << filename << "\n";
    }
    catch (...)
    {
        cout << "WARNING:  Error reading tax configuration. Using default settings.\n";
        // Reset to default values
        config.standardRate = 0.10;
        config.serviceChargeEnabled = true;
        config.serviceChargeRate = 0.06;
    }
    fin.close();
}

void loadCustomers(vector<Customer> &customers, const string &filename)
{
    customers.clear();
    ifstream fin(filename);
    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        Customer c;
        string idStr;
        getline(ss, c.name, ',');
        getline(ss, idStr, ',');
        c.id = stoi(idStr);
        customers.push_back(c);
    }
    fin.close();
}

void saveOrderHistory(const vector<Order> &orders, const string &filename)
{
    ofstream fout(filename);
    for (const auto &order : orders)
    {
        fout << order.orderId << ','
             << order.customer.name << ','
             << order.customer.id << ',';
        for (const auto &item : order.items)
        {
            fout << item.first << ':' << item.second << ';';
        }
        fout << ','
             << fixed << setprecision(2) << order.subtotal << ','
             << fixed << setprecision(2) << order.tax << ','
             << fixed << setprecision(2) << order.serviceCharge << ','
             << fixed << setprecision(2) << order.total << ','
             << fixed << setprecision(2) << order.discountAmount << ','
             << fixed << setprecision(2) << order.discountRate << ','
             << order.discountCode << ','
             << order.discountType << ','
             << order.status << ','
             << order.paymentMethod << ','
             << order.transactionId << '\n';
    }
    fout.close();
}

void loadOrderHistory(vector<Order> &orders, const string &filename)
{
    orders.clear();
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << "Error: Could not open " << filename << " for reading. Creating new order history.\n";
        return;
    }
    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        Order order;
        string itemStr, subtotalStr, taxStr, serviceChargeStr, totalStr;
        getline(ss, itemStr, ',');
        order.orderId = stoi(itemStr);
        getline(ss, order.customer.name, ',');
        getline(ss, itemStr, ',');
        order.customer.id = stoi(itemStr);
        string itemsPart;
        getline(ss, itemsPart, ',');
        stringstream itemsStream(itemsPart);
        string itemPair;
        while (getline(itemsStream, itemPair, ';'))
        {
            if (!itemPair.empty())
            {
                size_t colonPos = itemPair.find(':');
                if (colonPos != string::npos)
                {
                    int code = stoi(itemPair.substr(0, colonPos));
                    int qty = stoi(itemPair.substr(colonPos + 1));
                    order.items.push_back({code, qty});
                }
            }
        }
        getline(ss, subtotalStr, ',');
        order.subtotal = stod(subtotalStr);
        getline(ss, taxStr, ',');
        order.tax = stod(taxStr);
        getline(ss, serviceChargeStr, ',');
        order.serviceCharge = stod(serviceChargeStr);
        getline(ss, totalStr, ',');
        order.total = stod(totalStr);

        // Read discount fields (with backward compatibility)
        string discountAmountStr, discountRateStr;
        if (getline(ss, discountAmountStr, ',') && !discountAmountStr.empty())
        {
            try
            {
                order.discountAmount = stod(discountAmountStr);
            }
            catch (...)
            {
                order.discountAmount = 0.0;
            }
        }
        else
        {
            order.discountAmount = 0.0;
        }

        if (getline(ss, discountRateStr, ',') && !discountRateStr.empty())
        {
            try
            {
                order.discountRate = stod(discountRateStr);
            }
            catch (...)
            {
                order.discountRate = 0.0;
            }
        }
        else
        {
            order.discountRate = 0.0;
        }

        if (!getline(ss, order.discountCode, ','))
        {
            order.discountCode = "";
        }
        if (!getline(ss, order.discountType, ','))
        {
            order.discountType = "";
        }

        string statusStr, paymentStr, transactionStr;
        if (!getline(ss, statusStr, ','))
        {
            statusStr = "";
        }
        if (!getline(ss, paymentStr, ','))
        {
            paymentStr = "";
        }
        getline(ss, transactionStr); // Last field, no delimiter needed

        order.status = statusStr;
        order.paymentMethod = paymentStr;
        order.transactionId = transactionStr;

        // Set default values if fields are empty (for backward compatibility)
        if (order.status.empty())
        {
            order.status = "Completed"; // Assume old orders were completed
        }
        if (order.paymentMethod.empty())
        {
            order.paymentMethod = "Cash"; // Assume old orders were cash payments
        }

        orders.push_back(order);
    }
    fin.close();
    // Update invoiceNumber to the highest orderId + 1
    if (!orders.empty())
    {
        invoiceNumber = orders.back().orderId + 1;
    }
}

void addToCart(vector<pair<int, int>> &cart, vector<item> &catalog)
{
    string chosenCategory;
    vector<item> filteredItems = filterItemsByCategory(catalog, chosenCategory);
    displayItems(filteredItems);

    int code = getItemID(filteredItems);
    if (code == -1)
        return;

    int catalogIndex = getItemCode(catalog, code);
    if (catalogIndex == -1)
    {
        cout << "Error: Item not found in master catalog.\n";
        return;
    }

    int alreadyInCart = 0;
    for (auto &p : cart)
        if (p.first == code)
            alreadyInCart += p.second;
    int available = catalog[catalogIndex].stock - alreadyInCart;

    int quantity;
    while (true)
    {
        cout << "Enter the quantity (Available: " << available << "): ";
        quantity = getQuantity();
        if (quantity > available)
        {
            cout << "Error: Requested quantity exceeds available stock.\n";
        }
        else
        {
            break;
        }
    }

    bool itemFoundInCart = false;
    for (auto &pair : cart)
    {
        if (pair.first == code)
        {
            pair.second += quantity;
            itemFoundInCart = true;
            break;
        }
    }

    if (!itemFoundInCart)
    {
        cart.push_back({code, quantity});
    }

    // Get item name for better feedback
    string itemName = "";
    for (const auto &item : catalog)
    {
        if (item.code == code)
        {
            itemName = item.name;
            break;
        }
    }

    cout << "SUCCESS: " << quantity << "x " << itemName << " added to cart successfully!\n";
    cout << "Press Enter to continue...";
    cin.get();
    // Refresh display to show current cart state
    displayItems(filteredItems);
}

double viewCart(const vector<pair<int, int>> &cart, const vector<item> &items)
{
    clearScreen();
    double subtotal = 0.0;
    cout << "\n";
    cout << "+============================================================================+\n";
    cout << "|" << centerText("[1] SHOPPING CART", 78) << "|\n";
    cout << "+===========================+==========+==============+======================+\n";
    cout << "|" << centerText("ITEM", 27) << "|" << centerText("QTY", 10) << "|" << centerText("PRICE", 14) << "|" << centerText("SUBTOTAL", 22) << "|\n";
    cout << "+===========================+==========+==============+======================+\n";

    if (cart.empty())
    {
        cout << "|" << centerText("", 82) << "|\n";
        cout << "|" << centerText("[1] Your cart is empty", 82) << "|\n";
        cout << "|" << centerText("Please add items to continue", 82) << "|\n";
        cout << "|" << centerText("", 82) << "|\n";
        cout << "+==================================================================================+\n";
        return 0.0;
    }

    for (const auto &pair : cart)
    {
        int index = getItemCode(items, pair.first);
        double lineTotal = pair.second * items[index].price;
        subtotal += lineTotal;

        ostringstream priceStream, subtotalStream;
        priceStream << "RM " << fixed << setprecision(2) << items[index].price;
        subtotalStream << "RM " << fixed << setprecision(2) << lineTotal;

        cout << "|" << centerText(items[index].name, 27)
             << "|" << centerText(to_string(pair.second), 10)
             << "|" << centerText(priceStream.str(), 14)
             << "|" << centerText(subtotalStream.str(), 22)
             << "|\n";
    }
    cout << "+===========================+==========+==============+======================+\n";

    ostringstream subtotalStr;
    subtotalStr << "RM " << fixed << setprecision(2) << subtotal;
    cout << "|" << rightAlign("SUBTOTAL:", 53) << "|" << centerText(subtotalStr.str(), 22) << "|\n";
    cout << "+=====================================================+======================+\n";
    return subtotal;
}

void viewCartWithOrder(const Order &order, const vector<item> &items)
{
    clearScreen();
    cout << "\n";
    cout << "+============================================================================+\n";
    cout << "|" << centerText("[1] SHOPPING CART", 78) << "|\n";
    cout << "+===========================+==========+==============+======================+\n";
    cout << "|" << centerText("ITEM", 27) << "|" << centerText("QTY", 10) << "|" << centerText("PRICE", 14) << "|" << centerText("SUBTOTAL", 22) << "|\n";
    cout << "+===========================+==========+==============+======================+\n";

    if (order.items.empty())
    {
        cout << "|" << centerText("", 82) << "|\n";
        cout << "|" << centerText("[1] Your cart is empty", 82) << "|\n";
        cout << "|" << centerText("Please add items to continue", 82) << "|\n";
        cout << "|" << centerText("", 82) << "|\n";
        cout << "+==================================================================================+\n";
        return;
    }

    for (const auto &pair : order.items)
    {
        int index = getItemCode(items, pair.first);
        double lineTotal = pair.second * items[index].price;

        ostringstream priceStream, subtotalStream;
        priceStream << "RM " << fixed << setprecision(2) << items[index].price;
        subtotalStream << "RM " << fixed << setprecision(2) << lineTotal;

        cout << "|" << centerText(items[index].name, 27)
             << "|" << centerText(to_string(pair.second), 10)
             << "|" << centerText(priceStream.str(), 14)
             << "|" << centerText(subtotalStream.str(), 22)
             << "|\n";
    }
    cout << "+===========================+==========+==============+======================+\n";

    // Show subtotal
    ostringstream subtotalStr;
    subtotalStr << "RM " << fixed << setprecision(2) << order.subtotal;
    cout << "|" << rightAlign("SUBTOTAL:", 53) << "|" << centerText(subtotalStr.str(), 22) << "|\n";

    // Show service charge if enabled
    if (taxConfig.serviceChargeEnabled && order.serviceCharge > 0)
    {
        ostringstream serviceStr;
        serviceStr << "RM " << fixed << setprecision(2) << order.serviceCharge;
        cout << "|" << rightAlign("Service Charge (" + to_string((int)(taxConfig.serviceChargeRate * 100)) + "%):", 53)
             << "|" << centerText(serviceStr.str(), 22) << "|\n";
    }

    // Show tax
    if (order.tax > 0)
    {
        ostringstream taxStr;
        taxStr << "RM " << fixed << setprecision(2) << order.tax;
        cout << "|" << rightAlign("Tax (" + to_string((int)(taxConfig.standardRate * 100)) + "%):", 53)
             << "|" << centerText(taxStr.str(), 22) << "|\n";
    }

    // Show discount if applied
    if (order.discountAmount > 0)
    {
        ostringstream discountStr;
        discountStr << "-RM " << fixed << setprecision(2) << order.discountAmount;
        string discountLabel = "Discount";
        if (!order.discountCode.empty())
        {
            discountLabel += " (" + order.discountCode;
            if (order.discountType == "percentage")
            {
                discountLabel += " - " + to_string((int)(order.discountRate * 100)) + "%";
            }
            discountLabel += "):";
        }
        else
        {
            discountLabel += ":";
        }
        cout << "|" << rightAlign(discountLabel, 53) << "|" << centerText(discountStr.str(), 22) << "|\n";
    }

    cout << "+=====================================================+======================+\n";

    // Show total
    ostringstream totalStr;
    totalStr << "RM " << fixed << setprecision(2) << order.total;
    cout << "|" << rightAlign("TOTAL:", 53) << "|" << centerText(totalStr.str(), 22) << "|\n";
    cout << "+=====================================================+======================+\n";
}

void generateInvoice(const Order &order, const vector<item> &items)
{
    clearScreen();
    cout << "\n";
    cout << "+================================================================================================+\n";
    cout << "|" << centerText("INVOICE", 98) << "|\n";
    cout << "+================================================================================================+\n";
    cout << "|" << centerText("", 96) << "|\n";
    cout << "|" << leftAlign("  Invoice #: " + to_string(order.orderId), 96) << "|\n";
    cout << "|" << leftAlign("  Customer:  " + order.customer.name, 96) << "|\n";

    if (!order.transactionId.empty())
    {
        cout << "|" << leftAlign("  Transaction: " + order.transactionId, 96) << "|\n";
    }

    cout << "|" << centerText("", 96) << "|\n";
    cout << "+===================================+==========+==============+==================================+\n";
    cout << "|              ITEM                 |   QTY    |    PRICE     |            SUBTOTAL              |\n";
    cout << "+===================================+==========+==============+==================================+\n";

    for (const auto &pair : order.items)
    {
        int index = getItemCode(items, pair.first);
        if (index != -1)
        {
            double lineTotal = pair.second * items[index].price;

            ostringstream priceStream, lineTotalStream;
            priceStream << "RM " << fixed << setprecision(2) << items[index].price;
            lineTotalStream << "RM " << fixed << setprecision(2) << lineTotal;

            cout << "|" << centerText(items[index].name, 35)
                 << "|" << centerText(to_string(pair.second), 10)
                 << "|" << centerText(priceStream.str(), 14)
                 << "|" << centerText(lineTotalStream.str(), 34)
                 << "|\n";
        }
    }

    cout << "+===================================+==========+==============+==================================+\n";
    cout << "|                                                             |                                  |\n";

    // Create properly formatted strings for the breakdown
    ostringstream subtotalStr, serviceStr, taxStr, totalStr;
    subtotalStr << "RM " << fixed << setprecision(2) << order.subtotal;
    serviceStr << "RM " << fixed << setprecision(2) << order.serviceCharge;
    taxStr << "RM " << fixed << setprecision(2) << order.tax;
    totalStr << "RM " << fixed << setprecision(2) << order.total;

    cout << "|" << rightAlign("Subtotal:", 61) << "|" << centerText(subtotalStr.str(), 34) << "|\n";

    // Show service charge with rate
    if (taxConfig.serviceChargeEnabled && order.serviceCharge > 0)
    {
        string serviceLabel = "Service Charge (" + to_string((int)(taxConfig.serviceChargeRate * 100)) + "%):";
        cout << "|" << rightAlign(serviceLabel, 61) << "|" << centerText(serviceStr.str(), 34) << "|\n";
    }

    // Show tax with rate
    if (order.tax > 0)
    {
        string taxLabel = "Tax (" + to_string((int)(taxConfig.standardRate * 100)) + "%):";
        cout << "|" << rightAlign(taxLabel, 61) << "|" << centerText(taxStr.str(), 34) << "|\n";
    }

    // Show discount if applied
    if (order.discountAmount > 0)
    {
        ostringstream discountStr;
        discountStr << "-RM " << fixed << setprecision(2) << order.discountAmount;
        string discountLabel = "Discount";
        if (!order.discountCode.empty())
        {
            discountLabel += " (" + order.discountCode;
            if (order.discountType == "percentage")
            {
                discountLabel += " - " + to_string((int)(order.discountRate * 100)) + "%";
            }
            discountLabel += "):";
        }
        else
        {
            discountLabel += ":";
        }
        cout << "|" << rightAlign(discountLabel, 61) << "|" << centerText(discountStr.str(), 34) << "|\n";
    }

    cout << "|                                                             |                                  |\n";
    cout << "+=============================================================+==================================+\n";
    cout << "|" << rightAlign("TOTAL:", 61) << "|" << centerText(totalStr.str(), 34) << "|\n";
    cout << "+=============================================================+==================================+\n";
    cout << "|                                                                                                |\n";

    if (!order.paymentMethod.empty())
    {
        cout << "|" << leftAlign("  Payment Method: " + order.paymentMethod, 96) << "|\n";
    }

    cout << "|" << leftAlign("  Status: " + order.status, 96) << "|\n";
    cout << "|                                                                                                |\n";
    cout << "|" << centerText("🎉 Thank you for your business! 🎉", 100) << "|\n";
    cout << "|                                                                                                |\n";
    cout << "+================================================================================================+\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}

bool processPayment(double amount, string &method, string &transactionId)
{
    char choice;
    while (true)
    {
        cout << "\n";
        cout << "+======================================================+\n";
        cout << "|" << centerText("PAYMENT GATEWAY", 56) << "|\n";
        cout << "+======================================================+\n";
        cout << "|                                                      |\n";

        ostringstream amountStr;
        amountStr << "Total Amount: RM " << fixed << setprecision(2) << amount;
        cout << "|" << centerText(amountStr.str(), 54) << "|\n";
        cout << "|                                                      |\n";
        cout << "+======================================================+\n";
        cout << "|   [A] Credit Card                                    |\n";
        cout << "|                                                      |\n";
        cout << "|   [S] Cash Payment                                   |\n";
        cout << "|                                                      |\n";
        cout << "|   [D] Touch n Go (eWallet)                           |\n";
        cout << "|                                                      |\n";
        cout << "|   [Q] Cancel Payment                                 |\n";
        cout << "|                                                      |\n";
        cout << "+======================================================+\n";
        choice = getCharChoice("> Enter your choice: ", "ASDQ");

        switch (choice)
        {
        case 'a':
            method = "Credit Card";
            cout << "\nProcessing Credit Card payment...\n";
            break;
        case 's':
            method = "Cash";
            cout << "\nProcessing Cash payment...\n";
            break;
        case 'd':
            method = "Touch n Go";
            cout << "\nProcessing Touch 'n Go payment...\n";
            break;
        case 'q':
            cout << "\nPayment cancelled by user.\n";
            return false;
        default:
            cout << "ERROR: Invalid input. Please select A, S, D, or Q.\n";
            continue;
        }
        break;
    }

    // Generate transaction ID with timestamp for uniqueness
    auto now = time(0);
    transactionId = "ZX" + to_string(now % 100000) + to_string(rand() % 1000);

    // Simulate processing time and show success
    cout << "\nProcessing payment...\n";
    cout << "+======================================================+\n";
    cout << "|                  PAYMENT SUCCESS                    |\n";
    cout << "+======================================================+\n";
    cout << "|                                                      |\n";
    cout << "|   Amount:        RM " << fixed << setprecision(2) << setw(31) << left << amount << " |\n";
    cout << "|   Method:        " << setw(35) << left << method << " |\n";
    cout << "|   Transaction:   " << setw(35) << left << transactionId << " |\n";
    cout << "|                                                      |\n";
    cout << "|          Thank you for your payment! 🎉              |\n";
    cout << "|                                                      |\n";
    cout << "+======================================================+\n";
    return true;
}

void generateReceipt(const Order &order)
{
    clearScreen();
    cout << "\n";
    cout << "+==================================================================================================+\n";
    cout << "|" << centerText("PAYMENT RECEIPT", 100) << "|\n";
    cout << "+==================================================================================================+\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "|" << centerText("PAYMENT SUCCESSFUL", 99) << "|\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "+==================================================================================================+\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "|" << leftAlign("  Order ID:        " + to_string(order.orderId), 100) << "|\n";
    cout << "|" << leftAlign("  👤 Customer:        " + order.customer.name, 100) << "|\n";
    cout << "|" << leftAlign("  [A] Payment Method:  " + order.paymentMethod, 100) << "|\n";
    cout << "|" << leftAlign("  🆔 Transaction ID:  " + order.transactionId, 100) << "|\n";

    ostringstream amountStr;
    amountStr << "RM " << fixed << setprecision(2) << order.total;
    cout << "|" << leftAlign("  PRICE: Amount Paid:     " + amountStr.str(), 100) << "|\n";
    cout << "|" << leftAlign("  📅 Date:            " + string(__DATE__), 100) << "|\n";
    cout << "|" << leftAlign("  🕐 Time:            " + string(__TIME__), 100) << "|\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "+==================================================================================================+\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "|" << centerText("🎉 Thank you for your purchase! 🎉", 102) << "|\n";
    cout << "|" << centerText("Your order is being processed", 98) << "|\n";
    cout << "|" << centerText("Have a wonderful day!", 98) << "|\n";
    cout << "|" << centerText("", 98) << "|\n";
    cout << "+==================================================================================================+\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}

void displayOrderReport(const vector<Order> &orders, const vector<item> &catalog)
{
    clearScreen();
    double totalSales = 0.0;
    int totalItems = 0;

    cout << "\n";
    cout << "+==================================================================================================+\n";
    cout << "|                                REPORT: ORDER HISTORY REPORT                                           |\n";
    cout << "+==================================================================================================+\n";

    if (orders.empty())
    {
        cout << "+==================================================================================================+\n";
        cout << "|                                                                                                  |\n";
        cout << "|                                  📭 No Orders Found                                              |\n";
        cout << "|                              No order history available                                          |\n";
        cout << "|                                                                                                  |\n";
        cout << "+==================================================================================================+\n";
        return;
    }

    // Header for order details with improved spacing
    cout << "\n+=======+======================+==================+================+===============+==================+\n";
    cout << "|  ID   |      CUSTOMER        |     STATUS       |    PAYMENT     |   DISCOUNT    |      ITEMS       |\n";
    cout << "+=======+======================+==================+================+===============+==================+\n";

    for (const auto &order : orders)
    {
        // Count total items in this order
        int orderItemCount = 0;
        for (const auto &itemPair : order.items)
        {
            orderItemCount += itemPair.second;
        }
        totalItems += orderItemCount;

        // Status indicator
        string statusDisplay = order.status.empty() ? "Unknown" : order.status;
        if (order.status == "Completed")
        {
            statusDisplay = "SUCCESS: Completed";
        }
        else if (order.status == "Pending")
        {
            statusDisplay = "PENDING: Pending";
        }
        else if (order.status == "Cancelled")
        {
            statusDisplay = "ERROR: Cancelled";
        }
        else if (order.status.empty())
        {
            statusDisplay = "❓ Unknown";
        }

        // Payment method with icon
        string paymentDisplay = order.paymentMethod.empty() ? "Unknown" : order.paymentMethod;
        if (order.paymentMethod == "Credit Card")
        {
            paymentDisplay = "[A] Card";
        }
        else if (order.paymentMethod == "Cash")
        {
            paymentDisplay = "[S] Cash";
        }
        else if (order.paymentMethod == "Touch n Go" || order.paymentMethod == "Touch 'n Go")
        {
            paymentDisplay = "TnG: TnG";
        }
        else if (order.paymentMethod.empty())
        {
            paymentDisplay = "❓ Unknown";
        }

        ostringstream totalStream;
        totalStream << "RM " << fixed << setprecision(2) << order.total;

        cout << "|" << centerText(to_string(order.orderId), 7)
             << "|" << centerText(order.customer.name, 22)
             << "|" << centerText(statusDisplay, 19)
             << "|" << centerText(paymentDisplay, 18)
             << "|" << centerText(totalStream.str(), 15)
             << "|" << centerText(to_string(orderItemCount) + " items", 18)
             << "|\n";

        totalSales += order.total;
    }

    cout << "+=======+======================+==================+================+===============+==================+\n";

    // Detailed items breakdown
    cout << "|                                                                                                     |\n";
    cout << "|                                   � DETAILED ORDER BREAKDOWN                                        |\n";
    cout << "|                                                                                                     |\n";
    cout << "+=====================================================================================================+\n";

    for (const auto &order : orders)
    {
        cout << "\n+============================================================================================================+\n";
        cout << "| INVOICE Order #" << order.orderId << " - " << order.customer.name;
        if (!order.transactionId.empty())
        {
            cout << " (🆔 Transaction: " << order.transactionId << ")";
        }
        cout << string(max(0, 76 - (int)(to_string(order.orderId).length() + order.customer.name.length() +
                                         (order.transactionId.empty() ? 0 : order.transactionId.length() + 19))),
                       ' ')
             << "                  |\n";
        cout << "+==============================+=======+================+===============+===================+================+\n";
        cout << "|         ITEM NAME            |  QTY  |     PRICE      |   SUBTOTAL    |  DISCOUNT PRICE   |  ORDER TOTAL   |\n";
        cout << "+==============================+=======+================+===============+===================+================+\n";

        bool firstItem = true;
        for (const auto &itemPair : order.items)
        {
            int index = getItemCode(catalog, itemPair.first);
            if (index != -1)
            {
                double lineTotal = itemPair.second * catalog[index].price;
                ostringstream lineTotalStream, priceStream, totalStream, discountStream;
                lineTotalStream << "RM " << fixed << setprecision(2) << lineTotal;
                priceStream << "RM " << fixed << setprecision(2) << catalog[index].price;

                // Show discount and order total only on first item
                if (firstItem)
                {
                    totalStream << "RM " << fixed << setprecision(2) << order.total;
                    if (order.discountAmount > 0)
                    {
                        ostringstream discountAmountStream;
                        discountAmountStream << "RM " << fixed << setprecision(2) << order.discountAmount;
                        discountStream << discountAmountStream.str();
                    }
                    else
                    {
                        discountStream << "RM 0.00";
                    }
                    firstItem = false;
                }
                else
                {
                    totalStream << "";
                    discountStream << "";
                }

                cout << "|" << centerText(catalog[index].name, 30)
                     << "|" << centerText(to_string(itemPair.second), 7)
                     << "|" << centerText(priceStream.str(), 16)
                     << "|" << centerText(lineTotalStream.str(), 15)
                     << "|" << centerText(discountStream.str(), 19)
                     << "|" << centerText(totalStream.str(), 16) << "|\n";
            }
        }

        cout << "+==============================+=======+================+===============+===================+================+\n";

        // Order Information Table
        cout << "|                                                                                                            |\n";
        cout << "+============================================================================================================+\n";
        cout << "|" << centerText("[A] PAYMENT METHOD", 56) << "|" << centerText("REPORT: STATUS", 55) << "|\n";
        cout << "+============================================================================================================+\n";

        // Prepare payment method display
        string paymentDisplay = order.paymentMethod.empty() ? "❓ Unknown" : "� " + order.paymentMethod;
        if (paymentDisplay.length() > 45)
        {
            paymentDisplay = paymentDisplay.substr(0, 45) + "...";
        }

        // Prepare discount display with consistent length
        string discountDisplay;
        const int TARGET_LENGTH = 42; // Increased target length to accommodate longer codes

        if (order.discountAmount > 0)
        {
            if (!order.discountCode.empty())
            {
                discountDisplay = "CODE: " + order.discountCode;
                if (order.discountType == "percentage")
                {
                    discountDisplay += " (" + to_string((int)(order.discountRate * 100)) + "%)";
                }
                // Ensure consistent length - truncate if too long
                if (discountDisplay.length() > TARGET_LENGTH)
                {
                    discountDisplay = discountDisplay.substr(0, TARGET_LENGTH - 3) + "...";
                }
            }
            else
            {
                discountDisplay = "�️ " + order.discountCode;
            }
        }
        else
        {
            discountDisplay = "ERROR: NO DISCOUNT";
        }

        // Prepare status display
        string statusDisplay;
        if (order.status == "Completed")
        {
            statusDisplay = "SUCCESS: " + order.status;
        }
        else if (order.status == "Pending")
        {
            statusDisplay = "PENDING: " + order.status;
        }
        else if (order.status == "Cancelled")
        {
            statusDisplay = "ERROR: " + order.status;
        }
        else if (order.status.empty())
        {
            statusDisplay = "❓ Unknown";
        }
        else
        {
            statusDisplay = "REVIEW: " + order.status;
        }

        cout << "|" << centerText(paymentDisplay, 56) << "|" << centerText(statusDisplay, 54) << "|\n";
        cout << "+============================================================================================================+\n";
    }

    // Summary section moved here - after detailed breakdown
    cout << "\n+==================================================================================================+\n";
    cout << "|                                    STATS: SUMMARY STATISTICS                                         |\n";
    cout << "+==========================================================+=======================================+\n";
    cout << "|                    METRIC                                |                VALUE                  |\n";
    cout << "+==========================================================+=======================================+\n";

    // Calculate average order value
    double averageOrder = (orders.size() > 0) ? totalSales / orders.size() : 0.0;

    // Create formatted strings for proper table display
    ostringstream ordersStr, itemsStr, revenueStr, avgStr;
    ordersStr << orders.size();
    itemsStr << totalItems;
    revenueStr << "RM " << fixed << setprecision(2) << totalSales;
    avgStr << "RM " << fixed << setprecision(2) << averageOrder;

    cout << "| ITEM: Total Orders                                          |" << centerText(ordersStr.str(), 39) << "|\n";
    cout << "+==========================================================+=======================================+\n";
    cout << "| PRODUCT CATALOG  Total Items Sold                                      |" << centerText(itemsStr.str(), 39) << "|\n";
    cout << "+==========================================================+=======================================+\n";
    cout << "| PRICE: Total Revenue                                         |" << centerText(revenueStr.str(), 39) << "|\n";
    cout << "+==========================================================+=======================================+\n";
    cout << "| REPORT: Average Order Value                                   |" << centerText(avgStr.str(), 39) << "|\n";
    cout << "+==========================================================+=======================================+\n";

    cout << "\nPress Enter to continue...";
    cin.get();
}

bool staffLogin()
{
    string username, password;
    cout << "Staff Login\nUsername: ";
    cin >> username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Password: ";
    cin >> password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (staffidpass.find(username) != staffidpass.end() && staffidpass[username] == password)
    {
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid username or password.\n";
    return false;
}

void addItem(vector<item> &items)
{
    item newItem;
    clearScreen();

    cout << "\n";
    cout << "+==================================================================================================+\n";
    cout << "|" << centerText("ITEM: ADD NEW ITEM TO INVENTORY", 100) << "|\n";
    cout << "+==================================================================================================+\n";
    cout << "|                                                                                                  |\n";
    cout << "|  Welcome to the Item Management System! You can add new items or restock existing ones.          |\n";
    cout << "|                                                                                                  |\n";
    cout << "+==================================================================================================+\n";

    // Category Selection with improved UI
CATEGORY_SELECTION:
    while (true)
    {
        cout << "\n";
        cout << "+========================================+\n";
        cout << "|" << centerText("CATEGORY: SELECT CATEGORY", 42) << "|\n";
        cout << "+========================================+\n";
        cout << "|                                        |\n";
        cout << "|   🧴   1. PERSONAL CARE                |\n";
        cout << "|                                        |\n";
        cout << "|   💊   2. HEALTH SUPPLEMENT            |\n";
        cout << "|                                        |\n";
        cout << "|   🔧   3. TOOL                         |\n";
        cout << "|                                        |\n";
        cout << "|   ➕   4. Custom Category              |\n";
        cout << "|                                        |\n";
        cout << "|   🔙   5. Return to Staff Menu         |\n";
        cout << "|                                        |\n";
        cout << "+========================================+\n";
        cout << "+----------------------------------------+\n";
        cout << "│" << centerText("Please select a category (1-5):", 40) << "│\n";
        cout << "+----------------------------------------+\n";

        int categoryChoice = getIntInRange("> Your choice: ", 1, 5);

        if (categoryChoice == 5)
        {
            return;
        }

        switch (categoryChoice)
        {
        case 1:
            newItem.category = "PERSONAL CARE";
            break;
        case 2:
            newItem.category = "HEALTH SUPPLEMENT";
            break;
        case 3:
            newItem.category = "TOOL";
            break;
        case 4:
            cout << "\n+----------------------------------------+\n";
            cout << "│" << centerText("Enter Custom Category:", 40) << "│\n";
            cout << "+----------------------------------------+\n";
            newItem.category = getLimitedInput("> Category name (max 30 characters): ", 30, "Category name");
            if (newItem.category.empty())
            {
                cout << "ERROR: Category cannot be empty. Please try again.\n";
                cout << "Press Enter to continue...";
                cin.get();
                continue;
            }
            // Convert to uppercase for consistency
            transform(newItem.category.begin(), newItem.category.end(), newItem.category.begin(), ::toupper);
            break;
        }

        cout << "\nSUCCESS: Selected category: " << newItem.category << "\n";
        cout << "Continue with this category? (Y/N): ";
        if (getYesNo() == 'Y')
            break;
    }

    // Name Selection with improved UI
    while (true)
    {
        cout << "\n";
        cout << "+==================================================================================================+\n";
        cout << "|" << centerText("CODE:  SELECT ITEM NAME", 104) << "|\n";
        cout << "+==================================================================================================+\n";

        vector<string> availableNames;
        for (const auto &item : catalog)
        {
            if (item.category == newItem.category)
            {
                bool nameExists = false;
                for (const string &name : availableNames)
                {
                    if (name == item.name)
                    {
                        nameExists = true;
                        break;
                    }
                }
                if (!nameExists)
                {
                    availableNames.push_back(item.name);
                }
            }
        }

        if (availableNames.empty())
        {
            cout << "\n📝 No existing items in " << newItem.category << " category.\n";
            cout << "+----------------------------------------+\n";
            cout << "│" << centerText("Enter new item name:", 40) << "│\n";
            cout << "+----------------------------------------+\n";
            newItem.name = getLimitedInput("> Item name (max 30 characters): ", 30, "Item name");
            if (!newItem.name.empty())
            {
                break;
            }
            cout << "ERROR: Name cannot be empty. Please try again.\n";
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        cout << "\n+--------------------------------------------------------------------------------------------------+\n";
        cout << "│" << centerText("Existing items in " + newItem.category + ":", 98) << "│\n";
        cout << "+--------------------------------------------------------------------------------------------------+\n";
        for (size_t i = 0; i < availableNames.size(); ++i)
        {
            cout << "│  " << setw(2) << (i + 1) << ". " << setw(88) << left << availableNames[i] << "    │\n";
        }
        cout << "│  " << setw(2) << (availableNames.size() + 1) << ". " << setw(88) << left << "🆕 Create New Item Name" << "      │\n";
        cout << "│  " << setw(2) << (availableNames.size() + 2) << ". " << setw(88) << left << "🔙 Return to Category Selection" << "      │\n";
        cout << "+--------------------------------------------------------------------------------------------------+\n";

        int nameChoice = getIntInRange("> Your choice: ", 1, availableNames.size() + 2);

        if (nameChoice == (int)availableNames.size() + 2)
        {
            // User wants to return to category selection
            // We need to break out of both loops, so use a goto to restart from category selection
            goto CATEGORY_SELECTION;
        }

        if (nameChoice == (int)availableNames.size() + 1)
        {
            cout << "\n+----------------------------------------+\n";
            cout << "│" << centerText("Enter new item name:", 40) << "│\n";
            cout << "+----------------------------------------+\n";
            newItem.name = getLimitedInput("> Item name (max 30 characters): ", 30, "Item name");
            if (newItem.name.empty())
            {
                cout << "ERROR: Name cannot be empty. Please try again.\n";
                cout << "Press Enter to continue...";
                cin.get();
                continue;
            }
        }
        else
        {
            newItem.name = availableNames[nameChoice - 1];
        }

        cout << "\nSUCCESS: Selected item: " << newItem.name << "\n";
        cout << "Continue with this item name? (Y/N): ";
        if (getYesNo() == 'Y')
            break;
    }

    // Check for existing item and handle restock with improved UI
    bool isRestock = false;
    int existingIndex = -1;
    for (size_t i = 0; i < catalog.size(); ++i)
    {
        if (catalog[i].name == newItem.name && catalog[i].category == newItem.category)
        {
            existingIndex = i;
            break;
        }
    }

    if (existingIndex != -1)
    {
        cout << "\n";
        cout << "+==================================================================================================+\n";
        cout << "|" << centerText("🔄 ITEM ALREADY EXISTS", 100) << "|\n";
        cout << "+==================================================================================================+\n";
        cout << "|                                                                                                  |\n";
        cout << "|  ITEM: Item Found     : " << setw(29) << left << newItem.name << "                                               |\n";
        cout << "|  CATEGORY: Category       : " << setw(29) << left << newItem.category << "                                               |\n";
        cout << "|  CODE:  Code           : " << setw(29) << left << catalog[existingIndex].code << "                                               |\n";
        cout << "|  PRICE: Price          : RM " << setw(26) << left << fixed << setprecision(2) << catalog[existingIndex].price << "                                               |\n";
        cout << "|  REPORT: Current Stock  : " << setw(29) << left << catalog[existingIndex].stock << "                                               |\n";
        cout << "|                                                                                                  |\n";
        cout << "+==================================================================================================+\n";
        cout << "\n🔄 Would you like to restock this existing item? (Y/N): ";

        if (getYesNo() == 'Y')
        {
            isRestock = true;
        }
        else
        {
            cout << "\n📝 Proceeding to add a new item with different specifications...\n";
        }
    }

    if (isRestock)
    {
        // Restock existing item with improved UI
        cout << "\n";
        cout << "+========================================+\n";
        cout << "|" << centerText("ITEM: RESTOCK ITEM", 42) << "|\n";
        cout << "+========================================+\n";
        cout << "|                                        |\n";
        cout << "|  Current Stock: " << right << catalog[existingIndex].stock << "                    |\n";
        cout << "|                                        |\n";
        cout << "+========================================+\n";

        int additionalStock = getIntInRange("> Enter additional stock quantity: ", 0, 9999);

        catalog[existingIndex].stock += additionalStock;

        cout << "\n";
        cout << "+========================================+\n";
        cout << "|" << centerText("SUCCESS: RESTOCK SUCCESSFUL", 41) << "|\n";
        cout << "+========================================+\n";
        cout << "|                                        |\n";
        cout << "|  Item: " << setw(29) << left << newItem.name.substr(0, 29) << "   |\n";
        cout << "|  Added: " << setw(28) << right << additionalStock << "   |\n";
        cout << "|  New Stock: " << setw(24) << right << catalog[existingIndex].stock << "   |\n";
        cout << "|                                        |\n";
        cout << "+========================================+\n";

        cout << "\nPress Enter to continue...";
        cin.get();
        return;
    }
    else
    {
        // Add new item with improved UI
        cout << "\n";
        cout << "+==================================================================================================+\n";
        cout << "|" << centerText("➕ ADD NEW ITEM", 100) << "|\n";
        cout << "+==================================================================================================+\n";

        // Code input
        cout << "\n+----------------------------------------+\n";
        cout << "│" << centerText("CODE:  Enter Item Code", 40) << "│\n";
        cout << "+----------------------------------------+\n";
        while (true)
        {
            newItem.code = getIntInRange("> Enter unique 4-digit code (e.g., 1001): ", 1000, 9999);

            bool codeExists = false;
            for (const auto &item : items)
            {
                if (item.code == newItem.code)
                {
                    codeExists = true;
                    break;
                }
            }

            if (codeExists)
            {
                cout << "ERROR: Error: Code " << newItem.code << " is already in use. Please try another code.\n";
                continue;
            }

            cout << "SUCCESS: Code " << newItem.code << " is available. Use this code? (Y/N): ";
            if (getYesNo() == 'Y')
                break;
        }

        // Price input
        cout << "\n+----------------------------------------+\n";
        cout << "│" << centerText("PRICE: Enter Item Price", 40) << "│\n";
        cout << "+----------------------------------------+\n";
        while (true)
        {
            cout << "> Enter price (RM): ";
            if (cin >> newItem.price && newItem.price > 0)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "SUCCESS: Price set to RM " << fixed << setprecision(2) << newItem.price << ". Confirm? (Y/N): ";
                if (getYesNo() == 'Y')
                    break;
            }
            else
            {
                cout << "ERROR: Invalid price. Please enter a value greater than 0.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        // Stock input
        cout << "\n+----------------------------------------+\n";
        cout << "│" << centerText("ITEM: Enter Initial Stock", 40) << "│\n";
        cout << "+----------------------------------------+\n";
        newItem.stock = getIntInRange("> Enter initial stock quantity: ", 0, 9999);

        // Final confirmation with summary
        cout << "\n";
        cout << "+==================================================================================================+\n";
        cout << "|" << centerText("REVIEW NEW ITEM", 98) << "|\n";
        cout << "+==================================================================================================+\n";
        cout << "|                                                                                                  |\n";
        cout << "|  Category: " << setw(15) << left << newItem.category << "                                        |\n";
        cout << "|  Name: " << setw(19) << left << newItem.name << "                                                |\n";
        cout << "|  Code: " << setw(20) << left << newItem.code << "                                                |\n";
        cout << "|  Price: RM " << setw(15) << left << fixed << setprecision(2) << newItem.price << "                |\n";
        cout << "|  REPORT: Stock: " << setw(19) << left << newItem.stock << "                                            |\n";
        cout << "|                                                                                                  |\n";
        cout << "+==================================================================================================+\n";

        cout << "\n🔍 Review the information above. Add this item to inventory? (Y/N): ";
        if (getYesNo() == 'Y')
        {
            items.push_back(newItem);
            cout << "\n";
            cout << "+========================================+\n";
            cout << "|" << centerText("SUCCESS: ITEM ADDED SUCCESSFULLY", 40) << "|\n";
            cout << "+========================================+\n";
            cout << "|                                        |\n";
            cout << "|  " << setw(34) << left << newItem.name.substr(0, 34) << "  |\n";
            cout << "|  has been added to inventory!          |\n";
            cout << "|                                        |\n";
            cout << "+========================================+\n";
        }
        else
        {
            cout << "\nERROR: Item addition cancelled. No changes made.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }
}

vector<item> filterItemsByCategory(const vector<item> &items, string &chosenCategory)
{
    vector<string> categories;
    for (const auto &item : items)
    {
        if (find(categories.begin(), categories.end(), item.category) == categories.end())
        {
            categories.push_back(item.category);
        }
    }

    cout << "\nChoose a category to view:\n";
    for (size_t i = 0; i < categories.size(); ++i)
    {
        cout << i + 1 << ". " << categories[i] << endl;
    }
    cout << categories.size() + 1 << ". All Items" << endl;
    cout << "Enter your choice: ";

    int choice;
    while (!(cin >> choice) || choice < 1 || choice > (int)categories.size() + 1)
    {
        cout << "Invalid input. Please enter a valid option: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<item> filtered;
    if (choice == (int)categories.size() + 1)
    {
        chosenCategory = "ALL";
        filtered = items; // Use the latest items with updated stock
    }
    else
    {
        chosenCategory = categories[choice - 1];
        for (const auto &item : items)
        {
            if (item.category == chosenCategory)
            {
                filtered.push_back(item); // Use the latest stock from items
            }
        }
    }
    return filtered;
}

void applyDiscount(Order &order)
{
    // Get catalog reference for advanced discount calculations
    extern vector<item> catalog;
    showAdvancedDiscountOptions(order, catalog);
}

void enhancedDiscountSystem(Order &order)
{
    cout << "\n+==============================================================================================+\n";
    cout << "|" << centerText("[2] AVAILABLE DISCOUNTS", 94) << "|\n";
    cout << "+==============================================================================================+\n";
    cout << "|" << centerText("PERCENTAGE DISCOUNTS", 94) << "|\n";
    cout << "+==============================================================================================+\n";

    // Display percentage discounts
    int index = 1;
    vector<pair<string, double>> percentageOptions;
    for (const auto &code : promoCodes)
    {
        cout << "|" << leftAlign(" " + to_string(index) + ". " + code.first + " - " + to_string((int)(code.second * 100)) + "% off", 94) << "|\n";
        percentageOptions.push_back(code);
        index++;
    }

    cout << "+==============================================================================================+\n";
    cout << "|" << centerText("FIXED AMOUNT DISCOUNTS", 94) << "|\n";
    cout << "+==============================================================================================+\n";

    // Display fixed discounts
    vector<pair<string, FixedDiscount>> fixedOptions;
    for (const auto &code : fixedDiscounts)
    {
        ostringstream amountStr;
        amountStr << "RM " << fixed << setprecision(2) << code.second.amount;
        cout << "|" << leftAlign(" " + to_string(index) + ". " + code.first + " - " + amountStr.str() + " off (" + code.second.description + ")", 94) << "|\n";
        fixedOptions.push_back(code);
        index++;
    }

    cout << "|" << leftAlign(" " + to_string(index) + ". No discount", 94) << "|\n";
    cout << "+==============================================================================================+\n";

    cout << "Select a discount option (1-" << index << "): ";
    int choice = getIntInRange("> Your choice: ", 1, index);

    if (choice <= percentageOptions.size())
    {
        // Apply percentage discount
        auto selected = percentageOptions[choice - 1];
        double discountAmount = order.total * selected.second;
        order.total -= discountAmount;

        // Store discount information
        order.discountAmount = discountAmount;
        order.discountRate = selected.second;
        order.discountCode = selected.first;
        order.discountType = "percentage";

        cout << "\nSUCCESS: Applied " << selected.first << " - " << (int)(selected.second * 100)
             << "% discount (RM " << fixed << setprecision(2) << discountAmount << ")\n";
        cout << "New Total: RM " << fixed << setprecision(2) << order.total << "\n";
    }
    else if (choice <= percentageOptions.size() + fixedOptions.size())
    {
        // Apply fixed discount
        int fixedIndex = choice - percentageOptions.size() - 1;
        auto selected = fixedOptions[fixedIndex];

        // Check minimum purchase requirement
        if (order.total < selected.second.minPurchase)
        {
            cout << "\nERROR: " << selected.first << " requires minimum purchase of RM "
                 << fixed << setprecision(2) << selected.second.minPurchase << "\n";
            cout << "Your current order total: RM " << fixed << setprecision(2) << order.total << "\n";
            return;
        }

        double discountAmount = selected.second.amount;

        if (discountAmount >= order.total)
        {
            discountAmount = order.total * 0.95; // Maximum 95% discount
        }
        order.total -= discountAmount;

        // Store discount information
        order.discountAmount = discountAmount;
        order.discountRate = 0.0; // Not applicable for fixed discounts
        order.discountCode = selected.first;
        order.discountType = "fixed";

        cout << "\nSUCCESS: Applied " << selected.first << " - RM " << fixed << setprecision(2)
             << discountAmount << " discount\n";
        cout << "New Total: RM " << fixed << setprecision(2) << order.total << "\n";
    }
    else
    {
        cout << "\nNo discount applied.\n";
    }
}

void calculateTaxAndCharges(Order &order)
{
    // Calculate service charge
    if (taxConfig.serviceChargeEnabled)
    {
        order.serviceCharge = order.subtotal * taxConfig.serviceChargeRate;
    }
    else
    {
        order.serviceCharge = 0.0;
    }

    // Calculate tax (applied on subtotal + service charge)
    double taxableAmount = order.subtotal + order.serviceCharge;
    order.tax = taxableAmount * taxConfig.standardRate;

    // Update total
    order.total = order.subtotal + order.serviceCharge + order.tax;
}

void configureTaxSettings()
{
    cout << "\n+==============================================================================================+\n";
    cout << "|" << centerText("CONFIG: TAX CONFIGURATION", 99) << "|\n";
    cout << "+==============================================================================================+\n";
    cout << "|" << leftAlign(" Current Settings:", 94) << "|\n";

    ostringstream taxRateStr, serviceRateStr;
    taxRateStr << fixed << setprecision(1) << (taxConfig.standardRate * 100) << "%";
    serviceRateStr << fixed << setprecision(1) << (taxConfig.serviceChargeRate * 100) << "%";

    cout << "|" << leftAlign(" Standard Tax Rate: " + taxRateStr.str(), 94) << "|\n";
    cout << "|" << leftAlign(" Service Charge Rate: " + serviceRateStr.str(), 94) << "|\n";
    cout << "|" << leftAlign(" Service Charge Enabled: " + string(taxConfig.serviceChargeEnabled ? "Yes" : "No"), 94) << "|\n";
    cout << "+==============================================================================================+\n";
    cout << "|" << leftAlign(" 1. Change Standard Tax Rate", 94) << "|\n";
    cout << "|" << leftAlign(" 2. Change Service Charge Rate", 94) << "|\n";
    cout << "|" << leftAlign(" 3. Toggle Service Charge", 94) << "|\n";
    cout << "|" << leftAlign(" 4. Return to Main Menu", 94) << "|\n";
    cout << "+==============================================================================================+\n";

    cout << "Enter your choice (1-4): ";
    int choice = getIntInRange("> Your choice: ", 1, 4);

    switch (choice)
    {
    case 1:
        cout << "Enter new tax rate (0-50%): ";
        {
            int taxPercent = getIntInRange("> Tax rate: ", 0, 50);
            taxConfig.standardRate = taxPercent / 100.0;
            cout << "SUCCESS: Tax rate updated to " << taxPercent << "%\n";
            saveTaxConfig(taxConfig, "tax_config.txt");
        }
        break;
    case 2:
        cout << "Enter new service charge rate (0-20%): ";
        {
            int servicePercent = getIntInRange("> Service rate: ", 0, 20);
            taxConfig.serviceChargeRate = servicePercent / 100.0;
            cout << "SUCCESS: Service charge rate updated to " << servicePercent << "%\n";
            saveTaxConfig(taxConfig, "tax_config.txt");
        }
        break;
    case 3:
        taxConfig.serviceChargeEnabled = !taxConfig.serviceChargeEnabled;
        cout << "SUCCESS: Service charge " << (taxConfig.serviceChargeEnabled ? "enabled" : "disabled") << "\n";
        saveTaxConfig(taxConfig, "tax_config.txt");
        break;
    case 4:
        return;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    configureTaxSettings(); // Show menu again
}

void displayOffersAndPromotions()
{
    clearScreen();
    cout << "\n+===============================================================================================+\n";
    cout << "|" << centerText("[2] CURRENT OFFERS & PROMOTIONS", 97) << "|\n";
    cout << "+===============================================================================================+\n";
    cout << "|" << centerText("💯 PERCENTAGE DISCOUNTS", 97) << "|\n";
    cout << "+===============================================================================================+\n";

    for (const auto &promo : promoCodes)
    {
        string description;
        if (promo.first == "SAVE5")
            description = "General 5% off your order";
        else if (promo.first == "SAVE10")
            description = "Save 10% on all items";
        else if (promo.first == "SAVE15")
            description = "Special 15% discount";
        else if (promo.first == "WELCOME20")
            description = "Welcome bonus for new customers";
        else if (promo.first == "STUDENT")
            description = "Student discount with valid ID";
        else if (promo.first == "SENIOR")
            description = "Senior citizen special discount";
        else if (promo.first == "MEMBER")
            description = "Membership loyalty discount";
        else if (promo.first == "LOYALTY")
            description = "VIP loyalty program members";
        else
            description = "Special promotional offer";

        cout << "| CODE:  " << leftAlign(promo.first + " - " + to_string((int)(promo.second * 100)) + "% OFF", 30)
             << "│ " << leftAlign(description, 58) << " |\n";
    }

    cout << "+===============================================================================================+\n";
    cout << "|" << centerText("PRICE: FIXED AMOUNT DISCOUNTS", 97) << "|\n";
    cout << "+===============================================================================================+\n";

    for (const auto &fixed : fixedDiscounts)
    {
        cout << "| [S]  " << leftAlign(fixed.first + " - RM " + to_string((int)fixed.second.amount) + " OFF", 30)
             << "│ " << leftAlign(fixed.second.description, 58) << "|\n";
    }

    cout << "+===============================================================================================+\n";
    cout << "|" << centerText("🎯 AUTOMATIC DISCOUNTS", 97) << "|\n";
    cout << "+===============================================================================================+\n";

    cout << "| 🏆 TIERED DISCOUNTS (Applied automatically based on order total):                             |\n";
    for (const auto &tier : tieredDiscounts)
    {
        cout << "|    • " << leftAlign(tier.description, 89) << "|\n";
    }

    cout << "|                                                                                               |\n";
    cout << "| 🎪 COMBO DISCOUNTS (Applied when buying from multiple categories):                            |\n";
    for (const auto &combo : comboDiscounts)
    {
        cout << "|    • " << leftAlign(combo.description, 89) << "|\n";
    }

    cout << "|                                                                                               |\n";
    cout << "| ⏰ TIME-BASED DISCOUNTS (Applied during specific hours):                                      |\n";
    for (const auto &timeDeal : timeDiscounts)
    {
        cout << "|    • " << leftAlign(timeDeal.description, 89) << "|\n";
    }

    cout << "+===============================================================================================+\n";
    cout << "|" << centerText("💡 HOW TO USE", 97) << "|\n";
    cout << "+===============================================================================================+\n";
    cout << "| • Enter the promo code during checkout to apply discount                                      |\n";
    cout << "| • Codes are case-insensitive (SAVE5 = save5 = Save5)                                          |\n";
    cout << "| • Only one discount code can be applied per order                                             |\n";
    cout << "| • Some offers may require minimum purchase amount                                             |\n";
    cout << "| • Fixed discounts have maximum 95% off limit for order total                                  |\n";
    cout << "+===============================================================================================+\n";
    cout << "|" << centerText("INFO:  Current Tax & Service Information", 100) << "|\n";
    cout << "+===============================================================================================+\n";
    cout << "| • Service Charge: " << (taxConfig.serviceChargeEnabled ? to_string((int)(taxConfig.serviceChargeRate * 100)) + "% (applied to subtotal)" : "Disabled") << string(51, ' ') << "|\n";
    cout << "| • Tax Rate: " << to_string((int)(taxConfig.standardRate * 100)) << "% (applied to subtotal + service charge)" << string(40, ' ') << "|\n";
    cout << "| • Discounts are applied to the final total (after tax & service charge)                       |\n";
    cout << "+===============================================================================================+\n";

    cout << "\n🔙 Press Enter to return to main menu...";
    cin.get();
}

// Navigation system implementation
void pushPage(PageType type, const string &description, int contextData)
{
    NavigationPage page = {type, description, contextData};
    navigationStack.push_back(page);
}

void popPage()
{
    if (!navigationStack.empty())
    {
        navigationStack.pop_back();
    }
}

void clearNavigationStack()
{
    navigationStack.clear();
}

bool canGoBack()
{
    return navigationStack.size() > 1; // Need at least 2 pages to go back
}

void returnToPreviousPage()
{
    if (!canGoBack())
    {
        clearScreen();
        cout << "📍 Already at the main menu.\n";
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    // Remove current page
    popPage();

    // Get the previous page
    if (!navigationStack.empty())
    {
        NavigationPage previousPage = navigationStack.back();
        clearScreen();
        cout << "🔙 Returning to: " << previousPage.description << "\n";
        cout << "Press Enter to continue...";
        cin.get();

        // The actual navigation logic will be handled by the calling function
        // by checking the navigation stack state
    }
    else
    {
        clearScreen();
        cout << "HOME: Returning to main menu...\n";
        cout << "Press Enter to continue...";
        cin.get();
    }
}

// Step-based navigation function
bool returnToPreviousStep(OrderStep &currentStep)
{
    if (currentStep == PAYMENT_STEP)
    {
        currentStep = CART_REVIEW_STEP; // Payment → Cart Review
        cout << "\n🔙 Returning to cart review step...\n";
        return true;
    }
    else if (currentStep == CART_REVIEW_STEP)
    {
        currentStep = ITEM_SELECTION_STEP; // Cart Review → Item Selection
        cout << "\n🔙 Returning to item selection step...\n";
        return true;
    }
    else if (currentStep == ITEM_SELECTION_STEP)
    {
        cout << "\n🔙 Returning to main menu (no previous step)...\n";
        return false; // No previous step, exit to main menu
    }
    return false;
}

void OrderEntry(int customerId, Order &order)
{
    pushPage(ORDER_ENTRY, "Order Entry", customerId);

    vector<pair<int, int>> cart;
    vector<int> originalStocks(catalog.size()); // Store original stock values
    cout << "\nCustomer " << customerId << " placing order:\n";

    // Get customer name with character limit
    order.customer.name = getLimitedInput("Enter customer name (max 30 characters): ", 30, "Customer name");

    // Main order flow with proper step navigation
    OrderStep currentStep = ITEM_SELECTION_STEP;

    while (true)
    {
        if (currentStep == ITEM_SELECTION_STEP)
        {
            // Step 1: Item Selection
            pushPage(ITEM_SELECTION, "Item Selection");
            addToCart(cart, catalog);
            popPage(); // Remove item selection page after completing

            cout << "\n";
            cout << "+========================================+\n";
            cout << "|" << centerText("💡 WHAT'S NEXT?", 42) << "|\n";
            cout << "+========================================+\n";
            cout << "|                                        |\n";
            cout << "|   [1]   1. Add more items               |\n";
            cout << "|                                        |\n";
            cout << "|   [A]   2. Proceed to Payment           |\n";
            cout << "|                                        |\n";
            cout << "|   ↩️   3. Return to Previous Page       |\n";
            cout << "|                                        |\n";
            cout << "+========================================+\n";
            cout << "+----------------------------------------+\n";
            cout << "│" << centerText("Please select an option (1-3):", 40) << "│\n";
            cout << "+----------------------------------------+\n";
            int choice = getIntInRange("> Your choice: ", 1, 3);
            if (choice == 3)
            {
                // Use step-based navigation to go back
                if (returnToPreviousStep(currentStep))
                {
                    // Successfully returned to previous step (shouldn't happen in item selection)
                    continue;
                }
                else
                {
                    // No previous step, exit to main menu
                    returnToPreviousPage();
                    popPage(); // Remove ORDER_ENTRY page
                    return;
                }
            }
            else if (choice == 2)
            {
                if (cart.empty())
                {
                    cout << "\nERROR: Your cart is empty! Please add items before proceeding to payment.\n";
                    cout << "Press Enter to continue...";
                    cin.get();
                    continue; // Stay in item selection
                }
                currentStep = CART_REVIEW_STEP; // Move to next step
            }
            // choice == 1 => continue in item selection (loop again)
        }
        else if (currentStep == CART_REVIEW_STEP)
        {
            // Step 2: Cart Review
            order.items = cart;
            order.subtotal = 0.0;
            for (const auto &pair : cart)
            {
                int index = getItemCode(catalog, pair.first);
                order.subtotal += catalog[index].price * pair.second;
                string category = catalog[index].category;
                if (categoryToIndex.find(category) != categoryToIndex.end())
                {
                    dailySales[categoryToIndex[category]] += pair.second;
                }
            }

            // Calculate tax and service charges first
            calculateTaxAndCharges(order);

            // Then apply discount
            pushPage(DISCOUNT_SELECTION, "Discount Selection");
            applyDiscount(order);
            popPage(); // Remove discount selection page

            order.orderId = invoiceNumber++;
            order.customer.id = customerId;
            order.status = "Pending";

            pushPage(CART_VIEW, "Cart Review");
            viewCartWithOrder(order, catalog);

            // Display total in a nice box
            cout << "\n";
            cout << "+----------------------------------------+\n";
            ostringstream totalStr;
            totalStr << "Total: RM " << fixed << setprecision(2) << order.total;
            cout << "│" << centerText(totalStr.str(), 40) << "│\n";
            cout << "+----------------------------------------+\n";

            // Display options in a nice box
            cout << "\n";
            cout << "+========================================+\n";
            cout << "|" << centerText("💡 WHAT'S NEXT?", 42) << "|\n";
            cout << "+========================================+\n";
            cout << "|                                        |\n";
            cout << "|   [A]   1. Proceed to Payment           |\n";
            cout << "|                                        |\n";
            cout << "|   ↩️   2. Return to Previous Page       |\n";
            cout << "|                                        |\n";
            cout << "+========================================+\n";
            cout << "+----------------------------------------+\n";
            cout << "│" << centerText("Please select an option (1-2):", 40) << "│\n";
            cout << "+----------------------------------------+\n";
            int proceedChoice = getIntInRange("> Your choice: ", 1, 2);
            if (proceedChoice == 2)
            {
                // Use step-based navigation to go back
                popPage(); // Remove CART_VIEW page
                if (returnToPreviousStep(currentStep))
                {
                    // Successfully returned to previous step
                    continue;
                }
                else
                {
                    // No previous step, exit to main menu
                    returnToPreviousPage();
                    popPage(); // Remove ORDER_ENTRY page
                    return;
                }
            }
            else if (proceedChoice == 1)
            {
                popPage();                  // Remove CART_VIEW page after proceeding
                currentStep = PAYMENT_STEP; // Move to payment step
            }
        }
        else if (currentStep == PAYMENT_STEP)
        {
            // Step 3: Payment Processing
            break; // Exit the loop to proceed with payment
        }
    }

    cout << "Confirm payment for RM " << fixed << setprecision(2) << order.total << "? (Y/N): ";
    if (getYesNo() == 'N')
    {
        cout << "Payment cancelled.\n";
        popPage(); // Remove ORDER_ENTRY page
        return;
    }

    cout << "Thank you for your order, Customer " << customerId << "!\n";

    // Store original stock values before payment
    for (size_t i = 0; i < catalog.size(); ++i)
    {
        originalStocks[i] = catalog[i].stock;
    }

    // Process payment
    pushPage(PAYMENT_GATEWAY, "Payment Processing");
    if (processPayment(order.total, order.paymentMethod, order.transactionId))
    {
        order.status = "Completed";

        // Add to order history AFTER successful payment and status/payment updates
        orderHistory.push_back(order);

        for (const auto &pair : order.items)
        {
            int index = getItemCode(catalog, pair.first);
            catalog[index].stock -= pair.second; // Deduct stock only after successful payment
        }
        generateInvoice(order, catalog);
        generateReceipt(order);
        dailyRevenue += order.total;
        totalCustomers++;
        saveOrderHistory(orderHistory, "orders.txt");
        cout << "\nPress Enter to return to the home screen...";
        cin.get();

        popPage(); // Remove PAYMENT_GATEWAY page
        popPage(); // Remove ORDER_ENTRY page
    }
    else
    {
        // Restore original stock values on payment cancellation
        for (size_t i = 0; i < catalog.size(); ++i)
        {
            catalog[i].stock = originalStocks[i];
        }
        cout << "Order canceled, stock restored to original values.\n";
    }
}

// Advanced discount calculation functions
double calculateBestTieredDiscount(double orderTotal)
{
    double bestDiscount = 0.0;
    for (const auto &tier : tieredDiscounts)
    {
        if (orderTotal >= tier.minAmount)
        {
            bestDiscount = tier.discountRate;
        }
    }
    return bestDiscount;
}

double calculateComboDiscount(const Order &order, const vector<item> &catalog)
{
    // Check which categories are in the order
    set<string> orderCategories;
    for (const auto &orderItem : order.items)
    {
        int index = getItemCode(catalog, orderItem.first);
        orderCategories.insert(catalog[index].category);
    }

    // Find the best combo discount
    double bestComboRate = 0.0;
    for (const auto &combo : comboDiscounts)
    {
        bool hasAllCategories = true;
        for (const auto &requiredCategory : combo.requiredCategories)
        {
            if (orderCategories.find(requiredCategory) == orderCategories.end())
            {
                hasAllCategories = false;
                break;
            }
        }
        if (hasAllCategories && combo.discountRate > bestComboRate)
        {
            bestComboRate = combo.discountRate;
        }
    }

    return bestComboRate;
}

double calculateTimeBasedDiscount()
{
    time_t now = time(0); // <-- Add this line to define 'now'
    tm timeinfo;
#if defined(_MSC_VER)
    localtime_s(&timeinfo, &now);
    tm *timeinfoPtr = &timeinfo;
#else
    tm *timeinfoPtr = localtime(&now);
#endif
    return 0.0;
}

bool isDiscountValid(const string &code, double orderTotal)
{
    auto it = fixedDiscounts.find(code);
    if (it != fixedDiscounts.end())
    {
        return orderTotal >= it->second.minPurchase;
    }
    return true; // Percentage discounts don't have minimum requirements
}

void showAdvancedDiscountOptions(Order &order, const vector<item> &catalog)
{
    cout << "\n+==============================================================================================+\n";
    cout << "|" << centerText("🎯 SMART DISCOUNT ANALYZER", 96) << "|\n";
    cout << "+==============================================================================================+\n";

    ostringstream currentTotalStr;
    currentTotalStr << "RM " << fixed << setprecision(2) << order.total;
    cout << "|" << leftAlign(" Current Order Total: " + currentTotalStr.str(), 94) << "|\n";
    cout << "+==============================================================================================+\n";

    // Check automatic tiered discount
    double tieredRate = calculateBestTieredDiscount(order.total);
    if (tieredRate > 0)
    {
        double tieredAmount = order.total * tieredRate;
        ostringstream tieredAmountStr;
        tieredAmountStr << "RM " << fixed << setprecision(2) << tieredAmount;
        cout << "| 🏆 " << leftAlign("AUTOMATIC TIER DISCOUNT: " + to_string((int)(tieredRate * 100)) + "% off (" + tieredAmountStr.str() + ")", 90) << " |\n";
    }

    // Check combo discount
    double comboRate = calculateComboDiscount(order, catalog);
    if (comboRate > 0)
    {
        double comboAmount = order.total * comboRate;
        ostringstream comboAmountStr;
        comboAmountStr << "RM " << fixed << setprecision(2) << comboAmount;
        cout << "| 🎪 " << leftAlign("COMBO BONUS: " + to_string((int)(comboRate * 100)) + "% off (" + comboAmountStr.str() + ")", 90) << " |\n";
    }

    // Check time-based discount
    double timeRate = calculateTimeBasedDiscount();
    if (timeRate > 0)
    {
        double timeAmount = order.total * timeRate;
        ostringstream timeAmountStr;
        timeAmountStr << "RM " << fixed << setprecision(2) << timeAmount;
        cout << "| ⏰ " << leftAlign("TIME SPECIAL: " + to_string((int)(timeRate * 100)) + "% off (" + timeAmountStr.str() + ")", 90) << "|\n";
    }

    // Find the best automatic discount
    double bestAutoRate = max({tieredRate, comboRate, timeRate});

    cout << "+==============================================================================================+\n";
    cout << "|" << leftAlign(" DISCOUNT OPTIONS:", 94) << "|\n";
    cout << "|" << leftAlign(" 1. Apply Best Automatic Discount (" + to_string((int)(bestAutoRate * 100)) + "% off)", 94) << "|\n";
    cout << "|" << leftAlign(" 2. Browse All Available Discounts", 94) << "|\n";
    cout << "|" << leftAlign(" 3. Skip Discount", 94) << "|\n";
    cout << "+==============================================================================================+\n";

    cout << "Enter your choice (1-3): ";
    int choice = getIntInRange("> Your choice: ", 1, 3);

    switch (choice)
    {
    case 1:
    {
        if (bestAutoRate > 0)
        {
            double discountAmount = order.total * bestAutoRate;
            order.total -= discountAmount;
            order.discountAmount = discountAmount;
            order.discountRate = bestAutoRate;

            // Determine discount type and code
            if (bestAutoRate == tieredRate)
            {
                order.discountCode = "AUTO_TIER";
                order.discountType = "automatic_tier";
            }
            else if (bestAutoRate == comboRate)
            {
                order.discountCode = "AUTO_COMBO";
                order.discountType = "automatic_combo";
            }
            else if (bestAutoRate == timeRate)
            {
                order.discountCode = "AUTO_TIME";
                order.discountType = "automatic_time";
            }

            cout << "\nSUCCESS: Applied best automatic discount: " << (int)(bestAutoRate * 100)
                 << "% off (RM " << fixed << setprecision(2) << discountAmount << ")\n";
            cout << "New Total: RM " << fixed << setprecision(2) << order.total << "\n";
        }
        else
        {
            cout << "\nERROR: No automatic discounts available for your order.\n";
        }
        break;
    }
    case 2:
        pushPage(ADVANCED_DISCOUNT, "Advanced Discount System");
        enhancedDiscountSystem(order);
        popPage(); // Remove advanced discount page
        break;
    case 3:
        cout << "\nNo discount applied.\n";
        break;
    }
}

int main()
{
    // Load items from file or initialize default catalog
    loadItems(catalog, "items.txt"); // tries to load from "items.txt"
    if (catalog.empty())
    {
        cout << "Failed to load items.txt. Initializing default catalog.\n";
        initializeCatalog();
    }
    loadCustomers(customers, "customer.txt");
    loadOrderHistory(orderHistory, "orders.txt");
    loadTaxConfig(taxConfig, "tax_config.txt"); // Load tax configuration
    int customerId = 1;

    // Initialize navigation stack with main menu
    clearNavigationStack();
    pushPage(MAIN_MENU, "Main Menu");

    while (true)
    {
        clearScreen();
        logoscreen();
        int choice = display_MainMenu();
        switch (choice)
        {
        case 1:
        {
            Order order;
            OrderEntry(customerId++, order);
            // Reset to main menu after order completion or cancellation
            clearNavigationStack();
            pushPage(MAIN_MENU, "Main Menu");
            break;
        }
        case 2:
        {
            displayOffersAndPromotions();
            break;
        }
        case 3:
            if (staffLogin())
            {
                pushPage(STAFF_MENU, "Staff Menu");
                while (true) // Staff menu loop
                {
                    clearScreen();
                    cout << "\n";
                    cout << "+========================================+\n";
                    cout << "|" << centerText("STAFF MENU", 47) << "|\n";
                    cout << "+========================================+\n";
                    cout << "|                                        |\n";
                    cout << "|   1. Add Item                          |\n";
                    cout << "|                                        |\n";
                    cout << "|   2. View Report                       |\n";
                    cout << "|                                        |\n";
                    cout << "|   3. Tax Configuration                 |\n";
                    cout << "|                                        |\n";
                    cout << "|   4. Return to Main Menu               |\n";
                    cout << "|                                        |\n";
                    cout << "+========================================+\n";
                    cout << "+----------------------------------------+\n";
                    cout << "|" << centerText("Please select an option (1-4):", 40) << "|\n";
                    cout << "+----------------------------------------+\n";
                    int staffChoice = getIntInRange("> Your choice: ", 1, 4);

                    if (staffChoice == 1)
                    {
                        addItem(catalog);
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                    else if (staffChoice == 2)
                    {
                        pushPage(REPORT_VIEW, "Report View");
                        displayOrderReport(orderHistory, catalog);
                        popPage(); // Remove report view page
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                    else if (staffChoice == 3)
                    {
                        pushPage(TAX_CONFIG, "Tax Configuration");
                        configureTaxSettings();
                        popPage(); // Remove tax config page
                        cout << "\nPress Enter to continue...";
                        cin.get();
                    }
                    else if (staffChoice == 4)
                    {
                        cout << "Returning to main menu...\n";
                        popPage(); // Remove staff menu page
                        break;     // Exit the staff menu loop and return to main menu
                    }
                }
                // Reset navigation to main menu after staff menu
                clearNavigationStack();
                pushPage(MAIN_MENU, "Main Menu");
            }
            break;
        case 4:
            cout << "Exiting program.\n";
            saveItems(catalog, "items.txt");
            saveOrderHistory(orderHistory, "orders.txt");
            saveTaxConfig(taxConfig, "tax_config.txt");
            return 0;
        default:
            cout << "Invalid choice, try again.\n";
        }
    }
}