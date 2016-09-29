#include "TopInclude.h"
#include "Engine.h"
#include "Vector.h"
#include "List.h"
#include "Parser.h"
#include "Resource.h"
#include "directx.h"
#include "Camera.h"
#include "ObjModel.h"

struct SurfaceMaterial
{
	struct WString* pName;
	XMFLOAT4 difColor;
	ID TexResID;
	bool hasTexture;
	bool transparent;
};

struct ObjModel {
	bool alive;
	ID3D11BlendState* Transparency;
	ID3D11Buffer* meshVertBuff;
	ID3D11Buffer* meshIndexBuff;
	XMMATRIX meshWorld;
	int meshSubsets = 0;
	struct Vector* pMeshSubsetIndexStart;
	struct Vector* pMeshSubsetTexture;
	struct Vector* pMeshSRV;
	struct Vector* pSurfaceMaterial;
	UINT stride;
	XMMATRIX meshworld;
};

HRESULT ObjModel_New(ObjModel ** ppObjModel)
{
	_NEW(ObjModel,*ppObjModel);
	ObjModel* pObjModel = *ppObjModel;
	pObjModel->alive = true;

	int expectedMeshSubsetCount = 100;
	pObjModel->pMeshSubsetIndexStart = Vector_New(sizeof(int), expectedMeshSubsetCount);
	pObjModel->pMeshSubsetTexture = Vector_New(sizeof(int), expectedMeshSubsetCount);

	int expectedTextureCount = 10;
	pObjModel->pMeshSRV = Vector_New(sizeof(ID*), expectedTextureCount);

	pObjModel->pSurfaceMaterial = Vector_New(sizeof(SurfaceMaterial*), expectedTextureCount);

	pObjModel->meshWorld = XMMatrixIdentity();
	return S_OK;
}

HRESULT ObjModel_Delete(ObjModel* pObjModel)
{
	if (!pObjModel) { return S_OK; }
	CE1_ASSERT(!pObjModel->alive && "Deleting dead ObjModel");
	pObjModel->alive = false;
	CE1_CALL(Vector_Delete(pObjModel->pMeshSubsetIndexStart));
	CE1_CALL(Vector_Delete(pObjModel->pMeshSubsetTexture));

	ID3D11ShaderResourceView* pMeshSRV;
	CE1_VECEXEC(
		pObjModel->pMeshSRV,
		pMeshSRV = (ID3D11ShaderResourceView*)Vector_Get(pObjModel->pMeshSRV, i);
		SAFE_RELEASE(pMeshSRV);
	);
	CE1_CALL(Vector_Delete(pObjModel->pMeshSRV));

	SurfaceMaterial* pSurfaceMaterial;
	CE1_VECEXEC(
		pObjModel->pSurfaceMaterial,
		pSurfaceMaterial = (SurfaceMaterial*)Vector_Get(pObjModel->pSurfaceMaterial, i);
		CE1_DEL(pSurfaceMaterial->pName->pBuffer);
		CE1_DEL(pSurfaceMaterial->pName);
	);
	CE1_CALL(Vector_FullDelete(pObjModel->pSurfaceMaterial,true));

	CE1_DEL(pObjModel);
	return S_OK;
}

HRESULT ObjModel_LoadFromFile(ObjModel* pObjModel, String* pFile) {
	CE1_CALL(ObjModel_LoadFromFile(pObjModel, pFile, true, false));
	return S_OK;
}

HRESULT ObjModel_Render(ObjModel * pObjModel)
{
	UINT offset = 0;
	XMMATRIX WVP;
	cbPerObject cbPerObj;
	SurfaceMaterial* pSM;
	XMMATRIX VP = *Camera_GetViewTimesProjection(Engine_GetCamera());
	ID3D11ShaderResourceView* pSRV;
	ID3D11DeviceContext* pDC = Engine_GetCD3D11()->pImmediateContext;
	for (int i = 0; i < pObjModel->meshSubsets; ++i)
	{
		pDC->IASetIndexBuffer(pObjModel->meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
		pDC->IASetVertexBuffers(0, 1, &pObjModel->meshVertBuff, &pObjModel->stride, &offset);
		WVP = pObjModel->meshWorld * VP;
		cbPerObj.WVP = XMMatrixTranspose(WVP);
		cbPerObj.World = XMMatrixTranspose(pObjModel->meshWorld);
		pSM = (SurfaceMaterial*)Vector_Get(pObjModel->pSurfaceMaterial, *(int*)Vector_Get(pObjModel->pMeshSubsetTexture, i));
		cbPerObj.difColor = pSM->difColor;
		cbPerObj.hasTexture = pSM->hasTexture;
		pDC->UpdateSubresource(Engine_GetCD3D11()->cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		pDC->VSSetConstantBuffers(0, 1, &Engine_GetCD3D11()->cbPerObjectBuffer);
		pDC->PSSetConstantBuffers(1, 1, &Engine_GetCD3D11()->cbPerObjectBuffer);
		if (pSM->hasTexture) {
			ResourceManager_GetResource(pSM->TexResID, (void**)&pSRV);
		}
		pDC->PSSetShaderResources(0, 1, &pSRV);
		//pDC->PSSetSamplers(0, 1, &CubesTexSamplerState);
		pDC->RSSetState(Engine_GetCD3D11()->RSCullNone);
		int indexStart = *(int*)Vector_Get(pObjModel->pMeshSubsetIndexStart,i);
		int indexDrawAmount = *(int*)Vector_Get(pObjModel->pMeshSubsetIndexStart, i+1) - indexStart;
		if (!pSM->transparent) {
			pDC->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}
	return S_OK;
}

HRESULT ObjModel_LoadFromFile(ObjModel * pObjModel, String * pFile,bool isRHCoordSys,bool computeNormals)
{
	std::wifstream fileIn(pFile->pBuffer);
	std::wstring meshMatLib; 
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<std::wstring> meshMaterials;
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;
	bool hasTexCoord = false;
	bool hasNorm = false;
	std::wstring meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;
	wchar_t checkChar; 
	std::wstring face;    
	int vIndex = 0;        
	int triangleCount = 0;  
	int totalVerts = 0;
	int meshTriangles = 0;
	if (fileIn)
	{
		while (fileIn)
		{
			checkChar = fileIn.get();   

			switch (checkChar)
			{
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':    
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;  
					if (isRHCoordSys)  
						vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f)); 
					else
						vertPos.push_back(XMFLOAT3(vx, vy, vz));
				}
				if (checkChar == 't') 
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv; 
					if (isRHCoordSys) 
						vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));
					else
						vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));
					hasTexCoord = true;  
				}
				if (checkChar == 'n') 
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;
					if (isRHCoordSys) 
						vertNorm.push_back(XMFLOAT3(vnx, vny, vnz * -1.0f)); 
					else
						vertNorm.push_back(XMFLOAT3(vnx, vny, vnz));

					hasNorm = true; 
				}
				break;
			case 'g': 
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					CE1_NEW(int,pIndex);
					*pIndex = vIndex;
					Vector_Pushback(pObjModel->pMeshSubsetIndexStart,pIndex);
					pObjModel->meshSubsets++;
				}
				break;
			case 'f':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;  
					triangleCount = 0;
					checkChar = fileIn.get();
					while (checkChar != '\n')
					{
						face += checkChar;    
						checkChar = fileIn.get();
						if (checkChar == ' ')
							triangleCount++;
					}
					if (face[face.length() - 1] == ' ')
						triangleCount--;  
					triangleCount -= 1;
					std::wstringstream ss(face);
					if (face.length() > 0)
					{
						int firstVIndex, lastVIndex; 

						for (int i = 0; i < 3; ++i)
						{
							ss >> VertDef;  
							std::wstring vertPart;
							int whichPart = 0; 
							for (int j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/') 
									vertPart += VertDef[j];
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);
									if (whichPart == 0)  
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1; 
										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"") 
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;
									}
									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);
										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}
									vertPart = L""; 
									whichPart++;                   
								}
							}
							if (pObjModel->meshSubsets == 0)
							{
								CE1_NEW(int,pIndex);
								*pIndex = vIndex;
								Vector_Pushback(pObjModel->pMeshSubsetIndexStart,pIndex); 
								pObjModel->meshSubsets++;
							}
							bool vertAlreadyExists = false;
							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExists = true;
										}
									}
								}
							}
							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++; 
								indices.push_back(totalVerts - 1);
							}
							if (i == 0)
							{
								firstVIndex = indices[vIndex]; 

							}
							if (i == 2)
							{
								lastVIndex = indices[vIndex]; 
							}
							vIndex++;  
						}
						meshTriangles++; 
						{
							indices.push_back(firstVIndex);
							vIndex++;
							indices.push_back(lastVIndex); 
							vIndex++;
							ss >> VertDef;
							std::wstring vertPart;
							int whichPart = 0;
							for (int j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/')
									vertPart += VertDef[j];
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);
									if (whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;
										if (j == VertDef.length() - 1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;
									}
									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);
										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}
									vertPart = L"";
									whichPart++;
								}
							}
							bool vertAlreadyExists = false;
							if (totalVerts >= 3)
							{
								for (int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);
											vertAlreadyExists = true;
										}
									}
								}
							}
							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;   
								indices.push_back(totalVerts - 1);  
							}     
							lastVIndex = indices[vIndex]; 
							meshTriangles++; 
							vIndex++;
						}
					}
				}
				break;
			case 'm':
				checkChar = fileIn.get();
				if (checkChar == 't')
				{
					checkChar = fileIn.get();
					if (checkChar == 'l')
					{
						checkChar = fileIn.get();
						if (checkChar == 'l')
						{
							checkChar = fileIn.get();
							if (checkChar == 'i')
							{
								checkChar = fileIn.get();
								if (checkChar == 'b')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}
				break;
			case 'u': 
				checkChar = fileIn.get();
				if (checkChar == 's')
				{
					checkChar = fileIn.get();
					if (checkChar == 'e')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										meshMaterialsTemp = L""; 
										fileIn >> meshMaterialsTemp;
										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else  
	{
		CE1_ASSERT(0&&"Could not open .obj file");
		return ERROR_SUCCESS;
	}
	CE1_NEW(int,pIndex);
	*pIndex = vIndex;
	Vector_Pushback(pObjModel->pMeshSubsetIndexStart,pIndex);
	if (*(int*)Vector_Get(pObjModel->pMeshSubsetIndexStart, 1) == 0)
	{
		CE1_CALL(Vector_Erase(pObjModel->pMeshSubsetIndexStart,1,true));
		pObjModel->meshSubsets--;
	}
	if (!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if (!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));
	fileIn.close();
	fileIn.open(meshMatLib.c_str());
	std::wstring lastStringRead;
	SurfaceMaterial* pCurrentMat = 0;
	bool kdset = false;

	if (fileIn)
	{
		while (fileIn)
		{
			checkChar = fileIn.get(); 
			switch (checkChar)
			{
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'K':
				checkChar = fileIn.get();
				if (checkChar == 'd')
				{
					checkChar = fileIn.get(); 
					fileIn >> pCurrentMat->difColor.x;
					fileIn >> pCurrentMat->difColor.y;
					fileIn >> pCurrentMat->difColor.z;
					kdset = true;
				}
				if (checkChar == 'a')
				{
					checkChar = fileIn.get();
					if (!kdset)
					{
						pCurrentMat->difColor.x;
						pCurrentMat->difColor.y;
						pCurrentMat->difColor.z;
					}
				}
				break;
			case 'T':
				checkChar = fileIn.get();
				if (checkChar == 'r')
				{
					checkChar = fileIn.get();
					float Transparency;
					fileIn >> Transparency;
					pCurrentMat->difColor.w = Transparency;
					if (Transparency > 0.0f)
						pCurrentMat->transparent = true;
				}
				break;
			case 'd':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float Transparency;
					fileIn >> Transparency;
					Transparency = 1.0f - Transparency;
					pCurrentMat->difColor.w = Transparency;
					if (Transparency > 0.0f)
						pCurrentMat->transparent = true;
				}
				break;
			case 'm':
				checkChar = fileIn.get();
				if (checkChar == 'a')
				{
					checkChar = fileIn.get();
					if (checkChar == 'p')
					{
						checkChar = fileIn.get();
						if (checkChar == '_')
						{
							checkChar = fileIn.get();
							if (checkChar == 'K')
							{
								checkChar = fileIn.get();
								if (checkChar == 'd')
								{
									std::wstring fileNamePath;
									fileIn.get();
									bool texFilePathEnd = false;
									while (!texFilePathEnd)
									{
										checkChar = fileIn.get();
										fileNamePath += checkChar;
										if (checkChar == '.')
										{
											for (int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}
									}
									bool alreadyLoaded = false;
									WString* pFileNamePath;
									CE1_WSTR(pFileNamePath, fileNamePath.c_str());
									CE1_CALL(ResourceManager_LoadResource(pFileNamePath, &pCurrentMat->TexResID));
									pCurrentMat->hasTexture = true;
								}
							}
							else if (checkChar == 'd')
							{
								pCurrentMat->transparent = true;
							}
						}
					}
				}
				break;
			case 'n':
				checkChar = fileIn.get();
				if (checkChar == 'e')
				{
					checkChar = fileIn.get();
					if (checkChar == 'w')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										CE1_NEW(SurfaceMaterial, pTempMat);
										Vector_Pushback(pObjModel->pSurfaceMaterial, pTempMat);
										std::wstring tempWString;
										fileIn >> tempWString;
										CE1_WSTR(pCurrentMat->pName,tempWString.c_str());
										pCurrentMat->transparent = false;
										pCurrentMat->hasTexture = false;
										pCurrentMat->TexResID = 0;
										kdset = false;
									}
								}
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		CE1_ASSERT(0&&"Could not open material lib");
		return ERROR_SUCCESS;
	}
	std::vector<Vertex_UnlitTextured> vertices;
	Vertex_UnlitTextured tempVert;
	for (int j = 0; j < totalVerts; ++j)
	{
		tempVert.Pos = vertPos[vertPosIndex[j]];
		tempVert.Normal = vertNorm[vertNormIndex[j]];
		tempVert.Uv = vertTexCoord[vertTCIndex[j]];
		vertices.push_back(tempVert);
	}
	if (computeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float vecX, vecY, vecZ;
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < meshTriangles; ++i)
		{
			vecX = vertices[indices[(i * 3)]].Pos.x - vertices[indices[(i * 3) + 2]].Pos.x;
			vecY = vertices[indices[(i * 3)]].Pos.y - vertices[indices[(i * 3) + 2]].Pos.y;
			vecZ = vertices[indices[(i * 3)]].Pos.z - vertices[indices[(i * 3) + 2]].Pos.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);
			vecX = vertices[indices[(i * 3) + 2]].Pos.x - vertices[indices[(i * 3) + 1]].Pos.x;
			vecY = vertices[indices[(i * 3) + 2]].Pos.y - vertices[indices[(i * 3) + 1]].Pos.y;
			vecZ = vertices[indices[(i * 3) + 2]].Pos.z - vertices[indices[(i * 3) + 1]].Pos.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
			tempNormal.push_back(unnormalized);
		}
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX;
		float tY;
		float tZ;
		for (int i = 0; i < totalVerts; ++i)
		{
			for (int j = 0; j < meshTriangles; ++j)
			{
				if (indices[j * 3] == i ||
					indices[(j * 3) + 1] == i ||
					indices[(j * 3) + 2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;
					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);
					facesUsing++;
				}
			}
			normalSum = normalSum / facesUsing;
			normalSum = XMVector3Normalize(normalSum);
			vertices[i].Normal.x = XMVectorGetX(normalSum);
			vertices[i].Normal.y = XMVectorGetY(normalSum);
			vertices[i].Normal.z = XMVectorGetZ(normalSum);
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			facesUsing = 0;
		}
	}
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	Engine_GetCD3D11()->pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &pObjModel->meshIndexBuff);

	pObjModel->stride = sizeof(Vertex_UnlitTextured);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = pObjModel->stride * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	Engine_GetCD3D11()->pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &pObjModel->meshVertBuff);

	return S_OK;
}