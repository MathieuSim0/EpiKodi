#include <QApplication>
#include <QFile>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configuration de l'application
    app.setApplicationName("EpiKodi");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("EpiKodi");
    
    // Charger le style QSS depuis les ressources
    QFile styleFile(":/styles/dark.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }
    
    // Créer et afficher la fenêtre principale
    MainWindow window;
    window.show();
    
    return app.exec();
}
