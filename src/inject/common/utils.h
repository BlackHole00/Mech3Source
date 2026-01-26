#pragma once

#define CUTL_COUNTOF(_x) (sizeof(_x) / sizeof(*_x))
#define CUTL_COUNTOF_FIELD(_type, _x) (sizeof(((_type*)0)->_x) / sizeof(*((_type*)0)->_x))
#define CUTL_MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
