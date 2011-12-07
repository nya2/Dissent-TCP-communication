#ifndef DISSENT_TCP_TRANSPORT_ADDRESS_H_GUARD
#define DISSENT_TCP_TRANSPORT_ADDRESS_H_GUARD

#include "Address.hpp"
#include "AddressException.hpp"
#include <QHostAddress>

#define DISSENT_PORT 24000

namespace Dissent {
namespace Transports {
  /**
   * Private data holder for TcpAddress
   */
  class TcpAddressData : public AddressData {
    public:
      TcpAddressData(const QUrl &url, const QHostAddress &ip, const int port) : AddressData(url), ip(ip), port(port) { }
      ~TcpAddressData() { }
      virtual bool Equals(const AddressData *other) const;

      const QHostAddress ip;
      const int port;
      
      TcpAddressData(const TcpAddressData &other) : AddressData(other), ip(""), port(DISSENT_PORT)
      {
        throw std::logic_error("Not callable");
      }
                
      TcpAddressData &operator=(const TcpAddressData &)
      {
        throw std::logic_error("Not callable");
      }
  };

  /**
   * A wrapper container for (Tcp)AddressData for Tcp end points
   */
  class TcpAddress : public Address {
    public:
      TcpAddress(const QUrl &url);
      TcpAddress(const QHostAddress &ip);
      TcpAddress(const QHostAddress &ip, const int port);
      TcpAddress(const TcpAddress &other);
      TcpAddress() { } ;
      static const Address CreateAddress(const QUrl &url);

      /**
       * An ip address that uniquely identifies a TcpEdge endpoint
       */
      inline QHostAddress GetIP() const {
        const TcpAddressData *data = GetData<TcpAddressData>();
        if(data == 0) {
          return QHostAddress("0.0.0.0"/*"127.0.0.1"*/);
        } else {
          return data->ip;
        }
      }

      inline int GetPort() const {
        const TcpAddressData *data = GetData<TcpAddressData>();
        if(data == 0) {
          return -1;
        } else {
          return data->port;
        }
      }
  };
}
}

#endif
