#include "IntegerTypeInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/LLVMContext.h"

namespace PascalCompiler {

IntegerTypeInfo::IntegerTypeInfo():
    BasicTypeInfo(BasicTypeInfo::INTEGER)
{
    m_name = "integer";
    m_llvmType  = llvm::TypeBuilder<llvm::types::i<32>, false>::get(llvm::getGlobalContext());
}

}
