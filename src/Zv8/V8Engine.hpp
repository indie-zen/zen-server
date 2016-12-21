//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Python plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/I_ScriptEngine.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class V8Engine
:   public Zen::Scripting::I_ScriptEngine
{
    /// @name I_ScriptEngine implementation
    /// @{
public:
    virtual void initialize(pConfiguration_type _pConfiguration = nullptr);
    virtual Scripting::I_ObjectHeap& heap();
    virtual bool executeScript(const std::string& _fileName);
    virtual void executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms);
    virtual pScriptModule_type createScriptModule(const std::string& _moduleName, const std::string& _docString);
    /// @}

    /// @name V8Engine implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             V8Engine();
    virtual ~V8Engine();
    /// @}

    /// @name Member variables
    /// @{
private:
    v8::Platform*   m_pPlatform;
    v8::Isolate*    m_pIsolate; 
    /// @}
    
};  // class V8Engine

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
