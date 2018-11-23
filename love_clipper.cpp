#include <stdio.h>
#include "clipper/clipper.hpp"
extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
}

using namespace ClipperLib;

static int love_clipper_merge_polygons(lua_State *L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);
    unsigned int n1 = luaL_getn(L, 1);
    unsigned int n2 = luaL_getn(L, 2);
    float *v1 = (float *)malloc(sizeof(float)*n1);
    float *v2 = (float *)malloc(sizeof(float)*n2);
    for (int i = 0; i < n1; i++) {
        lua_rawgeti(L, 1, i+1);
        v1[i] = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }
    for (int i = 0; i < n2; i++) {
        lua_rawgeti(L, 2, i+1);
        v2[i] = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    Paths p1(1);
    Paths p2(1); 
    Paths solution;
    for (int i = 0; i < n1; i+=2) {
        p1[0] << IntPoint((int)(v1[i]*1000), (int)(v1[i+1]*1000));
    }
    for (int i = 0; i < n2; i+=2) {
        p2[0] << IntPoint((int)(v2[i]*1000), (int)(v2[i+1]*1000));
    }

    Clipper c;
    c.AddPaths(p1, ptSubject, true);
    c.AddPaths(p2, ptClip, true);
    c.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);

    lua_newtable(L);
    for (int i = 0; i < solution.size(); i++) {
        lua_pushnumber(L, i+1);
        int k = 1;
        lua_newtable(L);
        for (int j = 0; j < solution[i].size(); j++) {
            lua_pushnumber(L, k);
            lua_pushnumber(L, (float)(solution[i][j].X/1000.0));
            lua_settable(L, -3);
            k++;
            lua_pushnumber(L, k);
            lua_pushnumber(L, (float)(solution[i][j].Y/1000.0));
            lua_settable(L, -3);
            k++;
        }
        lua_settable(L, -3);
    }
    return 1;
}

static const struct luaL_reg love_clipper_methods[] = {
    {"merge_polygons", love_clipper_merge_polygons},
    {NULL, NULL}
};

extern "C" {
__declspec(dllexport) int luaopen_love_clipper(lua_State *L) {
    luaL_openlib(L, "love_clipper", love_clipper_methods, 0);
    return 1;
}
}
