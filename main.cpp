#include <QCoreApplication>
#include <QCommandLineParser>

#include "updatecontrol.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);


    QCoreApplication::setApplicationName("UpdateServer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Update server for ESHF device");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption workingDirectory(QStringList() << "d" << "directory", QCoreApplication::translate("Working directory", "set directory "));
    parser.addOption(workingDirectory);
    parser.process(app);

    UpdateControl ctrl;

    QString dir = "";
    if (parser.isSet(workingDirectory)) {
        dir = QString::fromStdString(parser.value(workingDirectory).toStdString());
        QDir directory = QDir(dir);
        if (directory.exists())
        {
            ctrl.setDirectory(dir);
        }
    }


    return app.exec();
}
