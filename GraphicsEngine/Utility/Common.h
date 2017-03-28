#pragma once

#define CHECK_BYTE(value, byte) ((value & byte) > 0)

#define MAP_ELEM_NOT_FOUND(iter, map) ((iter) == (map).end())