#include <imgui.h>
#include <imgui_stdlib.h>
#include <SDL_image.h>
#include "imgui_utils.hpp"
#include "platform.hpp"
#include "tileset/source_tile.hpp"

using namespace PokeDev;

SourceTile::SourceTile() 
    : m_surface {}
    , m_texture {}
    , m_imagePath {}
    , m_name {"DEFAULT TILE NAME"}
{
    setSurface("missing_tile.png");
}

void SourceTile::drawEditor() {
    ImGui::Image(m_texture, ImVec2 {static_cast<float>(m_surface->w), static_cast<float>(m_surface->h)});

    if (ImGuiUtils::filePicker("Tile Image File", m_imagePath, Platform::FilePickerOptions{})) {
        setSurface(m_imagePath.string().c_str());
    }

    ImGui::InputText("Tile Name", &m_name);
}

SDL_Surface* SourceTile::getSurface() {
    return m_surface;
}

SDL_Texture* SourceTile::getTexture() {
    return m_texture;
}

const std::string& SourceTile::getName() {
    return m_name;
}

const std::filesystem::path& SourceTile::getPath() {
    return m_imagePath;
}

void SourceTile::setSurface(const char* file) {
        if (m_surface != nullptr) { SDL_FreeSurface(m_surface); }
        if (m_texture != nullptr) { SDL_DestroyTexture(m_texture); }
        m_surface = IMG_Load(file);
        m_texture = SDL_CreateTextureFromSurface(Platform::getRenderer(), m_surface);
}