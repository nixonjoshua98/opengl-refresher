#pragma once

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture2D.h"

class ResourceCache
{
public:
	static std::unordered_map<std::string, Texture2D*> m_Textures;
	static std::unordered_map<std::string, Shader*> m_Shaders;

	static Texture2D* GetTexture(const std::string& name);
	static Texture2D* LoadTexture(const std::string& name, const std::string& fp);

	static Shader* GetShader(const std::string& name);
	static Shader* LoadShader(const std::string& name, const std::string& fp);
};