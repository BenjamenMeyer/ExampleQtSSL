#include <QApplication>
#include <client/gui.h>

int main(int argc, char* argv[])
    {
    QApplication app(argc, argv);
	exampleQtSsl::client::gui::userInterface theUserInterface(NULL);
	theUserInterface.show();
    return app.exec();
    }
