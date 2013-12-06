#include "luacppinterface.h"
#include "luauserdata.h"

#include <sstream>

// Added by hibm98(강정규)
#include <functional>
#include <csetjmp>

std::string LuaGetLastError(lua_State* state)
{
        std::stringstream ss;
        for (int level = 1; level <= lua_gettop(state); level++)
        {
                const char* errorMessage = lua_tostring(state, level);
                if (errorMessage)
                {
                        ss << "Error: " << errorMessage << std::endl;
                }
        }

        lua_Debug debugInfo;
        for (int level = 0; lua_getstack(state, level, &debugInfo); level++)
        {
                lua_getinfo(state, "nSlf", &debugInfo);
                ss << "Line: " << debugInfo.currentline << std::endl;
                ss << "Source: " << debugInfo.source << std::endl;

                const char* function = lua_tostring(state, -1);
                if (function)
                {
                        ss << "Function: " << function << std::endl;
                }
        }

        return ss.str();
}

Lua::Lua() : 
	state(luaL_newstate(), lua_close),
	registry(LuaTable(state, LUA_REGISTRYINDEX)),
	globals(registry.Get<LuaTable>(LUA_RIDX_GLOBALS))
{
}

Lua::Lua(std::shared_ptr<lua_State> state) :
	state(state),
	registry(LuaTable(state, LUA_REGISTRYINDEX)),
	globals(registry.Get<LuaTable>(LUA_RIDX_GLOBALS))

{
}
	
void Lua::LoadStandardLibraries()
{
	luaL_openlibs(state.get());
}

LuaTable Lua::GetRegistry() const
{
	return registry;
}

LuaTable Lua::GetGlobalEnvironment() const
{
	return globals;
}

LuaTable Lua::CreateTable()
{
	lua_newtable(state.get());
	LuaTable table = LuaTable(state, -1);
	lua_pop(state.get(), 1);
	return table;
}

LuaCoroutine Lua::CreateCoroutine()
{
	lua_newthread(state.get());
	LuaCoroutine thread(state, -1);
	lua_pop(state.get(), 1);
	return thread;
}

// Modified by hibm98(강정규)
std::string Lua::RunScript(std::string script)
{
	int err = luaL_dostring(state.get(), script.c_str());
	std::string result;

	if (err != LUA_OK)
	{
		const char* r = lua_tostring(state.get(), -1);
		result = LuaGetLastError(state.get());
	}
	else
	{
		result = "No errors";
	}

	lua_settop(state.get(), 0);
	return result;
}

// Added by hibm98(강정규)
thread_local jmp_buf place;
thread_local int countdown;

void hooker(lua_State* L, lua_Debug* ar)
{

	if (countdown > 0)
		--countdown;
	else
		luaL_error(L, "Time exceed.");
}

std::string Lua::RunScriptWithHook(std::string script, int limit)
{
	std::string result;

	// 카운트다운을 초기화한다.
	countdown = limit;

	// hook을 설치한다.
	lua_sethook(state.get(), hooker, LUA_MASKCOUNT, 1);

	result = this->RunScript(script);

	// hook을 해제한다.
	lua_sethook(state.get(), hooker, 0, 1);

	// 결과 반환.
	return result;
}

Lua::~Lua()
{
}
