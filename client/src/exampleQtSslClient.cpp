#include <QApplication>
#include <client/gui.h>

int main(int argc, char* argv[])
    {
    QApplication app(argc, argv);
	userInterface theUserInterface(NULL);
	theUserInterface.show();
    return app.exec();
    }
