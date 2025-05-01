//
// Created by Cameron on 4/26/25.
//

#ifndef LOGGER_H
#define LOGGER_H

#ifdef _WIN32
inline auto NULL_DEVICE = "NUL";
#else
inline auto NULL_DEVICE = "/dev/null";
#endif

#include <iosfwd>
#include <fstream>
#include <cstring>

// Under-complicated logging class
class Logger {
    std::ofstream logFile;
public:
    enum Levels {
        INFO,
        WARNING,
        ERROR
    };

    // Open a file by a string file location. Opens a Win/Unix null device by default
    void openFile(const char* fileName=NULL_DEVICE) {
        logFile.open(fileName);

        if (!logFile.is_open()) {
            std::cerr << "Failed to open file " << fileName << ". Check file permissions." << std::endl;
            exit(1);
        }
    }

    // The main reason that this is a header-only implementation
    template <typename... Args>
    void log(const Levels level, Args... messages) {
        const time_t now = time(nullptr);
        char * dt = ctime(&now);
        // Get rid of pesky newline char
        dt[strlen(dt) - 1] = '\0';

        logFile << "[" << dt << "]-["
            << (level==INFO ? "INFO" : level==WARNING ? "WARNING" : "ERROR")
            << "]: ";

        (logFile << ... << messages) << std::endl;
    }

    template <typename... Args>
    void log(Args... messages) {
        log(INFO, messages...);
    }
};


#endif //LOGGER_H
