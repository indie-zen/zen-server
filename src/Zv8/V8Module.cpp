//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "V8Module.hpp"
#include "V8Engine.hpp"
#include "V8Type.hpp"

#include <stdexcept>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Module::V8Module(V8Engine* _pEngine, const std::string& _name, const std::string& _docString)
:   m_pEngine(_pEngine)
,   m_name(_name)
,   m_docString(_docString)
{
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Module::~V8Module()
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Module::pScriptType_type
V8Module::createScriptType(const std::string& _typeName, 
    const std::string& _docString, unsigned long _rawSize)
{
    V8Type* const pV8Type = new V8Type(this, _typeName, _docString, _rawSize);

    m_module.Set(
      v8::String::NewFromUtf8(getIsolate(), _typeName, v8::NewStringType::kNormal)
          .ToLocalChecked(),
      pV8Type->getRawType());

    pScriptType_type const pType(pV8Type);
    
    m_scriptTypes[_typeName] = pType;

    return pType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Module::pScriptType_type
V8Module::getScriptType(const std::string& _typeName)
{
    return m_scriptTypes[_typeName];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Module::activate()
{
    throw std::runtime_error("V8Module::activate(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Module::createObject(pScriptType_type _pType, pObjectWrapper_type _pObject)
{
    throw std::runtime_error("V8Module::createObject(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Module::createGlobalObject(const std::string& _name, pScriptType_type _pType, 
    pObjectWrapper_type _pObject)
{
    throw std::runtime_error("V8Module::createGlobalObject(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Module::pScriptEngine_type
V8Module::getScriptEngine()
{
    return m_pEngine->getSelfReference();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
v8::Isolate*
V8Module::getIsolate()
{
    return m_pEngine->getIsolate();    
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
