#pragma once

#include <algorithm>
#include <chrono>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <memory>
#include <vector>
#include <Windows.h>
#include <glm/glm.hpp>


struct Vertex {
	glm::vec4 pos;
	glm::vec2 uv;
};

class Application
{
public:
	Application(HWND hwnd, int width, int height);

	void Update();
	void Render();

	void InitShaders();
	void Initialize(HWND hwnd, int width, int height);
	void Clean();

public:
	int width_, height_;
	std::vector<glm::vec4> pixels; // 색깔 값들을 저장
	//Rasterization rasterization_; // Render()에서 pixels 에 그림

	ID3D11Device* device_;
	ID3D11DeviceContext* device_context_;
	IDXGISwapChain* swap_chain_;
	D3D11_VIEWPORT viewport_;
	ID3D11RenderTargetView* render_target_view_;
	ID3D11VertexShader* vertex_shader_;
	ID3D11PixelShader* pixel_shader_;
	ID3D11InputLayout* layout_;

	ID3D11Buffer* vertex_buffer_ = nullptr;
	ID3D11Buffer* index_buffer_ = nullptr;
	ID3D11Texture2D* canvas_texture_ = nullptr;
	// TODO : 홍랩강의 삼각형 래스터화 7:30 까지 완료

};

