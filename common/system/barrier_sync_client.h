#ifndef __BARRIER_SYNC_CLIENT_H__
#define __BARRIER_SYNC_CLIENT_H__

#include <cassert>

#include "clock_skew_minimization_object.h"
#include "fixed_types.h"
#include "packetize.h"

// Forward Decls
class Tile;

class BarrierSyncClient : public ClockSkewMinimizationClient
{
   private:
      Tile* m_core;

      UInt64 m_barrier_interval;
      UInt64 m_next_sync_time;

   public:
      BarrierSyncClient(Tile* tile);
      ~BarrierSyncClient();

      void enable() {}
      void disable() {}

      void synchronize(UInt64 cycle_count);
      void netProcessSyncMsg(const NetPacket& packet) { assert(false); }

      static const unsigned int BARRIER_RELEASE = 0xBABECAFE;
};

#endif /* __BARRIER_SYNC_CLIENT_H__ */
