
#pragma once

#include <vector>

#include <libevm/ExtVMFace.h>

#include "CompilerHelper.h"
#include "Utils.h"


#ifdef _MSC_VER
	#define EXPORT __declspec(dllexport)
#else
	#define EXPORT
#endif

namespace dev
{
namespace eth
{
namespace jit
{

struct RuntimeData
{
	static llvm::StructType* getType();

	i256 gas;
};

using StackImpl = std::vector<i256>;
using MemoryImpl = bytes;

class Runtime
{
public:
	Runtime(u256 _gas, ExtVMFace& _ext);
	~Runtime();

	Runtime(const Runtime&) = delete;
	void operator=(const Runtime&) = delete;

	RuntimeData* getDataPtr() { return &m_data; }

	static StackImpl& getStack();
	MemoryImpl& getMemory() { return m_memory; }
	static ExtVMFace& getExt();
	u256 getGas();
	bytesConstRef getReturnData();

private:

	/// @internal Must be the first element to asure Runtime* === RuntimeData*
	RuntimeData m_data;
	StackImpl m_stack;
	MemoryImpl m_memory;
	ExtVMFace& m_ext;
};

class RuntimeManager: public CompilerHelper
{
public:
	RuntimeManager(llvm::IRBuilder<>& _builder);

	llvm::Value* getRuntimePtr();

	llvm::Value* getGas();
	void setGas(llvm::Value* _gas);

private:
	llvm::GlobalVariable* m_dataPtr;
};

}
}
}
