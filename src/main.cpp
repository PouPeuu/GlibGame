#include <iostream>
#include <DissensionFramework/shader.hpp>
#include <testipaska.hpp>

int main() {
    testkakka();
    kakka();

    /*lua_State* lua_state = luaL_newstate();
    if (!lua_state) {
        std::cerr << "Failed to create Lua state\n";
        return 1;
    }

    luaL_openlibs(lua_state);

    std::string script((std::istreambuf_iterator<char>(std::cin)),
                        std::istreambuf_iterator<char>());
    
    int load_status = luaL_loadbuffer(lua_state, script.c_str(), script.size(), "stdin");
    if (load_status != LUA_OK) {
        std::cerr << "Error loading script: " << lua_tostring(lua_state, -1) << std::endl;
        lua_pop(lua_state, 1);
        lua_close(lua_state);
        return 1;
    }

    int exec_status = lua_pcall(lua_state, 0, LUA_MULTRET, 0);
    if (exec_status != LUA_OK) {
        std::cerr << "Error executing script: " << lua_tostring(lua_state, -1) << std::endl;
        lua_pop(lua_state, 1);
        lua_close(lua_state);
        return 1;
    }

    lua_close(lua_state);*/
}