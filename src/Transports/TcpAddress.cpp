#include "TcpAddress.hpp"

namespace Dissent {
namespace Transports {
  TcpAddress::TcpAddress(const QUrl &url)
  {
    int portnum = url.port();
    if(portnum < 1) {
      portnum = DISSENT_PORT;
    }

    _data = new TcpAddressData(url, QHostAddress(url.host()), portnum);
  }

  TcpAddress::TcpAddress(const QHostAddress &ip)
  {
    QUrl url("dissent://" + ip.toString());
    url.setPort(DISSENT_PORT);

    _data = new TcpAddressData(url, ip, DISSENT_PORT);
  }

  TcpAddress::TcpAddress(const QHostAddress &ip, const int portnum)
  {
    QUrl url("dissent://" + ip.toString());
    int port = portnum;
    if(port < 1) {
      port = DISSENT_PORT;
    }

    url.setPort(port);

    _data = new TcpAddressData(url, ip, port);
  }

  TcpAddress::TcpAddress(const TcpAddress &other) : Address(other)
  {
  }

  const Address TcpAddress::CreateAddress(const QUrl &url)
  {
    return TcpAddress(url);
  }

  bool TcpAddressData::Equals(const AddressData *other) const
  {
    const TcpAddressData *bother = dynamic_cast<const TcpAddressData *>(other);
    if(bother) {
      return ip == bother->ip;
    } else {
      return AddressData::Equals(other);
    }
    return false;
  }
}
}
