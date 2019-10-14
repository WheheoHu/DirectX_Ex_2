
#include "Object_Index.h"


Square_Index::Square_Index()
{

}

Square_Index::~Square_Index()
{
	if (_ib)
	{
		_ib->Release();
		_ib = 0;
	}
	if (_vb)
	{
		_vb->Release();
		_vb = 0;
	}
}


bool Square_Index::CreateBuffer(IDirect3DDevice9* _device)
{
	HRESULT hr = 0;
	//创建顶点缓存
	hr = _device->CreateVertexBuffer(
		4 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	if (FAILED(hr))
	{
		MessageBox(0, "Square_Index: CreateVertexBuffer() - FAILED", 0, 0);
		return false;
	}
	//访问顶点缓存区域
	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-0.5f, -0.5f, 0.0f);
	v[1] = Vertex(0.5f, -0.5f, 0.0f);
	v[2] = Vertex(-0.5f, 0.5f, 0.0f);
	v[3] = Vertex(0.5f, 0.5f, 0.0f);

	_vb->Unlock();

	//创建索引缓存
	hr = _device->CreateIndexBuffer(
		2 * 3 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);
	if (FAILED(hr))
	{
		MessageBox(0, "Square_Index: CreateIndexBuffer() - FAILED", 0, 0);
		return false;
	}

	//访问索引缓存区域
	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	i[0] = 0; i[1] = 2; i[2] = 1;
	i[3] = 2; i[4] = 3; i[5] = 1;

	_ib->Unlock();

	return true;
}

void Square_Index::Render(IDirect3DDevice9* _device)
{
	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);

	_device->SetFVF(FVF_VERTEX);

	_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_device->SetTransform(D3DTS_WORLD, &m_Transform);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}



Plane_Index::Plane_Index()
{

}

Plane_Index::~Plane_Index()
{
	if (_ib)
	{
		_ib->Release();
		_ib = 0;
	}
	if (_vb)
	{
		_vb->Release();
		_vb = 0;
	}
}


bool Plane_Index::CreateBuffer(IDirect3DDevice9* _device)
{
	return CreateBuffer(_device, 2, 2, 1);
}


bool Plane_Index::CreateBuffer(IDirect3DDevice9* _device, int _width, int _height, float _resolution)
{
	if (_width <= 0 || _height <= 0 || _resolution <= 0)
	{
		MessageBox(0, "Plane_Index parameters are wrong!", 0, 0);
		return false;
	}

	d_width = _width;
	d_height = _height;
	d_resolution = _resolution;

	HRESULT hr = 0;
	//创建顶点缓存
	hr = _device->CreateVertexBuffer(
		(d_width + 1) * (d_height + 1) * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	if (FAILED(hr))
	{
		MessageBox(0, "Plane_Index: CreateVertexBuffer() - FAILED", 0, 0);
		return false;
	}
	//访问顶点缓存区域
	Vertex* _v;
	_vb->Lock(0, 0, (void**)&_v, 0);


	for (int i = 0; i <= d_height; i++)
	{
		for (int j = 0; j <= d_width; j++)
		{
			_v[i * (d_width + 1) + j] = Vertex((j - d_width * 0.5f) * d_resolution,
				(i - d_height * 0.5f) * d_resolution,
				0.0f);
		}
	}

	_vb->Unlock();

	//创建索引缓存
	hr = _device->CreateIndexBuffer(
		d_width * d_height * 6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);
	if (FAILED(hr))
	{
		MessageBox(0, "Plane_Index: CreateIndexBuffer() - FAILED", 0, 0);
		return false;
	}

	//访问索引缓存区域
	WORD* _i = 0;
	_ib->Lock(0, 0, (void**)&_i, 0);

	for (int i = 0; i < d_height; i++)
	{
		for (int j = 0; j < d_width; j++)
		{
			int _cell = i * d_width + j;
			_i[_cell * 6 + 0] = i * (d_width + 1) + j;
			_i[_cell * 6 + 1] = (i + 1) * (d_width + 1) + j;
			_i[_cell * 6 + 2] = i * (d_width + 1) + j + 1;

			_i[_cell * 6 + 3] = (i + 1) * (d_width + 1) + j;
			_i[_cell * 6 + 4] = (i + 1) * (d_width + 1) + j + 1;
			_i[_cell * 6 + 5] = i * (d_width + 1) + j + 1;
		}
	}

	_ib->Unlock();

	return true;
}

void Plane_Index::Render(IDirect3DDevice9* _device)
{
	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);

	_device->SetFVF(FVF_VERTEX);

	_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_device->SetTransform(D3DTS_WORLD, &m_Transform);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		(d_width + 1) * (d_height + 1), 0, d_width * d_height * 2);
}

Cube_Index::Cube_Index()
{
}

Cube_Index::~Cube_Index()
{
	if (_ib)
	{
		_ib->Release();
		_ib = 0;
	}
	if (_vb)
	{
		_vb->Release();
		_vb = 0;
	}
}

bool Cube_Index::CreateBuffer(IDirect3DDevice9 * _device)
{
	HRESULT hr = 0;
	hr = _device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);
	if (FAILED(hr))
	{
		MessageBox(0, "Cube_Index: CreateVertexBuffer() - FAILED", 0, 0);
		return false;
	}
	//访问顶点缓存区域
	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-0.5f, -0.5f, 0.5f);
	v[1] = Vertex(0.5f, -0.5f, 0.5f);
	v[2] = Vertex(-0.5f, 0.5f, 0.5f);
	v[3] = Vertex(0.5f, 0.5f, 0.5f);

	v[4] = Vertex(-0.5f, -0.5f, -0.5f);
	v[5] = Vertex(0.5f, -0.5f, -0.5f);
	v[6] = Vertex(-0.5f, 0.5f, -0.5f);
	v[7] = Vertex(0.5f, 0.5f, -0.5f);

	_vb->Unlock();

	//创建索引缓存
	hr = _device->CreateIndexBuffer(
		6 * 3 * 2 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);
	if (FAILED(hr))
	{
		MessageBox(0, "Box_Color: CreateIndexBuffer() - FAILED", 0, 0);
		return false;
	}

	//访问索引缓存区域
	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	//front face
	i[0] = 0; i[1] = 2; i[2] = 1;
	i[3] = 2; i[4] = 3; i[5] = 1;

	//back face
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 6; i[10] = 5; i[11] = 7;

	//left face
	i[12] = 0; i[13] = 4; i[14] = 6;
	i[15] = 6; i[16] = 2; i[17] = 0;

	//right face
	i[18] = 1; i[19] = 3; i[20] = 5;
	i[21] = 3; i[22] = 7; i[23] = 5;

	//top face
	i[24] = 2; i[25] = 6; i[26] = 3;
	i[27] = 6; i[28] = 7; i[29] = 3;

	//Bottom face
	i[30] = 0; i[31] = 1; i[32] = 4;
	i[33] = 4; i[34] = 1; i[35] = 5;

	_ib->Unlock();

	return true;
}

void Cube_Index::Render(IDirect3DDevice9 * _device)
{
	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);
	_device->SetFVF(FVF_VERTEX);
	_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_device->SetTransform(D3DTS_WORLD, &m_Transform);
	_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}
