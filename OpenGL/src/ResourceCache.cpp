#include "ResourceCache.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "stb_image/stb_image.h"

std::unordered_map<std::string, Texture2D*> ResourceCache::m_Textures;
std::unordered_map<std::string, Shader*> ResourceCache::m_Shaders;


Texture2D* ResourceCache::LoadTexture(const std::string& name, const std::string& fp)
{
	stbi_set_flip_vertically_on_load(1);

	int w, h, channels;

	unsigned char* data = stbi_load(fp.c_str(), &w, &h, &channels, 4);

	Texture2D* texture = new Texture2D();

	texture->Generate(w, h, data);

	stbi_image_free(data);

	m_Textures[name] = texture;

	return texture;
}

Texture2D* ResourceCache::GetTexture(const std::string& name)
{
    return m_Textures[name];
}

Shader* ResourceCache::GetShader(const std::string& name)
{
	return m_Shaders[name];
}

Shader* ResourceCache::LoadShader(const std::string& name, const std::string& fp)
{
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::fstream stream(fp);

    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            // Set shader to vertex
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;

            // Set shader to fragment
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

	Shader* shader = new Shader();

    shader->Compile(ss[0].str(), ss[1].str());

    m_Shaders[name] = shader;

    return shader;
}
