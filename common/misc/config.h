// config.h
//
// The Config class is used to set, store, and retrieve all the configurable
// parameters of the simulator.
//
// Initial creation: Sep 18, 2008 by jasonm

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include "fixed_types.h"

class Config
{
public:
   class CoreParameters
   {
      private:
         std::string m_type;
         volatile float m_frequency;
         std::string m_l1_icache_type;
         std::string m_l1_dcache_type;
         std::string m_l2_cache_type;

      public:
         CoreParameters(std::string type, volatile float frequency, std::string l1_icache_type, std::string l1_dcache_type, std::string l2_cache_type):
            m_type(type),
            m_frequency(frequency),
            m_l1_icache_type(l1_icache_type),
            m_l1_dcache_type(l1_dcache_type),
            m_l2_cache_type(l2_cache_type)
         {}
         ~CoreParameters() {}

         volatile float getFrequency() { return m_frequency; }
         void setFrequency(volatile float frequency) { m_frequency = frequency; }
         std::string getType() { return m_type; }
         std::string getL1ICacheType() { return m_l1_icache_type; }
         std::string getL1DCacheType() { return m_l1_dcache_type; }
         std::string getL2CacheType() { return m_l2_cache_type; }
   };

   class NetworkParameters
   {
      private:
         std::string m_type;
         volatile float m_frequency;

      public:
         NetworkParameters(std::string type, volatile float frequency):
            m_type(type), m_frequency(frequency)
         {}
         ~NetworkParameters() {}

         volatile float getFrequency() { return m_frequency; }
         std::string getType() { return m_type; }
   };

   enum SimulationMode
   {
      FULL = 0,
      LITE,
      NUM_SIMULATION_MODES
   };
   typedef std::vector<tile_id_t> TileList;
   typedef std::vector<UInt32> TileToProcMap;
   typedef std::vector<tile_id_t>::const_iterator TLCI;
   typedef std::map<UInt32,tile_id_t> CommToTileMap;

   Config();
   ~Config();

   void loadFromFile(char* filename);
   void loadFromCmdLine();

   // Return the number of processes involved in this simulation
   UInt32 getProcessCount() { return m_num_processes; }
   void setProcessCount(UInt32 in_num_processes) { m_num_processes = in_num_processes; }

   // Retrieve and set the process number for this process (I'm expecting
   //  that the initialization routine of the Transport layer will set this)
   UInt32 getCurrentProcessNum() { return m_current_process_num; }
   void setProcessNum(UInt32 in_my_proc_num) { m_current_process_num = in_my_proc_num; }

   tile_id_t getMCPTileNum() { return (getTotalTiles() -1); }
   core_id_t getMCPCoreId() { return (core_id_t) {getTotalTiles() -1, MAIN_CORE_TYPE}; }

   tile_id_t getMainThreadTileNum() { return 0; }
   core_id_t getMainThreadCoreId() { return (core_id_t) {0, MAIN_CORE_TYPE}; }

   tile_id_t getThreadSpawnerTileNum(UInt32 proc_num);
   core_id_t getThreadSpawnerCoreId(UInt32 proc_num);
   tile_id_t getCurrentThreadSpawnerTileNum(); 
   core_id_t getCurrentThreadSpawnerCoreId(); 

   // Return the number of modules (tiles) in a given process
   UInt32 getNumTilesInProcess(UInt32 proc_num)
   { 
      assert (proc_num < m_num_processes); 
      return m_proc_to_tile_list_map[proc_num].size(); 
   }

   SInt32 getIndexFromTileID(UInt32 proc_num, tile_id_t tile_id);
   tile_id_t getTileIDFromIndex(UInt32 proc_num, SInt32 index);
   
   UInt32 getNumLocalTiles() { return getNumTilesInProcess(getCurrentProcessNum()); }

   // Return the total number of modules in all processes
   UInt32 getTotalTiles();
   UInt32 getApplicationTiles();

   // Return an array of tile numbers for a given process
   //  The returned array will have numMods(proc_num) elements
   const TileList & getTileListForProcess(UInt32 proc_num)
   { assert(proc_num < m_num_processes); return m_proc_to_tile_list_map[proc_num]; }

   const TileList & getTileListForCurrentProcess()
   { return getTileListForProcess(getCurrentProcessNum()); }

   UInt32 getProcessNumForTile(UInt32 tile)
   { 
     if (tile >= m_total_tiles)
     {
       fprintf(stderr, "tile(%u), m_total_tiles(%u)\n", tile, m_total_tiles);
       exit(-1);
     }
     return m_tile_to_proc_map[tile]; 
   }

   // For mapping between user-land communication id's to actual tile id's
   void updateCommToTileMap(UInt32 comm_id, tile_id_t tile_id);
   UInt32 getTileFromCommId(UInt32 comm_id);

   // Get TileId length
   UInt32 getTileIDLength()
   { return m_tile_id_length; }

   SimulationMode getSimulationMode()
   { return m_simulation_mode; }

   // Tile & Network Parameters
   std::string getCoreType(tile_id_t tile_id);
   std::string getL1ICacheType(tile_id_t tile_id);
   std::string getL1DCacheType(tile_id_t tile_id);
   std::string getL2CacheType(tile_id_t tile_id);
   volatile float getCoreFrequency(core_id_t core_id);
   void setCoreFrequency(core_id_t core_id, volatile float frequency);

   std::string getNetworkType(SInt32 network_id);

   // Knobs
   bool getEnableDCacheModeling() const;
   bool getEnableICacheModeling() const;
   bool isSimulatingSharedMemory() const;
   bool getEnablePerformanceModeling() const;
   bool getEnablePowerModeling() const;
   bool getEnableAreaModeling() const;

   // Logging
   std::string getOutputFileName() const;
   std::string formatOutputFileName(std::string filename) const;
   void logTileMap();

   static Config *getSingleton();

private:
   void GenerateTileMap();
   std::vector<TileList> computeProcessToTileMapping();
   void printProcessToTileMapping();
   
   UInt32  m_num_processes;         // Total number of processes (incl myself)
   UInt32  m_total_tiles;           // Total number of tiles in all processes
   UInt32  m_application_tiles;     // Total number of tiles used by the application
   UInt32  m_tile_id_length;        // Number of bytes needed to store a tile_id

   UInt32  m_current_process_num;   // Process number for this process

   std::vector<CoreParameters> m_core_parameters_vec;         // Vector holding main tile parameters
   std::vector<NetworkParameters> m_network_parameters_vec;   // Vector holding network parameters

   // This data structure keeps track of which tiles are in each process.
   // It is an array of size num_processes where each element is a list of
   // tile numbers.  Each list specifies which tiles are in the corresponding
   // process.

   TileToProcMap m_tile_to_proc_map;
   TileList* m_proc_to_tile_list_map;
   CommToTileMap m_comm_to_tile_map;

   // Simulation Mode
   SimulationMode m_simulation_mode;

   UInt32  m_mcp_process;          // The process where the MCP lives

   static Config *m_singleton;

   static UInt32 m_knob_total_tiles;
   static UInt32 m_knob_num_process;
   static bool m_knob_simarch_has_shared_mem;
   static std::string m_knob_output_file;
   static bool m_knob_enable_dcache_modeling;
   static bool m_knob_enable_icache_modeling;
   static bool m_knob_enable_performance_modeling;
   static bool m_knob_enable_power_modeling;
   static bool m_knob_enable_area_modeling;

   // Get Tile & Network Parameters
   void parseCoreParameters();
   void parseNetworkParameters();

   static SimulationMode parseSimulationMode(std::string mode);
   static UInt32 computeTileIDLength(UInt32 tile_count);
   static UInt32 getNearestAcceptableTileCount(UInt32 tile_count);
};

#endif
