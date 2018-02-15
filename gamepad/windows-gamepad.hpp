#ifndef WINDOWS_GAMEPAD_HPP
#define WINDOWS_GAMEPAD_HPP

#define DEAD_ZONE(x, dz) ((x < dz) && (x > -dz))
#define X_PRESSED(b) ((m_xinput.Gamepad.wButtons & b) != 0)

#include <unistd.h>
#include "layout.hpp"

#if HAVE_XINPUT
#include <Xinput.h>
#endif

struct WindowsGamepad
{
public:
    WindowsGamepad(uint8_t id, std::vector<InputKey> * keys);
    {
        m_pad_id = id;
        m_keys = keys;
    }

    ~WindowsGamepad()
    {
        unload();
        m_keys = nullptr;
    }

    void load();
    void unload();
    void update(uint8_t id, uint16_t r_dz, uint16_t l_dz);

    void check_keys();

    bool is_valid() { return m_valid; }
    
    float left_stick_x() { return m_l_stick_x; }
    float left_stick_y() { return m_l_stick_y; }

    float right_stick_x() { return m_r_stick_x; }
    float right_stick_y() { return m_r_stick_y; }
private:
    uint8_t m_pad_id = 0;
    XINPUT_STATE m_xinput;
    
	/* Data */
	std::vector<InputKey> * m_keys;
	float m_l_stick_x = 0.f, m_l_stick_y = 0.f;
	float m_r_stick_x = 0.f, m_r_stick_y = 0.f;
	uint16_t m_r_dead_zone = 0, m_l_dead_zone = 0;

    bool m_valid = false;
};

#endif // WINDOWS_GAMEPAD_HPP