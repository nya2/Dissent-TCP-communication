#ifndef DISSENT_CONNECTIONS_CONNECTION_H_GUARD
#define DISSENT_CONNECTIONS_CONNECTION_H_GUARD

#include <ostream>

#include <QDebug>
#include <QScopedPointer>

#include "../Transports/Edge.hpp"
#include "../Messaging/Filter.hpp"

#include "Id.hpp"


namespace Dissent {
namespace Connections {

  namespace {
    using Dissent::Transports::Edge;
    using namespace Dissent::Messaging;
  }

  /**
   * A container class linking a global identifier to a transport layer
   * identifier
   */
  class Connection : public QObject, public Filter {
    Q_OBJECT

    public:
      /**
       * Constructor
       * @param edge the transport layer communication device
       * @param local_id the Id of the local member
       * @param remote_id the Id of the remote member
       */
      Connection(Edge *edge, const Id &local_id, const Id &remote_id);

      virtual QString ToString() const;

      /**
       * The local node wants to remove this connection
       */
      void Disconnect();

      virtual void Send(const QByteArray &data);

      /**
       * Returns the underlying edge
       */
      inline Edge * GetEdge() { return _edge.data(); }

      /**
       * Returns the local id
       */
      inline const Id GetLocalId() const { return _local_id; }

      /**
       * Returns the remote id
       */
      inline const Id GetRemoteId() const { return _remote_id; }

    signals:
      /**
       * Disconnect emits this signal
       */
      void CalledDisconnect(Connection *con);

      /**
       * Once an edge has been closed, this is emitted
       */
      void Disconnected(Connection *con, const QString &reason);

    private:
      /**
       * The transport layer communication device
       */
      QScopedPointer<Edge> _edge;

      /**
       * The Id of the local member
       */
      const Id _local_id;

      /**
       * The Id of the remote member
       */
      const Id _remote_id;

    private slots:
      /**
       * Called when the _edge is closed
       * @param edge should be the same as _edge
       * @param reason the reason why the edge was closed
       */
      void HandleEdgeClose(const Edge *edge, const QString &reason);
  };
}
}

#endif
