#include <QCoreApplication>
#include <QDateTime>
#include <QSocketNotifier>
#include <QTimer>

#include <iostream>
#include <vector>

#include "socket.h"

using namespace std;

vector<string> logs;

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void addLog(const string &type, const string &message)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");

    logs.push_back("[" + time.toStdString() + "] " + type + " " + message);

    if (logs.size() > 8)
        logs.erase(logs.begin());
}

void drawHeader()
{
    cout << "\033[36m";

    cout << R"(

 ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗
 ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗
 ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝
 ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗
 ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║
 ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝

)";

    cout << "\033[0m";
}

void drawDashboard(int clients)
{
    clearConsole();

    drawHeader();

    cout << "\033[32m";

    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║            QT WEBSOCKET SERVER              ║\n";
    cout << "╠══════════════════════════════════════════════╣\n";
    cout << "║ Status    : RUNNING                         ║\n";
    cout << "║ Port      : 12345                           ║\n";

    cout << "║ Clients   : " << clients;

    if (clients < 10)
        cout << "                                 ║\n";
    else
        cout << "                                ║\n";

    cout << "║ SSL/WSS   : ENABLED                         ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    cout << "\033[0m";

    cout << "\n";

    cout << "\033[33m";

    cout << " Commands\n";
    cout << " ───────────────────────────────────────────\n";
    cout << "  h  → Help Menu\n";
    cout << "  c  → Show Client Count\n";
    cout << "  l  → Show Logs\n";
    cout << "  q  → Quit Server\n";

    cout << "\033[0m";

    cout << "\n";

    cout << "\033[35m";

    cout << " Logs\n";
    cout << " ───────────────────────────────────────────\n";

    for (const auto &log : logs) {
        cout << " " << log << endl;
    }

    cout << "\033[0m";

    cout << "\n> ";
    cout.flush();
}

void printHelp()
{
    cout << "\033[36m";

    cout << R"(

╔════════════════════════════════════════════╗
║                 HELP MENU                 ║
╠════════════════════════════════════════════╣
║  h  → Open help menu                      ║
║  c  → Print connected clients             ║
║  l  → Print server logs                   ║
║  q  → Shutdown server                     ║
╚════════════════════════════════════════════╝

)";

    cout << "\033[0m";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Socket server;

    int clientCount = 0;

    addLog("[INFO]", "Server started successfully");
    addLog("[INFO]", "Listening on port 12345");

    drawDashboard(clientCount);

    QTimer dashboardTimer;

    QObject::connect(&dashboardTimer, &QTimer::timeout, [&]() { drawDashboard(clientCount); });

    dashboardTimer.start(3000);

    QSocketNotifier *notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read);

    QObject::connect(notifier, &QSocketNotifier::activated, [&](int) {
        string input;
        getline(cin, input);

        if (input == "h") {
            printHelp();
            addLog("[CMD]", "Opened help menu");
        } else if (input == "c") {
            cout << "\nConnected Clients : " << clientCount << endl;

            addLog("[CMD]", "Checked client count");
        } else if (input == "l") {
            cout << "\nServer Logs:\n";

            for (const auto &log : logs)
                cout << log << endl;
        } else if (input == "q") {
            addLog("[SYSTEM]", "Server stopped");

            cout << "\033[31m";
            cout << "\nStopping server...\n";
            cout << "\033[0m";

            a.quit();
        } else {
            addLog("[ERROR]", "Unknown command: " + input);
        }
    });

    return a.exec();
}
