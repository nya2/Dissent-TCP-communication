#include "TcpEdgeListener.hpp"
#include <QDebug>

namespace Dissent {
namespace Transports {
  QHash<QString, TcpEdgeListener *> TcpEdgeListener::_el_map;

  TcpEdgeListener::TcpEdgeListener(const TcpAddress &local_address) :
    EdgeListener(local_address)
  {
    /* FIXME: Allow same host with different port */
    QString key = local_address.GetIP().toString() + ":" + QString::number(local_address.GetPort());
    if(_el_map.contains(key)) {
      qWarning() << "Attempting to create two TcpEdgeListeners with the same" <<
        " address: " << local_address.ToString() << ":" << QString::number(local_address.GetPort());
      return;
    }
    _el_map[key] = this;
  }

  TcpEdgeListener::~TcpEdgeListener()
  {
    const TcpAddress &loc_ta = static_cast<const TcpAddress &>(_local_address);
    _el_map.remove(loc_ta.GetIP().toString() + ":" + QString::number(loc_ta.GetPort()));
  }

  EdgeListener *TcpEdgeListener::Create(const Address &local_address)
  {
    const TcpAddress &ta = static_cast<const TcpAddress &>(local_address);
    return new TcpEdgeListener(ta);
  }

  void TcpEdgeListener::CreateEdgeTo(const Address &to)
  {
    const TcpAddress &rem_ta = static_cast<const TcpAddress &>(to);
    TcpEdgeListener *remote_el = _el_map[rem_ta.GetIP().toString() + ":" + QString::number(rem_ta.GetPort())];
    if(remote_el == 0) {
      qDebug() << "Attempting to create an Edge to an EL that doesn't exist from " <<
        _local_address.ToString() << " to " << to.ToString();
      return;
    }

    TcpEdge *local_edge(new TcpEdge(static_cast<const TcpAddress &>(_local_address), static_cast<const TcpAddress &>(remote_el->_local_address), true));
    TcpEdge *remote_edge(new TcpEdge(static_cast<const TcpAddress &>(remote_el->_local_address), static_cast<const TcpAddress &>(_local_address), false));

    local_edge->SetRemoteEdge(remote_edge);
    remote_edge->SetRemoteEdge(local_edge);

    ProcessNewEdge(local_edge);
    remote_el->ProcessNewEdge(remote_edge);
  }
/*
  void TcpEdgeListener::Start()
  {
    const TcpAddress &loc_ta = static_cast<const TcpAddress &>(_local_address);
    server.listen(loc_ta.GetIP(), loc_ta.GetPort());
  }

  void TcpEdgeListener::Stop()
  {
    server.close();
  }
*/
}
}
