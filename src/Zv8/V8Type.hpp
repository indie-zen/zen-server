//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#pragma once

#include <Zen/Scripting/I_ScriptEngine.hpp>

#include <memory>
#include <string>

#include <v8.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class V8Type
: public Zen::Scripting::I_ScriptType
{
    /// @name I_ScriptType implementation
    /// @{
public:
    virtual pScriptModule_type getScriptModule();
    virtual const std::string& getTypeName();
    virtual const std::string& getDocumentation();

    virtual void addMethod(const std::string& _name, const std::string& _docString, void_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, void_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, object_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, object_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, string_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, string_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, bool_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, bool_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, int_function_no_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, int_function_args_type _function);
    virtual void addMethod(const std::string& _name, const std::string& _docString, Scripting::I_ScriptMethod* _function);
    virtual void activate();
    /// @}

    /// @name Additional Implementation
    /// @{
public:
    v8::Local<v8::ObjectTemplate>   getRawType();
    const std::string&              getName() const;
    V8Module&                       getModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             V8Type(V8Module* _pModule, const std::string& _name, const std::string& _docString, unsigned long _rawSize);
    virtual ~V8Type();
    /// @}

    /// @name Member Variables
    /// @{
private:
    V8Module*               m_pModule;      ///< Module
    std::string             m_name;         ///< Class Name
    std::string             m_docString;    ///< Document String
    std::string             m_fullName;     ///< Full Class Name including module
    
    v8::Persistent<v8::ObjectTemplate>  m_type;

};  // class V8Type

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

