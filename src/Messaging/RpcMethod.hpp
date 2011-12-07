#ifndef DISSENT_RPC_METHOD_H_GUARD
#define DISSENT_RPC_METHOD_H_GUARD

#include "RpcRequest.hpp"

namespace Dissent {
namespace Messaging {
  /**
   * Common base class so we can use a template for Rpc methods
   */
  class Callback {
    public:
      /**
       * Invoke the underlying method with the request to receive a response
       * @param request map consisting of request variables
       * @param response map to store responses
       */
      virtual void Invoke(RpcRequest& request) = 0;
  };

  /**
   * Used to store an RpcMethod for either incoming requests or handling responses
   */
  template<class T> class RpcMethod : public Callback {
    public:
      /**
       * T method signature
       */
      typedef void (T::*Method)(RpcRequest &request);

      /**
       * Constructor
       * @param object the object to make the call upon
       * @param method the method to call
       * @param name name used to map to this callback
       */
      RpcMethod(T &object, Method method) :
        _object(object), _method(method)
      {
      }

      inline virtual void Invoke(RpcRequest &request)
      {
        (_object.*_method)(request);
      }

    private:
      T &_object;
      Method _method;
  };
}
}

#endif
