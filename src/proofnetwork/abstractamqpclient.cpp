#include "abstractamqpclient.h"
#include "abstractamqpclient_p.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QThread>

using namespace Proof;

static const int AUTO_RECONNECTION_TRIES = 3;

AbstractAmqpClient::AbstractAmqpClient(AbstractAmqpClientPrivate &dd, QObject *parent)
    : ProofObject(dd, parent)
{
    Q_D(AbstractAmqpClient);
    //TODO: Make lazy init for QAmqpClient for correct work in client network thread
    d->m_rabbitClient = new QAmqpClient();

    QObject::connect(d->m_rabbitClient, static_cast<void(QAmqpClient::*)(QAMQP::Error)>(&QAmqpClient::error), this, [this, d](QAMQP::Error error) {
        if (d->m_rabbitClient->autoReconnect() && d->m_autoReconnectionTries) {
            --d->m_autoReconnectionTries;
            qCDebug(proofNetworkAmqpLog) << "Client Connection Error:" << error << "Reconnection tries count:" << d->m_autoReconnectionTries;
            return;
        }

        d->m_rabbitClient->disconnectFromHost();
        emit errorOccurred(NETWORK_MODULE_CODE, NetworkErrorCode::InternalError, QString("Client Error: %1").arg(error), false);
        qCDebug(proofNetworkAmqpLog) << "Client Error:" << error;
    });

    QObject::connect(d->m_rabbitClient, static_cast<void(QAmqpClient::*)(QAbstractSocket::SocketError)>(&QAmqpClient::socketError), this, [this, d](QAbstractSocket::SocketError error) {
        if (d->m_rabbitClient->autoReconnect() && d->m_autoReconnectionTries) {
            --d->m_autoReconnectionTries;
            qCDebug(proofNetworkAmqpLog) << "Client Connection Error:" << error << "Reconnection tries count:" << d->m_autoReconnectionTries;
            return;
        }

        d->m_rabbitClient->disconnectFromHost();
        emit errorOccurred(NETWORK_MODULE_CODE, NetworkErrorCode::ServiceUnavailable, "Can't connect to qamqp server (Socket)", false);
        qCDebug(proofNetworkAmqpLog) << "Socket error" << error;
    });

    QObject::connect(d->m_rabbitClient, &QAmqpClient::sslErrors, this, [this](const QList<QSslError> &errors) {
        emit errorOccurred(NETWORK_MODULE_CODE, NetworkErrorCode::SslError, "Can't connect to qamqp server (SSL)", false);

        QString errorsString;
        for(const auto &error : errors)
            errorsString += QString("%1,\n").arg(error.errorString());
        errorsString.chop(2);
        qCDebug(proofNetworkAmqpLog) << "SSL Socket errors:" << errorsString;
    });

    QObject::connect(d->m_rabbitClient, &QAmqpClient::connected, this, [this, d]() {
        qCDebug(proofNetworkAmqpLog) << "Connected";
        d->m_autoReconnectionTries = AUTO_RECONNECTION_TRIES;

        auto queue = d->m_rabbitClient->createQueue(d->m_queueName);
        if (d->m_queue != queue){
            d->m_queue = queue;
            qCDebug(proofNetworkAmqpLog) << "Create queue:" << d->m_queueName;
            QObject::connect(d->m_queue, static_cast<void(QAmqpQueue::*)(QAMQP::Error)>(&QAmqpQueue::error), this, [this](QAMQP::Error error) {
                emit errorOccurred(NETWORK_MODULE_CODE, NetworkErrorCode::InternalError, QString("Queue Error: %1").arg(error), false);
                qCDebug(proofNetworkAmqpLog) << "Queue Error:" << error;
            });

            QObject::connect(d->m_queue, &QAmqpQueue::opened, this, [this, d]() {
                qCDebug(proofNetworkAmqpLog) << "Queue opened" << sender();
                QObject::connect(d->m_queue, &QAmqpQueue::messageReceived, this, [this, d]() {d->amqpMessageReceived();});
                d->m_queue->consume(QAmqpQueue::coNoAck);
                emit connected();
            });
        }

    });
}

quint16 AbstractAmqpClient::port() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->port();
}

void AbstractAmqpClient::setPort(quint16 port)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setPort(port);
}

QString AbstractAmqpClient::host() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->host();
}

void AbstractAmqpClient::setHost(const QString &host)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setHost(host);
}

QString AbstractAmqpClient::virtualHost() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->virtualHost();
}

void AbstractAmqpClient::setVirtualHost(const QString &virtualHost)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setVirtualHost(virtualHost);
}

QString AbstractAmqpClient::userName() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->username();
}

void AbstractAmqpClient::setUserName(const QString &username)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setUsername(username);
}

QString AbstractAmqpClient::password() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->password();
}

void AbstractAmqpClient::setPassword(const QString &password)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setPassword(password);
}

bool AbstractAmqpClient::autoReconnect() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->autoReconnect();
}

void AbstractAmqpClient::setAutoReconnect(bool value)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setAutoReconnect(value);
}

qint16 AbstractAmqpClient::channelMax() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->channelMax();
}

void AbstractAmqpClient::setChannelMax(qint16 channelMax)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setChannelMax(channelMax);
}

qint32 AbstractAmqpClient::frameMax() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->frameMax();
}

void AbstractAmqpClient::setFrameMax(qint32 frameMax)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setFrameMax(frameMax);
}

qint16 AbstractAmqpClient::heartbeatDelay() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->heartbeatDelay();
}

void AbstractAmqpClient::setHeartbeatDelay(qint16 delay)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setHeartbeatDelay(delay);
}

QSslConfiguration AbstractAmqpClient::sslConfiguration() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->sslConfiguration();
}

void AbstractAmqpClient::setSslConfiguration(const QSslConfiguration &config)
{
    Q_D(AbstractAmqpClient);
    d->m_rabbitClient->setSslConfiguration(config);
}

QString AbstractAmqpClient::queueName() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_queueName;
}

void AbstractAmqpClient::setQueueName(const QString &queueName)
{
    Q_D(AbstractAmqpClient);
    d->m_queueName = queueName;
}

bool AbstractAmqpClient::isConnected() const
{
    Q_D(const AbstractAmqpClient);
    return d->m_rabbitClient->isConnected();
}

void AbstractAmqpClient::connectToHost()
{
    Q_D(AbstractAmqpClient);

    if (!isConnected())
        d->m_rabbitClient->connectToHost();
}

AbstractAmqpClientPrivate::AbstractAmqpClientPrivate() : ProofObjectPrivate()
{

}