#include "DissentTest.hpp"
#include <QDebug>

using namespace Dissent::Messaging;
using namespace Dissent::Transports;
using namespace Dissent::Connections;
using namespace Dissent::Utils;

namespace Dissent {
namespace Tests {
  TEST(Connection, OneWay)
  {
    Timer::GetInstance().UseVirtualTime();

    const BufferAddress addr0(1000);
    BufferEdgeListener *be0 = new BufferEdgeListener(addr0);
    RpcHandler rpc0;
    Id id0;
    ConnectionManager cm0(id0, &rpc0);
    cm0.AddEdgeListener(be0);

    const BufferAddress addr1(10001);
    BufferEdgeListener *be1 = new BufferEdgeListener(addr1);
    RpcHandler rpc1;
    Id id1;
    ConnectionManager cm1(id1, &rpc1);
    cm1.AddEdgeListener(be1);

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
    cm1.ConnectTo(addr0);

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    TestRpc test0;
    rpc0.Register(new RpcMethod<TestRpc>(test0, &TestRpc::Add), "add");

    TestRpcResponse test1;
    RpcMethod<TestRpcResponse> cb = RpcMethod<TestRpcResponse>(test1, &TestRpcResponse::HandleResponse);

    QVariantMap request;
    request["method"] = "add";
    request["x"] = 3;
    request["y"] = 6;

    EXPECT_EQ(-1, test1.value);
    rpc1.SendRequest(request, cm1.GetConnectionTable().GetConnection(id0), &cb);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_EQ(9, test1.value);

    cm1.GetConnectionTable().GetConnection(id0)->Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
  }

  TEST(Connection, Bidirectional)
  {
    Timer::GetInstance().UseVirtualTime();

    const BufferAddress addr0(1000);
    BufferEdgeListener *be0 = new BufferEdgeListener(addr0);
    RpcHandler rpc0;
    Id id0;
    ConnectionManager cm0(id0, &rpc0);
    cm0.AddEdgeListener(be0);

    const BufferAddress addr1(10001);
    BufferEdgeListener *be1 = new BufferEdgeListener(addr1);
    RpcHandler rpc1;
    Id id1;
    ConnectionManager cm1(id1, &rpc1);
    cm1.AddEdgeListener(be1);

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    cm1.ConnectTo(addr0);

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    cm0.ConnectTo(addr1);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_TRUE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    TestRpc test0;
    rpc0.Register(new RpcMethod<TestRpc>(test0, &TestRpc::Add), "add");

    TestRpcResponse test1;
    RpcMethod<TestRpcResponse> cb = RpcMethod<TestRpcResponse>(test1, &TestRpcResponse::HandleResponse);

    QVariantMap request;
    request["method"] = "add";
    request["x"] = 3;
    request["y"] = 6;

    EXPECT_EQ(-1, test1.value);
    rpc1.SendRequest(request, cm1.GetConnectionTable().GetConnection(id0), &cb);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_EQ(9, test1.value);

    cm1.GetConnectionTable().GetConnection(id0)->Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_TRUE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    cm0.GetConnectionTable().GetConnection(id1)->Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
  }

  TEST(Connection, SameTime)
  {
    Timer::GetInstance().UseVirtualTime();

    const BufferAddress addr0(1000);
    BufferEdgeListener *be0 = new BufferEdgeListener(addr0);
    RpcHandler rpc0;
    Id id0;
    ConnectionManager cm0(id0, &rpc0);
    cm0.AddEdgeListener(be0);

    const BufferAddress addr1(10001);
    BufferEdgeListener *be1 = new BufferEdgeListener(addr1);
    RpcHandler rpc1;
    Id id1;
    ConnectionManager cm1(id1, &rpc1);
    cm1.AddEdgeListener(be1);

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
    cm1.ConnectTo(addr0);
    cm0.ConnectTo(addr1);

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_TRUE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    TestRpc test0;
    rpc0.Register(new RpcMethod<TestRpc>(test0, &TestRpc::Add), "add");

    TestRpcResponse test1;
    RpcMethod<TestRpcResponse> cb = RpcMethod<TestRpcResponse>(test1, &TestRpcResponse::HandleResponse);

    QVariantMap request;
    request["method"] = "add";
    request["x"] = 3;
    request["y"] = 6;

    EXPECT_EQ(-1, test1.value);
    rpc1.SendRequest(request, cm1.GetConnectionTable().GetConnection(id0), &cb);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_EQ(9, test1.value);

    cm1.GetConnectionTable().GetConnection(id0)->Disconnect();
    cm0.GetConnectionTable().GetConnection(id1)->Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
  }

  TEST(Connection, Disconnect)
  {
    Timer::GetInstance().UseVirtualTime();

    const BufferAddress addr0(1000);
    BufferEdgeListener *be0 = new BufferEdgeListener(addr0);
    RpcHandler rpc0;
    Id id0;
    ConnectionManager cm0(id0, &rpc0);
    cm0.AddEdgeListener(be0);

    const BufferAddress addr1(10001);
    BufferEdgeListener *be1 = new BufferEdgeListener(addr1);
    RpcHandler rpc1;
    Id id1;
    ConnectionManager cm1(id1, &rpc1);
    cm1.AddEdgeListener(be1);

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
    cm1.ConnectTo(addr0);
    cm0.ConnectTo(addr1);

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_TRUE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    cm1.Disconnect();
    cm0.Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    DisableLogging();

    cm1.ConnectTo(addr0);
    cm0.ConnectTo(addr1);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    EnableLogging();
  }

  TEST(Connection, OneWayDisconnect)
  {
    Timer::GetInstance().UseVirtualTime();

    const BufferAddress addr0(1000);
    BufferEdgeListener *be0 = new BufferEdgeListener(addr0);
    RpcHandler rpc0;
    Id id0;
    ConnectionManager cm0(id0, &rpc0);
    cm0.AddEdgeListener(be0);

    const BufferAddress addr1(10001);
    BufferEdgeListener *be1 = new BufferEdgeListener(addr1);
    RpcHandler rpc1;
    Id id1;
    ConnectionManager cm1(id1, &rpc1);
    cm1.AddEdgeListener(be1);

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));
    cm1.ConnectTo(addr0);
    cm0.ConnectTo(addr1);

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_TRUE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_TRUE(cm1.GetConnectionTable().GetConnection(id0));

    cm0.Disconnect();

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    DisableLogging();

    cm1.ConnectTo(addr0);
    cm0.ConnectTo(addr1);

    next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    EXPECT_FALSE(cm0.GetConnectionTable().GetConnection(id1));
//    EXPECT_FALSE(cm1.GetConnectionTable().GetConnection(id0));

    EnableLogging();
  }
}
}
