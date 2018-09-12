/**
 * This file is part of input-overlay
 * which is licensed under the MPL 2.0 license
 * See LICENSE or mozilla.org/en-US/MPL/2.0/
 * github.com/univrsal/input-overlay
 */

#include "element_button.hpp"
#include "../../ccl/ccl.hpp"

void element_button::load(ccl_config* cfg, const std::string& id)
{
    element_texture::load(cfg, id);
    m_keycode = cfg->get_int(id + CFG_KEY_CODE);
    m_pressed = m_mapping;
    m_pressed.y = m_mapping.cy + CFG_INNER_BORDER;
}

void element_button::draw(gs_effect_t* effect, gs_image_file_t* image, element_data* data)
{
    if (data && reinterpret_cast<element_data_button*>(data)->get_state() == STATE_PRESSED)
    {
        element_texture::draw(effect, image, &m_pressed);
    }
    else
    {
        element_texture::draw(effect, image, nullptr);
    }
}
