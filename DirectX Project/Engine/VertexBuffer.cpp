#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	this->device = Global::g_device;
}

VertexBuffer::~VertexBuffer()
{
	vertexBuffer.Reset();
}

void VertexBuffer::PushData()
{
	Global::g_immediateContext->IASetVertexBuffers(slot, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
