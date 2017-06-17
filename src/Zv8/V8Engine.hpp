//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/I_ScriptEngine.hpp>

#include <memory>

#include <v8.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class V8Engine
:   public Zen::Scripting::I_ScriptEngine
,   public std::enable_shared_from_this<V8Engine>
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
    /// Implementation of managed_self_ref from Zen 1.x
    std::shared_ptr<V8Engine> getSelfReference();
    
    /// Execute a script that is embedded in a string
    /// @param _source - String to execute
    /// @param _name - Name of the string (usually the URI where the string
    ///     was loaded from)
    /// @param _printResult - output the results of the script to std::cout
    /// @param _reportExceptions - output an exception report if the script throws
    ///     an uncaught exception.
    bool executeString(v8::Local<v8::String> _source, v8::Local<v8::Value> _name, 
        bool _printResult, bool _reportExecptions);
    
    /// Read a file into a (maybe local) string
    /// @todo Zen Server should not support direct file access; instead, all
    ///     source must be loaded from a container local segment of Zen Spaces.
    ///     This is only being supported as a temporary measure until Zen Spaces
    ///     integration is complete.
    v8::MaybeLocal<v8::String> readFile(const std::string& _scriptName);

    /// Report an exception
    /// Output an exception to std::cout (maybe it should be std::cerr?)
    void reportException(v8::TryCatch* _pTryCatch);

    /// Convert a v8 string to a C string
    const char* toCString(const v8::String::Utf8Value& value); 

    v8::Isolate* getIsolate(); 
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
    v8::Platform*                   m_pPlatform;
    v8::Isolate*                    m_pIsolate; 
    v8::Isolate::Scope*             m_pGlobalScope;
    v8::HandleScope*                m_pHandleScope;

    // v8::Global<v8::ObjectTemplate>   m_global;
    v8::Global<v8::Context>         m_context;
    /// @}
    
};  // class V8Engine

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
