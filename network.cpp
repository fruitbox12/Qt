#include "network.h"

Network::Network(QObject *parent) : QObject(parent) {
	// 1. Create a TCP socket: set up the server  and make it listen for new connections on server ip address and TCP port
	server = new QTcpServer(this);

	// 2. Bind: associate the socket with your adapter's IP address and a TCP port number
	// Pick a port number from 29152 through 65535. IANA publishes a list of currently assigned ports
	// For localhost can use QHostAddress:LocalHost(127.0.0.1)
	// QHostAddress ipAddress("127.0.0.1");
	QHostAddress ipAddress("192.168.1.59");
	
	if (!(server->listen(ipAddress, 56789))) {
		// qFatal() never returns - it crashes the program immediantely
		qFatal("ERROR: Failed to bind TCP server to port 56789 on 192.168.1.59");
	}

	// Most OS's are event driven, they wait for certain events to occur and auto run special functions/methods.
	// Programmers "schedule" event handlers. In Qt, event are called SIGNALS, even handler functions are called SLOTS.
	// Connect the socket object's client connection request event to the sendData() even handler.
	// When the client sends a connect() request (called newConnection() it Qt), auto. run the event handler sendData().
	connect(server, SIGNAL(newConnection()), this, SLOT(sendData()));

	data.append(QByteArray("You've been leading a dog's life. Stay off the furniture. \n"));
	data.append(QByteArray("You have to think about tomorrow. \n"));
	data.append(QByteArray("You will be surprised by a loud noise. \n"));
	data.append(QByteArray("You will feel hungry again in another hour. \n"));
	data.append(QByteArray("My ears my ears. \n"));
	data.append(QByteArray("You ugly. \n"));
	data.append(QByteArray("Water is not wet. \n"));
}


Network::~Network() {
	// Shut down the server first
	server->close();

	// Disconnect all signals and slots connected to this server
	server->disconnect();
	
	// Queue this object for deletion at the first opportune moment.
	// Dont delete any QObject-derived object the standard C++ way because there might be pending signals
	// that must be processed by the object. If we delete the object and Qt tries to deliver a signal to it,
	// the program will segfault and crash. We call the deleteLater() method (which is actually defined as a slot,
	// and we will use that property in the next function we define),
	// and this makes sure there are no pending tasks for the object before pulling the plug on it.
	server->deleteLater();
}

void Network::sendData() {
	// Take a client socket off of the server
	QTcpSocket * clientSocket = server->nextPendingConnection();

	// Wait until the client socket is connected
	if (!(clientSocket->waitForConnected())) {
		// qDebug() works just like std::cout, except that it automatically inserts a new line at the end
		// of every statement
		// qDebug is not a stream, but a stream factory.
		qDebug() << clientSocket->errorString();
		return;
	}

	// Choose a random fortune and send it to the receiver
	clientSocket->write(data.at(qrand() % data.size()));

	// Tear down the host connection. Call disconnectFromHost(), not disconnect(), because disconnect() 
	// on any QObject derived class (which is almost all the classes in Qt) discconects all signals and slots
	// connected to the object. Finally, we connect the disconnected() signal (which is emitted when the socket
	// has finally disconnected) to the deleteLater() slot on the same socket. So now, when the socket disconects, 
	// it will be queued for deletion. This is the standard way of tearing down a socket.
	clientSocket->disconnectFromHost();
	connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
}
