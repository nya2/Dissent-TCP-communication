#ifndef DISSENT_TRANSPORTS_TCP_EDGE_H_GUARD
#define DISSENT_TRANSPORTS_TCP_EDGE_H_GUARD

#include <stdexcept>
#include <QTcpSocket>
#include <QTimer>

#include "Edge.hpp"
#include "TcpAddress.hpp"

namespace Dissent {
namespace Transports {
  /**
   * Used to pass packets
   */
  class TcpEdge : public Edge {
    public:
      /**
       * Constructor
       * @param local the local address of the edge
       * @param remote the address of the remote point of the edge
       * @param incoming true if the remote side requested the creation of this edge
       */
      TcpEdge(const TcpAddress &local, const TcpAddress &remote, bool incoming);
      virtual ~TcpEdge();
      virtual void Send(const QByteArray &data);
      virtual void Close(const QString& reason);
      
      /**
       * TcpEdges just pass memory around, this matches this edge with
       * another edge where it will deliver sent messages
       * @param remote the remote peer which will handle incoming messages
       */

      void SetRemoteEdge(TcpEdge *remote);

      /**
       * TCP Socket
       */
      QTcpSocket *Socket;

     /**
      * local/remote addresses
      */
      const TcpAddress loc;
      const TcpAddress rem;

    private:
      /**
       * On the receiver side, handle an incoming request
       * @param data the data sent from the remote peer
       */
      void Receive(const QByteArray &data);

      /**
       * Start receiving data, called by Receive()
       * @param data the data sent from the remote peer
       */
      void ReceiveData(QByteArray &data);

      /**
       * Retry connection to receive more data
       */
      void requestNewFortune();

      /**
       * The remote edge
       */
      TcpEdge *_remote_edge;

      /**
       * The Edge is closing ... waiting for incoming packets
       */
      bool _closing;

      /**
       * The Remote side is closing...
       */
      bool _rem_closing;

      /**
       * Packets sent but not arrived
       */
      int _incoming;

      /**
       * The reason for closing
       */
      QString _close_reason;

      /**
       * Block size for incoming data
       */
      quint16 blockSize;

      /**
       * Current data
       */
      QString currentFortune;
  };
}
}
#endif
