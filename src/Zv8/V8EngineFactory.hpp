//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Python plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/Scripting.hpp>
#include <Zen/Scripting/I_ScriptEngineFactory.hpp>

#include <v8.h>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class V8EngineFactory
:   public Zen::Scripting::I_ScriptEngineFactory
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ScriptEngineFactory implementation
    /// @{
public:
    virtual pScriptEngine_type create(const std::string& _name);
    /// @}

    /// @name PythonEngineFactory implementation
    /// @{
public:
    void onDestroy(wpScriptEngine_type _pScriptEngine);
    /// @}

    /// @name Static methods
    /// @{
public:
    static V8EngineFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
public:
             V8EngineFactory();
    virtual ~V8EngineFactory();
    /// @}

    /// @name Member variables
    /// @{
private:
    /// @}

};  // class V8EngineFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
