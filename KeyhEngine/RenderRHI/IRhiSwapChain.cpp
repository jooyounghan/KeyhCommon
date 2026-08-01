#include "RenderRhiPch.h"
#include "IRhiSwapChain.h"
#include "IRhiDevice.h"

namespace keyh
{
	IRhiSwapChain::IRhiSwapChain(const RHISwapChainDesc& desc)
		: _desc(desc)
	{
	}


	D3D12SwapChain::D3D12SwapChain(const RHISwapChainDesc& desc, IDXGIFactory7* factory, IRhiCommandQueue* presentQueue)
		: IRhiSwapChain(desc)
	{
		initialize(factory, presentQueue);
	}

	bool D3D12SwapChain::initialize(IDXGIFactory7* factory, IRhiCommandQueue* presentQueue)
	{
		const D3D12ResourceFormatInfo& formatInfo = D3D12ResourceFormatInfo::getInfo(_desc._format);

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = _desc._width;
		swapChainDesc.Height = _desc._height;
		swapChainDesc.Format = formatInfo._format;
		swapChainDesc.Stereo = FALSE;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = _desc._bufferCount;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Flags = 0;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = factory->CreateSwapChainForHwnd(
			nullptr,
			static_cast<HWND>(_desc._windowHandle),
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain1
		);

		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to create swap chain. HRESULT: 0x%X", hr);
			return false;
		}

		hr = swapChain1.As(&_swapChain);
		if (FAILED(hr))
		{
			KEYH_ASSERT_ARGS(false, "Failed to query IDXGISwapChain3 interface. HRESULT: 0x%X", hr);
			return false;
		}

		return true;
	}
}

//class IRHICommandQueue
//{
//public:
//    virtual ~IRHICommandQueue() = default;
//
//    virtual void executeCommandLists() = 0;
//};
//
//class IRHISwapChain
//{
//public:
//    virtual ~IRHISwapChain() = default;
//
//    virtual bool present(bool enableVSync) = 0;
//    virtual void resize(uint32_t width, uint32_t height) = 0;
//};
//
//class IRHIDevice
//{
//public:
//    virtual ~IRHIDevice() = default;
//
//    virtual IRHICommandQueue* createCommandQueue(RHIQueueType type) = 0;
//    virtual IRHISwapChain* createSwapChain(const RHISwapChainDesc& desc, IRHICommandQueue* presentQueue) = 0;
//};
//
//class D3D12CommandQueue : public IRHICommandQueue
//{
//public:
//    D3D12CommandQueue(ComPtr<ID3D12CommandQueue> queue)
//        : _commandQueue(queue)
//    {}
//
//    void executeCommandLists() override
//    {}
//
//    ID3D12CommandQueue* getD3D12CommandQueue() const
//    {
//        return _commandQueue.Get();
//    }
//
//private:
//    ComPtr<ID3D12CommandQueue> _commandQueue;
//};
//
//class D3D12SwapChain : public IRHISwapChain
//{
//public:
//    D3D12SwapChain(ComPtr<IDXGISwapChain3> swapChain)
//        : _swapChain(swapChain)
//    {}
//
//    bool present(bool enableVSync) override
//    {
//        if (_swapChain == nullptr)
//        {
//            return false;
//        }
//
//        UINT syncInterval = enableVSync ? 1 : 0;
//        HRESULT hr = _swapChain->Present(syncInterval, 0);
//        return SUCCEEDED(hr);
//    }
//
//    void resize(uint32_t width, uint32_t height) override
//    {
//        if (_swapChain == nullptr)
//        {
//            return;
//        }
//
//        _swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
//    }
//
//    IDXGISwapChain3* getD3D12SwapChain() const
//    {
//        return _swapChain.Get();
//    }
//
//private:
//    ComPtr<IDXGISwapChain3> _swapChain;
//};
//
//class D3D12Device : public IRHIDevice
//{
//public:
//    D3D12Device(ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory4> dxgiFactory)
//        : _device(device), _dxgiFactory(dxgiFactory)
//    {}
//
//    IRHICommandQueue* createCommandQueue(RHIQueueType type) override
//    {
//        if (_device == nullptr)
//        {
//            return nullptr;
//        }
//
//        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
//        switch (type)
//        {
//        case RHIQueueType::Graphics:
//            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
//            break;
//        case RHIQueueType::Compute:
//            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
//            break;
//        case RHIQueueType::Copy:
//            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
//            break;
//        }
//
//        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
//        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
//        queueDesc.NodeMask = 0;
//
//        ComPtr<ID3D12CommandQueue> d3d12Queue;
//        HRESULT hr = _device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&d3d12Queue));
//        if (FAILED(hr))
//        {
//            return nullptr;
//        }
//
//        return new D3D12CommandQueue(d3d12Queue);
//    }
//
//    IRHISwapChain* createSwapChain(const RHISwapChainDesc& desc, IRHICommandQueue* presentQueue) override
//    {
//        if (_dxgiFactory == nullptr || presentQueue == nullptr)
//        {
//            return nullptr;
//        }
//
//        D3D12CommandQueue* d3d12QueueObj = static_cast<D3D12CommandQueue*>(presentQueue);
//        ID3D12CommandQueue* rawQueue = d3d12QueueObj->getD3D12CommandQueue();
//
//        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//        swapChainDesc.Width = desc.width;
//        swapChainDesc.Height = desc.height;
//        swapChainDesc.Format = convertFormat(desc.format);
//        swapChainDesc.Stereo = FALSE;
//        swapChainDesc.SampleDesc.Count = 1;
//        swapChainDesc.SampleDesc.Quality = 0;
//        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//        swapChainDesc.BufferCount = desc.bufferCount;
//        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
//        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
//        swapChainDesc.Flags = 0;
//
//        ComPtr<IDXGISwapChain1> swapChain1;
//        HRESULT hr = _dxgiFactory->CreateSwapChainForHwnd(
//            rawQueue,
//            static_cast<HWND>(desc.windowHandle),
//            &swapChainDesc,
//            nullptr,
//            nullptr,
//            &swapChain1
//        );
//
//        if (FAILED(hr))
//        {
//            return nullptr;
//        }
//
//        ComPtr<IDXGISwapChain3> swapChain3;
//        hr = swapChain1.As(&swapChain3);
//        if (FAILED(hr))
//        {
//            return nullptr;
//        }
//
//        return new D3D12SwapChain(swapChain3);
//    }
//
//private:
//    DXGI_FORMAT convertFormat(RHIPixelFormat format)
//    {
//        switch (format)
//        {
//        case RHIPixelFormat::R8G8B8A8_UNORM:
//            return DXGI_FORMAT_R8G8B8A8_UNORM;
//        case RHIPixelFormat::B8G8R8A8_UNORM:
//            return DXGI_FORMAT_B8G8R8A8_UNORM;
//        case RHIPixelFormat::R16G16B16A16_FLOAT:
//            return DXGI_FORMAT_R16G16B16A16_FLOAT;
//        default:
//            return DXGI_FORMAT_R8G8B8A8_UNORM;
//        }
//    }
//
//private:
//    ComPtr<ID3D12Device> _device;
//    ComPtr<IDXGIFactory4> _dxgiFactory;
//};