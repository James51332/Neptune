#include "neptunepch.h"
#include "MetalPipelineState.h"

#include "MetalShader.h"

#import <Metal/MTLVertexDescriptor.h>

namespace Neptune
{

// ----- MetalPipelineState -----------------

static MTLVertexFormat MTLVertexFormatFromPipelineAttributeType(PipelineAttributeType type, bool normalized)
{
	if (!normalized)
  {
  	switch (type)
  	{
  	  case PipelineAttributeType::Float: { return MTLVertexFormatFloat; }
  	  case PipelineAttributeType::Float2: { return MTLVertexFormatFloat2; }
  	  case PipelineAttributeType::Float3: { return MTLVertexFormatFloat3; }
  	  case PipelineAttributeType::Float4: { return MTLVertexFormatFloat4; }
  	  case PipelineAttributeType::Int: { return MTLVertexFormatInt; }
  	  case PipelineAttributeType::Int2: { return MTLVertexFormatInt2; }
  	  case PipelineAttributeType::Int3: { return MTLVertexFormatInt3; }
  	  case PipelineAttributeType::Int4: { return MTLVertexFormatInt4; }
      case PipelineAttributeType::Char: { return MTLVertexFormatChar; }
      case PipelineAttributeType::Char2: { return MTLVertexFormatChar2; }
      case PipelineAttributeType::Char3: { return MTLVertexFormatChar3; }
      case PipelineAttributeType::Char4: { return MTLVertexFormatChar4; }
      case PipelineAttributeType::UInt: { return MTLVertexFormatUInt; }
      case PipelineAttributeType::UInt2: { return MTLVertexFormatUInt2; }
      case PipelineAttributeType::UInt3: { return MTLVertexFormatUInt3; }
      case PipelineAttributeType::UInt4: { return MTLVertexFormatUInt4; }
  	  case PipelineAttributeType::UChar: { return MTLVertexFormatUChar; }
      case PipelineAttributeType::UChar2: { return MTLVertexFormatUChar2; }
      case PipelineAttributeType::UChar3: { return MTLVertexFormatUChar3; }
      case PipelineAttributeType::UChar4: { return MTLVertexFormatUChar4; }
    }
  } else
  {
    switch (type)
    {
      case PipelineAttributeType::Int: { return MTLVertexFormatInt1010102Normalized; }
      case PipelineAttributeType::UInt: { return MTLVertexFormatUInt1010102Normalized; }
      case PipelineAttributeType::UChar4: { return MTLVertexFormatUChar4Normalized; }
      default: { NEPTUNE_ASSERT(false, "PipelineAttributeType unsupported in Metal!"); return MTLVertexFormatFloat; }
    }
  }
}

MetalPipelineState::MetalPipelineState(id<MTLDevice> device, const PipelineStateDesc& desc)
{
  @autoreleasepool
  {
  	MTLRenderPipelineDescriptor *pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
  	pipelineDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
  	
  	// obtain the functions from the shader.
    {
  		Ref<MetalShader> metalShader = StaticRefCast<MetalShader>(desc.Shader);
    	pipelineDesc.vertexFunction = metalShader->GetVertexFunction();
  		pipelineDesc.fragmentFunction = metalShader->GetFragmentFunction();
    }
    
    // set the vertex layout of the pipeline
    {
      MTLVertexDescriptor* descriptor = [MTLVertexDescriptor vertexDescriptor];
      
      Size i = 0;
      for (auto& attr : desc.Layout)
      {
        descriptor.attributes[i].offset = attr.Offset;
        descriptor.attributes[i].format = MTLVertexFormatFromPipelineAttributeType(attr.Type, attr.Normalized);
        descriptor.attributes[i].bufferIndex = 0;
        ++i;
      }
      
      descriptor.layouts[0].stride = desc.Layout.Stride;
      pipelineDesc.vertexDescriptor = descriptor;
    }
    
  	NSError* err;
  	m_PipelineState = [device newRenderPipelineStateWithDescriptor: pipelineDesc
  	                                                         error: &err];
  	NEPTUNE_ASSERT(m_PipelineState, [err.description UTF8String]);
  	
  	[pipelineDesc release];
  	pipelineDesc = nil;
  }
}

MetalPipelineState::~MetalPipelineState()
{
  @autoreleasepool
  {
    [m_PipelineState release];
    m_PipelineState = nil;
  }
}

} // namespace Neptune
