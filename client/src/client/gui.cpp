#include <client/gui.h>

#include <QHBoxLayout>
#include <QVBoxLayout>

userInterface::userInterface(QWidget* _parent, Qt::WindowFlags _flags): QWidget(_parent, _flags), log(QString("exampleQtSslClient"), NULL)
	{
	createLayout();

    connect(this, SIGNAL(send_to_log(QString)), &log, SLOT(logMessage(QString)));
    log.moveToThread(&loggingThread);
    loggingThread.start();

	connect(this, SIGNAL(start_service()), &theManager, SLOT(start_control()));
	connect(this, SIGNAL(stop_service()), &theManager, SLOT(stop_control()));
	connect(&theManager, SIGNAL(logMessage(QString)), this, SLOT(logMessage(QString)));
	theManager.moveToThread(&serviceThread);
	serviceThread.start();
	}
userInterface::~userInterface()
	{
	serviceThread.quit();
	loggingThread.quit();

	serviceThread.wait();
	loggingThread.wait();
	}

void userInterface::createLayout()
    {
    QHBoxLayout* masterLayout = new QHBoxLayout();
    if (masterLayout != NULL)
        {
        QVBoxLayout* commandLayout = new QVBoxLayout();
        if (commandLayout != NULL)
            {
            if (buttonStart.isNull() == true)
                {
                buttonStart = new QPushButton(tr("Start"));
                }
            if (buttonStart.isNull() == false)
                {
                connect(buttonStart,SIGNAL(clicked()), this, SLOT(startService()));
                commandLayout->addWidget(buttonStart);
                }
            if (buttonStop.isNull() == true)
                {
                buttonStop = new QPushButton(tr("Stop"));
                }
            if (buttonStop.isNull() == false)
                {
                buttonStop->setEnabled(false);
                connect(buttonStop,SIGNAL(clicked()), this, SLOT(stopService()));
                commandLayout->addWidget(buttonStop);
                }

            commandLayout->addStretch();
            masterLayout->addLayout(commandLayout);
            }

        if (textDisplay.isNull() == true)
            {
            textDisplay = new QTextEdit();
            }
        if (textDisplay.isNull() == false)
            {
            textDisplay->setReadOnly(true);
            textDisplay->document()->setMaximumBlockCount(500);
            masterLayout->addWidget(textDisplay);
            }

        setLayout(masterLayout);
        }
    }

void userInterface::startService()
    {
    buttonStart->setEnabled(false);
    buttonStop->setEnabled(true);
    if (buttonStart->hasFocus() == true)
        {
        buttonStop->setFocus(Qt::OtherFocusReason);
        }
    Q_EMIT start_service();
    }
void userInterface::stopService()
    {
    buttonStop->setEnabled(false);
    buttonStart->setEnabled(true);
    if (buttonStop->hasFocus() == true)
        {
        buttonStart->setFocus(Qt::OtherFocusReason);
        }
    Q_EMIT stop_service();
    }

void userInterface::logMessage(QString _message)
    {
    if (textDisplay.isNull() == false)
        {
        textDisplay->append(_message);
        }
    Q_EMIT send_to_log(_message);
    }
