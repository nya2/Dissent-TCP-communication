#ifndef DISSENT_TRANSPORTS_TCP_EDGE_LISTENER_H_GUARD
#define DISSENT_TRANSPORTS_TCP_EDGE_LISTENER_H_GUARD

#include <QHash>

#include "TcpAddress.hpp"
#include "TcpEdge.hpp"
#include "EdgeListener.hpp"

namespace Dissent {
namespace Transports {
  /**
   * Creates edges which can be used to pass packets
   */
  class TcpEdgeListener : public EdgeListener {
    public:
      TcpEdgeListener(const TcpAddress &local_addr);
      static EdgeListener *Create(const Address &local_address);
      ~TcpEdgeListener();
      virtual void CreateEdgeTo(const Address &to);
      /*virtual void Start();
      virtual void Stop();*/
    private:
      static QHash<QString, TcpEdgeListener *> _el_map;
  };
}
}

#endif
