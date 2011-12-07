#include "TcpEdge.hpp"

namespace Dissent {
namespace Transports {
  TcpEdge::TcpEdge(const TcpAddress &local, const TcpAddress &remote, bool incoming) :
    Edge(local, remote, incoming), loc(local), rem(remote), _remote_edge(0),
    _closing(false), _rem_closing(false), _incoming(0)
  {
    Socket = new QTcpSocket(this);
  }

  TcpEdge::~TcpEdge()
  {
    _remote_edge = 0;
  }

  void TcpEdge::SetRemoteEdge(TcpEdge *remote_edge)
  {
    if(!_remote_edge == 0) {
      qWarning() << "TcpEdge's remote already set.";
      return;
    }
    _remote_edge = remote_edge;
  }

  void TcpEdge::Send(const QByteArray &data)
  {
    /*if(!_incoming) {
      return;
    }*/

    if(_closing) {
      qWarning() << "Attempted to send on a closed edge.";
      return;
    }

    if(_rem_closing) {
      return;
    }

    /* TODO: Check for connection errors */
    Socket->connectToHost(rem.GetIP(), rem.GetPort());
    /* waitForConnected waits for 30 seconds and then times out
       Pass -1 to make the function non-blocking */
    if(Socket->waitForConnected()) {
      Socket->write(data);
    }

    _remote_edge->_incoming++;
  }

  void TcpEdge::Close(const QString& reason)
  {
    if(_closing) {
      qWarning() << "TcpEdge already closed.";
      return;
    }

    qDebug() << "Calling Close on " << ToString() << " with " << _incoming << " remaining messages.";
    _closing = true;
    if(!_rem_closing) {
      _remote_edge->_rem_closing = true;
    }
    _close_reason = reason;

    if(_incoming == 0) {
      Socket->close();
      Edge::Close(_close_reason);
    }
  }

  void TcpEdge::Receive(const QByteArray &data)
  {
    _incoming--;
    if(_closing) {
      if(_incoming == 0) {
        qDebug() << "No more messages on calling Edge::Close";
	Socket->close();
        Edge::Close(_close_reason);
      }
      return;
    }

    /*if(_incoming) {
	return;
    }*/
    
    //data.clear();

    /* TODO: Check for connection errors */
    Socket->connectToHost(rem.GetIP(), rem.GetPort());
    connect(Socket, SIGNAL(readyRead()), this, SLOT(ReceiveData(data)));
    
    PushData(data, this);
  }

  void TcpEdge::ReceiveData(QByteArray &data)
  {
    QDataStream in(Socket);
    in.setVersion(QDataStream::Qt_4_0);

    if(blockSize == 0) {
      if(Socket->bytesAvailable() < (int)sizeof(quint16))
        return;

      in >> blockSize;
    }

    if(Socket->bytesAvailable() < blockSize)
      return;

    QString nextFortune;
    in >> nextFortune;

    data.append(nextFortune);

    if(nextFortune == currentFortune) {
      QTimer::singleShot(0, this, SLOT(requestNewFortune()));
      return;
    }

    currentFortune = nextFortune;
  }

  void TcpEdge::requestNewFortune()
  {
    blockSize = 0;
    Socket->abort();
    Socket->connectToHost(rem.GetIP(), rem.GetPort());
  }
}
}
