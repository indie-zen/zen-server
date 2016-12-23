//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/Scripting.hpp>

#include <memory>

#include <v8.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class V8Engine;
class V8Type;

class V8Module
:   public Zen::Scripting::I_ScriptModule
{
    /// @name Types
    /// @{
public:
    typedef std::shared_ptr<Zen::Scripting::I_ScriptEngine>             pScriptEngine_type;
    // typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>        pScriptType_type;
    // typedef Zen::Memory::managed_weak_ptr<Scripting::I_ScriptType>   wpScriptType_type;

    // typedef Zen::Memory::managed_ptr<Scripting::I_ScriptObject>      pScriptObject_type;
    // typedef Zen::Memory::managed_weak_ptr<Scripting::I_ScriptObject> wpScriptObject_type;

    /// The script wrapper provides a facade for any C++ object.
    /// This allows a C++ object to be used as a payload associated with a script object
    /// and then methods intercepted by the scripting engine can be directed to the C++
    /// object.
    typedef Zen::Scripting::I_ScriptWrapper*                            pObjectWrapper_type;

    // typedef std::map<std::string, pScriptType_type>                  type_collection_type;
    /// @}
    
    /// @name I_ScriptModule implementation
    /// @{
public:
    virtual pScriptType_type createScriptType(const std::string& _typeName, const std::string& _docString, unsigned long _rawSize);
    virtual pScriptType_type getScriptType(const std::string& _typeName);
    virtual void activate();
    virtual void createObject(pScriptType_type _pType, pObjectWrapper_type _pObject);
    virtual void createGlobalObject(const std::string& _name, pScriptType_type _pType, pObjectWrapper_type _pObject);
    virtual pScriptEngine_type getScriptEngine();
    /// @}

    /// @name 'Structors
    /// @{
public:
             V8Module(V8Engine* _pEngine, const std::string& _name, const std::string& _docString);
    virtual ~V8Module();
    /// @}

    /// @name Member Variables
    /// @{
private:
    V8Engine*       m_pEngine;
    std::string     m_name;
    std::string     m_docString;
    /// @}
    
};  // class V8Module

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
