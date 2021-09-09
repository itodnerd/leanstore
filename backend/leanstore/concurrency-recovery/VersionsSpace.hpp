#pragma once
#include "Exceptions.hpp"
#include "Units.hpp"
#include "VersionsSpaceInterface.hpp"
#include "leanstore/Config.hpp"
#include "leanstore/KVInterface.hpp"
#include "leanstore/storage/btree/BTreeLL.hpp"
#include "leanstore/utils/Misc.hpp"
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <shared_mutex>
#include <thread>
#include <unordered_map>
#include <vector>
// -------------------------------------------------------------------------------------
namespace leanstore
{
namespace cr
{
// -------------------------------------------------------------------------------------
using BTreeLL = leanstore::storage::btree::BTreeLL;
class VersionsSpace : public VersionsSpaceInterface
{
  private:
   struct alignas(64) Session {
      BufferFrame* bf;
      u64 version;
      s64 pos = -1;
      TXID last_tx_id;
      bool init = false;
   };
   Session sessions[leanstore::cr::STATIC_MAX_WORKERS];

  public:
   BTreeLL* btree;
   virtual void insertVersion(WORKERID session_id, TXID tx_id, u64 command_id, u64 payload_length, std::function<void(u8*)> cb);
   virtual bool retrieveVersion(WORKERID session_id, TXID tx_id, u64 command_id, std::function<void(const u8*, u64 payload_length)> cb);
   virtual void purgeTXIDRange(TXID from_tx_id, TXID to_tx_id);  // [from, to]
};
// -------------------------------------------------------------------------------------
}  // namespace cr
}  // namespace leanstore
