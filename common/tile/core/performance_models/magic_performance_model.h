#ifndef MAGIC_PERFORMANCE_MODEL_H
#define MAGIC_PERFORMANCE_MODEL_H

#include "core_model.h"

class MagicPerformanceModel : public CoreModel
{
public:
   MagicPerformanceModel(Core *core, float frequency);
   ~MagicPerformanceModel();

   void reset();
   void outputSummary(std::ostream &os);

   UInt64 getInstructionCount() { return m_instruction_count; }

private:
   void handleInstruction(Instruction *instruction);

   bool isModeled(InstructionType instruction_type);
   
   UInt64 m_instruction_count;
};

#endif
