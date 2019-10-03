#include <QApplication>
#include <string>
#include "frontend/MainWindow.hpp"
#include "spdlog/spdlog.h"

int main (int argc, char *argv[]) {
    spdlog::info("Initializing SotoShop...");
    spdlog::level::level_enum log_level;
    std::string log_level_arg;
    if (argc >= 2) {
        log_level_arg = argv[1];
        if (log_level_arg == "debug") {
            log_level = spdlog::level::debug;
        } else if (log_level_arg == "info") {
            log_level = spdlog::level::info;
        } else if (log_level_arg == "warn") {
            log_level = spdlog::level::warn;
        } else if (log_level_arg == "error") {
            log_level = spdlog::level::err;
        } else {
            spdlog::warn("Invalid log level parameter, setting log level to info...");
            log_level = spdlog::level::info;
            log_level_arg = "info";
        }
    } else {
        log_level = spdlog::level::info;
        log_level_arg = "info";
    }
    spdlog::info("Log level set to {}", log_level_arg);
    spdlog::set_level(log_level);
    spdlog::debug("Creating the main window...");
    QApplication app(argc, argv);
    MainWindow rootWindow;
    rootWindow.setWindowTitle("SotoShop");
    rootWindow.setFixedSize(800,600);
    rootWindow.show();

    /* STUB IMAGE DATA */
    spdlog::debug("Creating stub image...");
    Image * image = new Image(500, 400);
    rootWindow.setActiveImage(image);

    return app.exec();
}