#include <QApplication>
#include <QByteArray>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFile>
#include <QList>

#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslCertificate>

#include <server/gui.h>

#include <cstdio>

#define ADD_COMMAND_LINE_OPTION(name, description, defaultValue, parser) \
	{										\
	QCommandLineOption option(name);		\
	option.setDescription(description);		\
	option.setDefaultValue(defaultValue);	\
	option.setValueName(name);				\
	parser.addOption(option);				\
	}

#define OPTION_SSL_KEY			"ssl-key"
#define OPTION_SSL_CERTIFICATE	"ssl-certificate" 

#define EXIT_CODE_NO_CERTIFICATE_FILE				(-1)
#define EXIT_CODE_UNABLE_TO_OPEN_CERTIFICATE_FILE	(-2)
#define EXIT_CODE_NO_KEY_FILE						(-3)

int main(int argc, char* argv[])
    {
    QApplication app(argc, argv);

	QCommandLineParser argumentParser;
	argumentParser.setApplicationDescription("Example SSL Server Application");
	argumentParser.addHelpOption();
	ADD_COMMAND_LINE_OPTION(OPTION_SSL_KEY, "SSL Private Key File (PEM format)", ".ssl-key", argumentParser);
	ADD_COMMAND_LINE_OPTION(OPTION_SSL_CERTIFICATE, "SSL Certificate File (PEM format)", ".ssl-certificate", argumentParser);
	argumentParser.process(app);

	QString sslKeyFileName = argumentParser.value(OPTION_SSL_KEY);
	QString sslCertificateFileName = argumentParser.value(OPTION_SSL_CERTIFICATE);

	fprintf(stderr, "SSL Key File: %s\n", sslKeyFileName.toLatin1().data());
	fprintf(stderr, "SSL Certificate File: %s\n", sslKeyFileName.toLatin1().data());

	// load the configuration
	QFile sslKeyFile(sslKeyFileName);
	if (sslKeyFile.fileName() != sslKeyFileName)
		{
		fprintf(stderr, "SSL key File Name does not match");
		}
	if (sslKeyFile.exists() == false)
		{
		fprintf(stderr, "SSL Key File %s does not exist\n", sslKeyFile.fileName().toLatin1().data());
		exit(EXIT_CODE_NO_KEY_FILE);
		}

	QSslKey sslKey(&sslKeyFile);

	QFile sslCertificateFile(sslCertificateFileName);
	if (sslCertificateFile.exists())
		{
		if (sslCertificateFile.open(QIODevice::ReadOnly) == true)
			{
			fprintf(stderr, "Using SSL Certificate File %s\n", sslCertificateFileName.toLatin1().data());
			}
		else
			{
			fprintf(stderr, "Unable to open SSL Certificate File %s\n", sslCertificateFileName.toLatin1().data());
			exit(EXIT_CODE_UNABLE_TO_OPEN_CERTIFICATE_FILE);
			}
		}
	else
		{
		fprintf(stderr, "SSL Certificate File %s does not exist\n", sslCertificateFileName.toLatin1().data());
		exit(EXIT_CODE_NO_CERTIFICATE_FILE);
		}

	QByteArray certificateData = sslCertificateFile.readAll();
	QSslCertificate sslCertificate(certificateData);
    userInterface theUserInterface(sslKey, sslCertificate, NULL);
    theUserInterface.show();
    return app.exec();
    }
