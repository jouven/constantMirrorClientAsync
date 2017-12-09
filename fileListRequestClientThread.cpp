#include "fileListRequestClientThread.hpp"

#include "mirrorConfig.hpp"
#include "fileListRequestClientSocket.hpp"

#include "essentialQtso/essentialQt.hpp"

#include <QEventLoop>

fileListRequestClientThread_c::fileListRequestClientThread_c(const QHostAddress &address_par_con
        , const quint16 port_par_con
        , QByteArray * const destinationByteArray_par_con
        , const QString &password_par_con
        , QObject *parent_par) :
    threadedFunction_c(parent_par)
  , address_pri(address_par_con)
  , port_pri(port_par_con)
  , destinationByteArrayRef_pri_con(destinationByteArray_par_con)
  , password_pri_con(password_par_con)
{
}

void fileListRequestClientThread_c::run()
{
#ifdef DEBUGJOUVEN
    //QOUT_TS("fileListRequestClientThread_c::run()" << endl);
#endif
    fileListRequestClientSocket_c* fileListRequestClientSocketTmp = new fileListRequestClientSocket_c(
                address_pri
                , port_pri
                , destinationByteArrayRef_pri_con
                , password_pri_con
    );

    connect(fileListRequestClientSocketTmp, &QTcpSocket::destroyed, this, &QThread::quit);

    exec();
}

