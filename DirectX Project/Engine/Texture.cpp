#include "pch.h"
#include "Texture.h"

Texture::Texture() : ResourceBase(ResourceType::Texture)
{
}

Texture::~Texture()
{
}

const std::shared_ptr<DirectX::ScratchImage> Texture::GetInfo()
{
    ComPtr<ID3D11Texture2D> texture;
    shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());

    std::shared_ptr<DirectX::ScratchImage> image = std::make_shared<DirectX::ScratchImage>();
    HRESULT hr = DirectX::CaptureTexture(Global::g_device.Get(), Global::g_immediateContext.Get(), texture.Get(), *image.get());

    if(FAILED(hr))
		Utils::ShowErrorMessage(hr);

	return image;
}

bool Texture::Load(const std::wstring& path)
{
    this->path = path;

    ComPtr<ID3D11Texture2D> texture;
    auto imageobj = std::make_shared<DirectX::ScratchImage>();
    DirectX::TexMetadata mdata;

    // load dds Texture file
    HRESULT hr = DirectX::GetMetadataFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount()
                                                    , mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0
                                                    , CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                return true;
            }
            else
            {
                Utils::ShowErrorMessage(hr);
                return false;
            }
        }
        else
        {
            Utils::ShowErrorMessage(hr);
            return false;
        }
    }

    // load png, jpg, etc Texture file
    hr = DirectX::GetMetadataFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
             hr = DirectX::CreateShaderResourceViewEx(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount()
                                                , mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0
                                                , CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                return true;
            }
            else
            {
                Utils::ShowErrorMessage(hr);
                return false;
            }
        }
        else
        {
            Utils::ShowErrorMessage(hr);
            return false;
        }
    }

    // load tga Texture file
    hr = DirectX::GetMetadataFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, mdata);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromTGAFile(path.c_str(), DirectX::TGA_FLAGS_NONE, &mdata, *imageobj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(Global::g_device.Get(), imageobj->GetImages(), imageobj->GetImageCount()
                                                , mdata, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0
                                                , CREATETEX_DEFAULT, shaderResourceView.ReleaseAndGetAddressOf());
            if (SUCCEEDED(hr))
            {
                size.x = mdata.width;
                size.y = mdata.height;
                return true;
            }
            else
            {
                Utils::ShowErrorMessage(hr);
                return false;
            }
        }
        else
        {
            Utils::ShowErrorMessage(hr);
            return false;
        }
    }

    return false;
}

void Texture::CreateTexture(int width, int height)
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

    ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = Global::g_device->CreateTexture2D(&desc, &data, texture.ReleaseAndGetAddressOf());

    if(FAILED(hr))
        Utils::ShowErrorMessage(hr);

    hr = Global::g_device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.ReleaseAndGetAddressOf());

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

	delete[] buf;
}

void Texture::UpdateTexture(const std::vector<BYTE>& colors)
{
    ComPtr<ID3D11Texture2D> texture;
    shaderResourceView->GetResource((ID3D11Resource**)texture.ReleaseAndGetAddressOf());

    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	
    HRESULT hr = Global::g_immediateContext->Map(texture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    if (FAILED(hr))
        Utils::ShowErrorMessage(hr);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);

	for (int i = 0; i < size.x; i++)
	{
        UINT rowStart = i * mappedResource.RowPitch;
        UINT colorRowStart = i * size.x * 4;

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

void Texture::SaveTexture(const std::wstring& path)
{
    std::shared_ptr<DirectX::ScratchImage> srcimage = GetInfo();

    DirectX::SaveToWICFile(srcimage->GetImages(), srcimage->GetImageCount(),
        DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(DirectX::WIC_CODEC_PNG),
        path.c_str());
}

void Texture::GetTextureRGBAData(std::vector<BYTE>& colors)
{
    auto& info = GetInfo();
    auto mData = info->GetMetadata();
    auto images = info->GetImages();

    UINT rowNum = mData.height;
    UINT colNum = mData.width;

    colors.resize(rowNum * colNum * 4);
    BYTE* pTexels = (BYTE*)images->pixels;

    for (UINT i = 0; i < rowNum; i++)
    {
        UINT rowStart = i * images->rowPitch;
        UINT colorRowStart = i * 4 * mData.width;
        for (UINT j = 0; j < colNum; j++)
        {
            UINT colStart = j * 4;

            colors[colorRowStart + colStart + 0] = pTexels[rowStart + colStart + 0];
            colors[colorRowStart + colStart + 1] = pTexels[rowStart + colStart + 1];
            colors[colorRowStart + colStart + 2] = pTexels[rowStart + colStart + 2];
            colors[colorRowStart + colStart + 3] = pTexels[rowStart + colStart + 3];
        }
    }
}
