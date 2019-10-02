#include <QApplication>
#include "frontend/MainWindow.hpp"
#include "spdlog/spdlog.h"

int main (int argc, char *argv[]) {
    spdlog::info("Initializing SotoShop...");
    spdlog::info("Log level set to {}", "debug");
    spdlog::set_level(spdlog::level::debug);
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