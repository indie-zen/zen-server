//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Server
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/Scripting.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Console
:   public Zen::Scripting::I_ScriptableService
,   public std::enable_shared_from_this<Console>
{
public:
    typedef Console*                                        pScriptObject_type;
    typedef Zen::Scripting::ScriptWrapper<Console>          ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;

    /// @name I_Scriptable implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ScriptWrapper* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation
    /// @{
public:
    /// @todo should the meta data be separated from the implementation?
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name Console
    /// @{
public:
    /// Implementation of managed_self_ref from Zen 1.x
    /// The biggest difference is getSelfReference now returns a shared_ptr
    /// instead of a weak pointer, so getSelfReference().lock() is no longer
    /// necessary; instead use getSelfReference().
    std::shared_ptr<Console> getSelfReference();

    /// Get the script module where this class was defined
    /// Used by getScriptObject()
    pScriptModule_type getScriptModule();

    void print(const std::string& _value);
    /// @}

public:
             Console();
    virtual ~Console();
    // Not copyable nor assignable
    Console(const Console&) = delete;
    void operator=(const Console&) = delete;

    /// @name Member variables
    /// @{     
private:
    pScriptEngine_type                          m_pScriptEngine;
    Zen::Scripting::script_module*              m_pScriptModule;
    Zen::Scripting::I_ScriptWrapper*            m_pScriptObject;

    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
