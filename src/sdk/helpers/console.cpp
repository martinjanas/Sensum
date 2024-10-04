#include "Console.h"

void Console::detach()
{
    save_log();
    Sleep(1000);

    HWND hwnd = GetConsoleWindow();

    FreeConsole();
    PostMessageA(hwnd, WM_QUIT, 0, 0);
}

void Console::attach()
{
    AllocConsole();
    AttachConsole(ATTACH_PARENT_PROCESS);

    FILE* file;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);
}

void Console::log_output(const char* msg)
{
    std::stringstream ss;
    ss << msg << "\n";
    log_data.push_back(ss.str());
}

void Console::save_log()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm now_tm;
    localtime_s(&now_tm, &now_time);

    std::filesystem::path logDirectory = "C:\\Sensum\\logs";

    std::ostringstream folderName;
    folderName << std::put_time(&now_tm, "%Y-%m-%d");

    std::filesystem::path folderPath = logDirectory / folderName.str();
    std::filesystem::create_directories(folderPath);

    int logCount = 1;
    std::filesystem::path logFilePath;

    while (true)
    {
        logFilePath = folderPath / (folderName.str() + "-" + std::to_string(logCount) + ".log");
        if (!std::filesystem::exists(logFilePath))
            break;

        logCount++;
    }

    std::ofstream outFile(logFilePath, std::ios::app);
    if (!outFile)
        return;

    for (const auto& x : log_data)
    {
        outFile << x;
    }

    outFile.close();
}
