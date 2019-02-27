#include "present/xorg/Keyboard.h"
#include <X11/keysym.h>

Keyboard::Keyboard(xcb_connection_t* connection):
    key_symbols(xcb_key_symbols_alloc(connection)) {
}

Key Keyboard::translate(xcb_keycode_t kc) {
    xcb_keysym_t key = xcb_key_symbols_get_keysym(this->key_symbols.get(), kc, 0);

    switch (key) {
        case XK_space: return Key::Space;
        case XK_apostrophe: return Key::Apostrophe;
        case XK_comma: return Key::Comma;
        case XK_minus: return Key::Minus;
        case XK_period: return Key::Period;
        case XK_slash: return Key::Slash;
        case XK_0: return Key::Num0;
        case XK_1: return Key::Num1;
        case XK_2: return Key::Num2;
        case XK_3: return Key::Num3;
        case XK_4: return Key::Num4;
        case XK_5: return Key::Num5;
        case XK_6: return Key::Num6;
        case XK_7: return Key::Num7;
        case XK_8: return Key::Num8;
        case XK_9: return Key::Num9;
        case XK_semicolon: return Key::Semicolon;
        case XK_equal: return Key::Equal;

        // xcb key press and release events should only return Key::these
        case XK_a: return Key::A;
        case XK_b: return Key::B;
        case XK_c: return Key::C;
        case XK_d: return Key::D;
        case XK_e: return Key::E;
        case XK_f: return Key::F;
        case XK_g: return Key::G;
        case XK_h: return Key::H;
        case XK_i: return Key::I;
        case XK_j: return Key::J;
        case XK_k: return Key::K;
        case XK_l: return Key::L;
        case XK_m: return Key::M;
        case XK_n: return Key::N;
        case XK_o: return Key::O;
        case XK_p: return Key::P;
        case XK_q: return Key::Q;
        case XK_r: return Key::R;
        case XK_s: return Key::S;
        case XK_t: return Key::T;
        case XK_u: return Key::U;
        case XK_v: return Key::V;
        case XK_w: return Key::W;
        case XK_x: return Key::X;
        case XK_y: return Key::Y;
        case XK_z: return Key::Z;

        case XK_bracketleft: return Key::LeftBracket;
        case XK_backslash: return Key::Backslash;
        case XK_bracketright: return Key::RightBracket;
        case XK_grave: return Key::GraveAccent;
        case XK_Escape: return Key::Escape;
        case XK_Return: return Key::Enter;
        case XK_Tab: return Key::Tab;
        case XK_BackSpace: return Key::Backspace;
        case XK_Insert: return Key::Insert;
        case XK_Delete: return Key::Delete;
        case XK_Right: return Key::Right;
        case XK_Left: return Key::Left;
        case XK_Down: return Key::Down;
        case XK_Up: return Key::Up;
        case XK_Page_Up: return Key::PageUp;
        case XK_Page_Down: return Key::PageDown;
        case XK_Home: return Key::Home;
        case XK_End: return Key::End;
        case XK_Caps_Lock: return Key::CapsLock;
        case XK_Scroll_Lock: return Key::ScrollLock;
        case XK_Num_Lock: return Key::NumLock;
        case XK_Pause: return Key::Pause;
        case XK_F1: return Key::F1;
        case XK_F2: return Key::F2;
        case XK_F3: return Key::F3;
        case XK_F4: return Key::F4;
        case XK_F5: return Key::F5;
        case XK_F6: return Key::F6;
        case XK_F7: return Key::F7;
        case XK_F8: return Key::F8;
        case XK_F9: return Key::F9;
        case XK_F10: return Key::F10;
        case XK_F11: return Key::F11;
        case XK_F12: return Key::F12;
        case XK_KP_0: return Key::Kp0;
        case XK_KP_1: return Key::Kp1;
        case XK_KP_2: return Key::Kp2;
        case XK_KP_3: return Key::Kp3;
        case XK_KP_4: return Key::Kp4;
        case XK_KP_5: return Key::Kp5;
        case XK_KP_6: return Key::Kp6;
        case XK_KP_7: return Key::Kp7;
        case XK_KP_8: return Key::Kp8;
        case XK_KP_9: return Key::Kp9;
        case XK_KP_Decimal: return Key::KpDecimal;
        case XK_KP_Divide: return Key::KpDivide;
        case XK_KP_Multiply: return Key::KpMultiply;
        case XK_KP_Subtract: return Key::KpSubtract;
        case XK_KP_Add: return Key::KpAdd;
        case XK_KP_Enter: return Key::KpEnter;
        case XK_KP_Equal: return Key::KpEqual;
        case XK_Shift_L: return Key::LeftShift;
        case XK_Control_L: return Key::LeftControl;
        case XK_Alt_L: return Key::LeftAlt;
        case XK_Super_L: return Key::LeftSuper;
        case XK_Shift_R: return Key::RightShift;
        case XK_Control_R: return Key::RightControl;
        case XK_Alt_R: return Key::RightAlt;
        case XK_Super_R: return Key::RightSuper;
        case XK_Menu: return Key::Menu;
        default: return Key::Unknown;
    }
}
