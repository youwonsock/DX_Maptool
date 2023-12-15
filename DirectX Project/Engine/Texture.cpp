#include "pch.h"
#include "Texture.h"

Texture::Texture() : ResourceBase(ResourceType::Texture)
{
}

Texture::~Texture()
{
}

void Texture::Load(const std::wstring& path)
{
    // temporary
    this->path = path;
    // temporary

    imageobj = std::make_shared<DirectX::ScratchImage>();
    DirectX::TexMetadata mdata;

    // load dds Texture file
    HRESULT hr = DirectX::GetMetadataFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());
                return;
            }
            else
                ShowErrorMessage(hr);
        }
        else
            ShowErrorMessage(hr);
    }

    // load png, jpg, etc Texture file
    hr = DirectX::GetMetadataFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());
                return;
            }
            else
                ShowErrorMessage(hr);
        }
        else
            ShowErrorMessage(hr);
    }

    // load tga Texture file
    hr = DirectX::GetMetadataFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceView(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount(), mdata, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());
                return;
            }
            else
                ShowErrorMessage(hr);
        }
        else
            ShowErrorMessage(hr);
    }

    return;
}

void Texture::CreateAlphaTexture(int width, int height)
{
    D3D11_TEXTURE2D_DESC desc;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&desc, sizeof(desc));
    ZeroMemory(&data, sizeof(data));

    BYTE* buf = new BYTE[width * height * 4];

    for (int i = 0; i < width * height * 4; i)
    {
        buf[i + 0] = 0;
		buf[i + 1] = 0;
		buf[i + 2] = 0;
		buf[i + 3] = 0;

		i += 4;
    }

	data.pSysMem = (void*)buf;
	data.SysMemPitch = width * 4;
	data.SysMemSlicePitch = 0;

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

    size.x = width;
    size.y = height;

	HRESULT hr = Global::g_device->CreateTexture2D(&desc, &data, texture.ReleaseAndGetAddressOf());

    if(FAILED(hr))
	    ShowErrorMessage(hr);

    hr = Global::g_device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        ShowErrorMessage(hr);

	delete[] buf;
}

void Texture::UpdateAlphaTexture(const std::vector<BYTE>& colors)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
    HRESULT hr = Global::g_immediateContext->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    if (FAILED(hr))
		ShowErrorMessage(hr);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);

	for (int i = 0; i < size.x; i++)
	{
        UINT rowStart = i * mappedResource.RowPitch;
        UINT colorRowStart = i * size.x * 4;

        if (i > 127)
            int a = 0;

        int k = 0;
        for (int j = 0; j < size.y; ++j)
        {
			UINT colStart = j * 4;
			UINT index = rowStart + colStart;

			mappedData[index + 0] = colors[colorRowStart + k];
			mappedData[index + 1] = colors[colorRowStart + k + 1];
			mappedData[index + 2] = colors[colorRowStart + k + 2];
			mappedData[index + 3] = colors[colorRowStart + k + 3];
            
            k += 4;
        }
	}

	Global::g_immediateContext->Unmap(texture.Get(), 0);
}
