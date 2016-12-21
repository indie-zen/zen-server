//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "V8EngineFactory.hpp"
#include "V8Engine.hpp"

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <stdlib.h>

#include <stdexcept>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8EngineFactory::V8EngineFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8EngineFactory::~V8EngineFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8EngineFactory::pScriptEngine_type
V8EngineFactory::create(const std::string& _name)
{
    if (_name == "javasript")
    {
        // This will do the right thing, including replacement for
        // managed self reference (using enable_shared_from_this)
        pScriptEngine_type pService = std::make_shared<V8Engine>();

        // TODO Figure out how to make the deleter be onDestroy the way
        // managed pointers were.
        // (pRawEngine, boost::bind(&V8EngineFactory::onDestroy,this,_1));

        return pService;
    }

    return pScriptEngine_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8EngineFactory::onDestroy(wpScriptEngine_type _pScriptEngine)
{
    /// Fire the ScriptEngine's onDestroyEvent
    // _pScriptEngine->onDestroyEvent(_pScriptEngine);

    // Delete the ScriptEngine
    V8Engine* pScriptEngine = dynamic_cast<V8Engine*>(_pScriptEngine.lock().get());

    if( pScriptEngine )
    {
        delete pScriptEngine;
    }
    else
    {
        throw std::runtime_error("Zen::Zv8::V8EngineFactory::onDestroy() : _pScriptEngine is an invalid V8Engine.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static V8EngineFactory  sm_factory;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8EngineFactory&
V8EngineFactory::getSingleton()
{
    return sm_factory;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
