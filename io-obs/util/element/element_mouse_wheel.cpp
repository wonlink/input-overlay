/**
 * This file is part of input-overlay
 * which is licensed under the MPL 2.0 license
 * See LICENSE or mozilla.org/en-US/MPL/2.0/
 * github.com/univrsal/input-overlay
 */

#include "../../sources/input_source.hpp"
#include "element_mouse_wheel.hpp"
#include "../util.hpp"
#include "../../hook/hook_helper.hpp"
#include <util/platform.h>
#include "util/layout_constants.hpp"

element_wheel::element_wheel()
    : element_texture(MOUSE_SCROLLWHEEL)
{
    /* NO-OP */
};

void element_wheel::load(ccl_config* cfg, const std::string& id)
{
    element_texture::load(cfg, id);
    m_keycode = VC_MOUSE_WHEEL;
    auto i = 1;
    for (auto& map : m_mappings)
    {
        map = m_mapping;
        map.x += i * (m_mapping.cx + CFG_INNER_BORDER);
        i++;
    }
}

void element_wheel::draw(gs_effect_t* effect, gs_image_file_t* image,
    element_data* data, sources::shared_settings* settings)
{
    if (data)
    {
        const auto wheel = dynamic_cast<element_data_wheel*>(data);

        if (wheel)
        {
            if (os_gettime_ns() - hook::last_wheel >=
                SCROLL_TIMEOUT)
            {
                wheel->set_dir(WHEEL_DIR_NONE);
            }

            if (wheel->get_state() == STATE_PRESSED)
                element_texture::draw(effect, image, &m_mappings[WHEEL_MAP_MIDDLE]);

            switch (wheel->get_dir())
            {
            case WHEEL_DIR_UP:
                element_texture::draw(effect, image, &m_mappings[WHEEL_MAP_UP]);
                break;
            case WHEEL_DIR_DOWN:
                element_texture::draw(effect, image, &m_mappings[WHEEL_MAP_DOWN]);
                break;
            default:
            case WHEEL_DIR_NONE: ;
            }
        }
    }

    element_texture::draw(effect, image, data, settings);
}

data_source element_wheel::get_source()
{
    return DEFAULT;
}

element_data_wheel::element_data_wheel(const wheel_direction dir, const int amount)
    : element_data(MOUSE_SCROLLWHEEL)
{
    m_data_type = WHEEL_STATE;
    m_dir = dir;
    m_amount = amount;
}

element_data_wheel::element_data_wheel(const button_state state)
    : element_data(MOUSE_SCROLLWHEEL)
{
    m_data_type = BUTTON_STATE;
    m_middle_button = state;
}

int element_data_wheel::get_amount() const
{
    return m_amount;
}

wheel_direction element_data_wheel::get_dir() const
{
    return m_dir;
}

void element_data_wheel::set_dir(const wheel_direction dir)
{
    m_dir = dir;
}

button_state element_data_wheel::get_state() const
{
    return m_middle_button;
}

bool element_data_wheel::is_persistent()
{
    return true;
}

void element_data_wheel::merge(element_data* other)
{
    if (other && other->get_type() == m_type)
    {
        const auto other_wheel = dynamic_cast<element_data_wheel*>(other);
        if (other_wheel)
        {
            switch (other_wheel->m_data_type)
            {
            case BUTTON_STATE:
#ifdef _DEBUG
                blog(LOG_INFO, "Merged button state: %i old, %i new\n",
                    m_middle_button, other_wheel->get_state());
#endif
                m_middle_button = other_wheel->get_state();
                break;
            case WHEEL_STATE:
                m_amount = other_wheel->get_amount();
                m_dir = other_wheel->get_dir();
                break;
            default: ;
            }
        }
    }
}
