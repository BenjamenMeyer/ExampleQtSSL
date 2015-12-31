#ifndef QT_SSL_EXAMPLE_CLIENT_GUI_H__
#define QT_SSL_EXAMPLE_CLIENT_GUI_H__

#include <QWidget>
#include <QPointer>
#include <QPushButton>
#include <QTextEdit>
#include <QThread>

#include <log/logger.h>
#include <client/manager.h>

namespace exampleQtSsl
    {
    namespace client
        {
		namespace gui
			{
			class userInterface : public QWidget
				{
				Q_OBJECT
				public:
					userInterface(QWidget *parent = NULL, Qt::WindowFlags _flags=0);
					virtual ~userInterface();

				public Q_SLOTS:
                    void startService();
                    void stopService();
					void logMessage(QString _message);

				Q_SIGNALS:
                    void start_service();
                    void stop_service();
                    void send_to_log(QString _message);

				protected:
                    QPointer<QTextEdit> textDisplay;
                    QPointer<QPushButton> buttonStart;
                    QPointer<QPushButton> buttonStop;

                    // thread that runs the file logger
                    QThread loggingThread;
                    exampleQtSsl::common::logger::logger log;

					QThread serviceThread;
					exampleQtSsl::client::network::manager manager;

                    void createLayout();
				};
			}
        }
    }

#endif // QT_SSL_EXAMPLE_CLIENT_GUI_H__
