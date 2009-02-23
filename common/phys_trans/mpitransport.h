#ifndef MPITRANSPORT_H
#define MPITRANSPORT_H

#include "transport.h"

class MpiTransport : public Transport
{
public:
   MpiTransport();
   ~MpiTransport();

   class MpiNode : public Node
   {
   public:
      MpiNode(SInt32 core_id);
      ~MpiNode();

      void globalSend(SInt32, const void*, UInt32);
      void send(SInt32, const void*, UInt32);
      Byte* recv();
      bool query();

   private:
      void send(SInt32 dest_proc, UInt32 tag, const void *buffer, UInt32 length);
   };

   Node* createNode(SInt32 core_id);

   void barrier();
   Node* getGlobalNode();

private:
   Node *m_global_node;

   static const SInt32 PROC_COMM_TAG=32767;
};

#endif 
