#ifndef CMC_FILELISTREQUESTTHREAD_HPP
#define CMC_FILELISTREQUESTTHREAD_HPP

#include "threadedFunctionQtso/threadedFunctionQt.hpp"

#include <QByteArray>
#include <QHostAddress>

class fileListRequestClientThread_c : public threadedFunction_c
{
    Q_OBJECT

    QHostAddress address_pri;
    quint16 port_pri = 0;

    QByteArray* const destinationByteArrayRef_pri_con;
    const QString password_pri_con;

    void run() override;
public:
    explicit fileListRequestClientThread_c(
            const QHostAddress& address_par_con
            , const quint16 port_par_con
            , QByteArray* const destinationByteArray_par_con = nullptr
            , const QString& password_par_con = QString()
            , QObject * parent_par = nullptr
    );
};

#endif // CMC_FILELISTREQUESTTHREAD_HPP
