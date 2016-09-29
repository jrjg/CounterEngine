#include "TopInclude.h"
#include "Engine.h"
#include "Vector.h"
#include "List.h"
#include "Parser.h"

#include "ObjModel.h"

struct SurfaceMaterial
{
	struct String* name;
	XMFLOAT4 difColor;
	int texArrayIndex;
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
	struct Vector* pTextureNameArray;
	struct Vector* pSurfaceMaterial;

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
	pObjModel->pMeshSRV = Vector_New(sizeof(ID3D11ShaderResourceView*), expectedTextureCount);
	pObjModel->pTextureNameArray = Vector_New(sizeof(String*), expectedTextureCount);

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
		CE1_DEL(pSurfaceMaterial->name->pBuffer);
		CE1_DEL(pSurfaceMaterial->name);
	);
	CE1_CALL(Vector_FullDelete(pObjModel->pSurfaceMaterial,true));

	CE1_CALL(Vector_FullDelete(pObjModel->pTextureNameArray,true));

	CE1_DEL(pObjModel);
	return S_OK;
}

HRESULT ObjModel_LoadFromFile(ObjModel * pObjModel, String * pFile,bool isRHCoordSys,bool computeNormals)
{
	HRESULT hr = 0;

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
									WString* pTexName;
									CE1_VECEXEC(
										pObjModel->pTextureNameArray,
										pTexName = (WString*)Vector_Get(pObjModel->pTextureNameArray,i);
										if (CE1_CompareWStrings(pTexName, pFileNamePath)) {
											alreadyLoaded = true;
											pCurrentMat->texArrayIndex = i;
											pCurrentMat->hasTexture = true;
										}
									);
									if (!alreadyLoaded)
									{

										ID3D11ShaderResourceView* tempMeshSRV;
										hr = D3DX11CreateShaderResourceViewFromFile(d3d11Device, fileNamePath.c_str(),
											NULL, NULL, &tempMeshSRV, NULL);
										if (SUCCEEDED(hr))
										{
											textureNameArray.push_back(fileNamePath.c_str());
											material[matCount - 1].texArrayIndex = meshSRV.size();
											meshSRV.push_back(tempMeshSRV);
											material[matCount - 1].hasTexture = true;
										}
									}
								}
							}
							//map_d - alpha map
							else if (checkChar == 'd')
							{
								//Alpha maps are usually the same as the diffuse map
								//So we will assume that for now by only enabling
								//transparency for this material, as we will already
								//be using the alpha channel in the diffuse map
								material[matCount - 1].transparent = true;
							}
						}
					}
				}
				break;

			case 'n':    //newmtl - Declare new material
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
										//New material, set its defaults
										SurfaceMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].transparent = false;
										material[matCount].hasTexture = false;
										material[matCount].texArrayIndex = 0;
										matCount++;
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
		SwapChain->SetFullscreenState(false, NULL);    //Make sure we are out of fullscreen

		std::wstring message = L"Could not open: ";
		message += meshMatLib;

		MessageBox(0, message.c_str(),
			L"Error", MB_OK);

		return false;
	}

	//Set the subsets material to the index value
	//of the its material in our material array
	for (int i = 0; i < meshSubsets; ++i)
	{
		bool hasMat = false;
		for (int j = 0; j < material.size(); ++j)
		{
			if (meshMaterials[i] == material[j].matName)
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if (!hasMat)
			subsetMaterialArray.push_back(0); //Use first material in array
	}

	std::vector<Vertex> vertices;
	Vertex tempVert;

	//Create our vertices using the information we got 
	//from the file and store them in a vector
	for (int j = 0; j < totalVerts; ++j)
	{
		tempVert.pos = vertPos[vertPosIndex[j]];
		tempVert.normal = vertNorm[vertNormIndex[j]];
		tempVert.texCoord = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
	}

	//////////////////////Compute Normals///////////////////////////
	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	if (computeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		for (int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i * 3)]].pos.x - vertices[indices[(i * 3) + 2]].pos.x;
			vecY = vertices[indices[(i * 3)]].pos.y - vertices[indices[(i * 3) + 2]].pos.y;
			vecZ = vertices[indices[(i * 3)]].pos.z - vertices[indices[(i * 3) + 2]].pos.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

															//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[indices[(i * 3) + 2]].pos.x - vertices[indices[(i * 3) + 1]].pos.x;
			vecY = vertices[indices[(i * 3) + 2]].pos.y - vertices[indices[(i * 3) + 1]].pos.y;
			vecZ = vertices[indices[(i * 3) + 2]].pos.z - vertices[indices[(i * 3) + 1]].pos.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

															//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));
			tempNormal.push_back(unnormalized);            //Save unormalized normal (for normal averaging)
		}

		//Compute vertex normals (normal Averaging)
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX;
		float tY;
		float tZ;

		//Go through each vertex
		for (int i = 0; i < totalVerts; ++i)
		{
			//Check which triangles use this vertex
			for (int j = 0; j < meshTriangles; ++j)
			{
				if (indices[j * 3] == i ||
					indices[(j * 3) + 1] == i ||
					indices[(j * 3) + 2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);    //If a face is using the vertex, add the unormalized face normal to the normalSum
					facesUsing++;
				}
			}

			//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = normalSum / facesUsing;

			//Normalize the normalSum vector
			normalSum = XMVector3Normalize(normalSum);

			//Store the normal in our current vertex
			vertices[i].normal.x = XMVectorGetX(normalSum);
			vertices[i].normal.y = XMVectorGetY(normalSum);
			vertices[i].normal.z = XMVectorGetZ(normalSum);

			//Clear normalSum and facesUsing for next vertex
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			facesUsing = 0;

		}
	}

	//Create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, indexBuff);

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertBuff);

	return true;
	return S_OK;
}




//struct ObjModelCreator {
//	ObjModel* pObjModel;
//
//	bool isRHCoordSys;
//	bool computeNormals;
//	bool hasNorm;
//	String* pMatLibName;
//	Vector* pIndices;
//	Vector* pVertPos;
//	Vector* pVertNorm;
//	Vector* pVertTexCoord;
//	Vector* pMeshMaterials;
//	int vIndex;
//};
//
//HRESULT ObjModel_NewCreator(ObjModelCreator** ppObjModelCreator) {
//	_NEW(ObjModelCreator, *ppObjModelCreator);
//	ObjModelCreator* pObjModelCreator = *ppObjModelCreator;
//	CE1_CALL(ObjModel_New(&pObjModelCreator->pObjModel));
//	pObjModelCreator->isRHCoordSys = false;
//	pObjModelCreator->computeNormals = true;
//	pObjModelCreator->pMatLibName = 0;
//	pObjModelCreator->pIndices = Vector_New(sizeof(DWORD*), 100);
//	pObjModelCreator->pVertPos = Vector_New(sizeof(XMFLOAT3*), 100);
//	pObjModelCreator->pVertNorm = Vector_New(sizeof(XMFLOAT3*), 100);
//	pObjModelCreator->pVertTexCoord = Vector_New(sizeof(XMFLOAT2*), 100);
//	pObjModelCreator->pMeshMaterials = Vector_New(sizeof(String*), 100);
//	pObjModelCreator->hasNorm = false;
//	pObjModelCreator->vIndex = 0;
//	return S_OK;
//}
//
//
//
//#define CASE(NAME,EXEC)if (CE1_CMPSTR(pChildName->pBuffer, NAME, pChildName->length)) {EXEC;return S_OK;}
//
//HRESULT ObjModel_Handler(void* pParent, String* pChildName, void* pChild) {
//	ObjModelCreator* pObjModelCreator = (ObjModelCreator*)pParent;
//	CE1_ASSERT(pObjModelCreator&&pChildName&&pChild&&"ObjModel_Handler");
//
//	CASE("#", 1);
//	CASE("v",
//		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT3*)pChild)->z *= -1.0f; }
//	Vector_Pushback(pObjModelCreator->pVertPos, pChild);
//	);
//	CASE("vt",
//		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT2*)pChild)->y *= -1.0f; }
//	Vector_Pushback(pObjModelCreator->pVertTexCoord, pChild);
//	);
//	CASE("vn",
//		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT3*)pChild)->z *= -1.0f; }
//	Vector_Pushback(pObjModelCreator->pVertNorm, pChild);
//	pObjModelCreator->hasNorm = true;
//	);
//	CASE("g",
//		CE1_NEW(int, pIndex);
//	*pIndex = pObjModelCreator->vIndex;
//	Vector_Pushback(pObjModelCreator->pObjModel->pMeshSubsetIndexStart, pIndex);
//	pObjModelCreator->pObjModel->meshSubsets++;
//	);
//	CASE("f",
//
//		);
//	CASE("mtllib",
//
//		);
//	CASE("usemtl",
//
//		);
//	CE1_ASSERT(0 && "unknown Object received");
//}
//
//HRESULT ObjModel_LoadFromFile(String* pObjFilename) {
//	Parser* pParser = Parser_New();
//	CE1_CALL(Parser_DeclareVariable(pParser, "String", "mtllib", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "String", "usemtl", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vt", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vn", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "v", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "String", "g", &ObjectModelHandler));
//	CE1_CALL(Parser_DeclareVariable(pParser, "String", "#", &ObjectModelHandler));
//
//
//	//CE1_CALL(Parser_DeclareType(pParser,"XMFLOAT3",sizeof(XMFLOAT3),&Parser_ConvertStringToVertex));
//	CE1_CALL(Parser_RegisterOperator(pParser, "\n", OperatorCode::submit));
//
//	//parse
//	ObjModelCreator* pObjModelCreator;
//	CE1_CALL(ObjModel_NewCreator(&pObjModelCreator));
//	CE1_CALL(Parser_ParseFile(pParser, "spaceCompound.obj", &ObjectModelHandler, pObjModelCreator));
//
//}