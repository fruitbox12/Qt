#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <Qlist>
#include <QByteArray>

class Network : public QObject {
	Q_OBJECT

	private:
		QTcpServer*			server;
		QList<QByteArray>	data;

	private slots:
		void sendData();

	public:
		Network(QObject *parent = nullptr);
		virtual ~Network();
};

