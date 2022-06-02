#include "neptunepch.h"
#include "MetalShader.h"

namespace Neptune
{

// ----- MetalShader -----------------

MetalShader::MetalShader(id<MTLDevice> device, const ShaderDesc& desc)
{
  @autoreleasepool
  {
		// Vertex
  	{
  	  NSError* err;
  	  id<MTLLibrary> lib = [device newLibraryWithSource: @(desc.vertexSrc)
  	                                            options: nil
  	                                              error: &err];
  	  NEPTUNE_ASSERT(lib, [err.description UTF8String]);
  	
  	  m_VertexFunc = [lib newFunctionWithName: @"vertexFunc"]; // TODO: Handle names better
  	  NEPTUNE_ASSERT(m_VertexFunc, "Failed to compile vertex shader!");
  	
  	  [lib release];
  	  lib = nil;
  	}
  	
  	// Fragment
  	{
  	  NSError* err;
  	  id<MTLLibrary> lib = [device newLibraryWithSource: @(desc.fragmentSrc)
  	                                            options: nil
  	                                              error: &err];
  	  NEPTUNE_ASSERT(lib, [err.description UTF8String]);
  	
  	  m_FragmentFunc = [lib newFunctionWithName: @"fragmentFunc"];
  	  NEPTUNE_ASSERT(m_FragmentFunc, "Failed to compile fragment shader!");
  	
  	  [lib release];
  	  lib = nil;
  	}
  }
}

MetalShader::~MetalShader()
{
  @autoreleasepool
  {
  	[m_VertexFunc release];
  	m_VertexFunc = nil;
  	
  	[m_FragmentFunc release];
  	m_FragmentFunc = nil;
  }
}

} // namespace Neptune
