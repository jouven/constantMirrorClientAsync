#ifndef CMC_FILELISTREQUESTCLIENTSOCKET_HPP
#define CMC_FILELISTREQUESTCLIENTSOCKET_HPP

#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QSslSocket>

class fileListRequestClientSocket_c : public QSslSocket
{
    Q_OBJECT

    //if it's passed in the ctor the pointer is saved here
    QByteArray* const destinationByteArrayRef_pri_con;
    const QString password_pri_con;

public:
    explicit fileListRequestClientSocket_c(const QHostAddress& address_par_con
            , const quint16 port_par_con
            , QByteArray * const destinationByteArrayRef_par_con = nullptr
            , const QString& password_par_con = QString()
            , QObject * parent_par = nullptr
    );
private Q_SLOTS:
    void readyRead_f();
    //void disconnected_f();
    void connected_f();
};

#endif // CMC_FILELISTREQUESTCLIENTSOCKET_HPP
