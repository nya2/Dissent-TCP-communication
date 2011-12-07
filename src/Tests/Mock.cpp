#include "Mock.hpp"

namespace Dissent {
namespace Tests {
  void MockSource::IncomingData(const QByteArray& data, ISender *from)
  {
    PushData(data, from);
  }

  MockSender::MockSender(MockSource *source) : _source(source)
  {
  }

  void MockSender::Send(const QByteArray& data)
  {
    _source->IncomingData(data, _from);
  }

  void MockSender::SetReturnPath(ISender *sender)
  {
    _from = sender;
  }

  void MockSink::HandleData(const QByteArray& data, ISender *from)
  {
    _last_sender = from;
    _last_data = data;
  }

  const QByteArray MockSink::GetLastData()
  {
    return _last_data;
  }

  ISender *MockSink::GetLastSender()
  {
    return _last_sender;
  }

  MockEdgeHandler::MockEdgeHandler(EdgeListener *el)
  {
    QObject::connect(el, SIGNAL(NewEdgeSignal(Edge *)),
        this, SLOT(HandleEdge(Edge *)));
  }

  void MockEdgeHandler::HandleEdge(Edge *edge)
  {
    this->edge.reset(edge);
  }

  void MockExec()
  {
    QCoreApplication::processEvents();
    QCoreApplication::sendPostedEvents(0, 0);
  }
}
}
