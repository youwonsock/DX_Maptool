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
