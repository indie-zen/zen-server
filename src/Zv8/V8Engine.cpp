#include "V8Engine.hpp"

#include <stdexcept>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Zv8 {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Engine::V8Engine()
:   m_pGlobalScope(nullptr)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Engine::~V8Engine()
{
    if(m_pGlobalScope)
    {
        delete m_pGlobalScope;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Engine::initialize(pConfiguration_type _pConfiguration)
{
    Configuration_type& config = *_pConfiguration;
    std::string arg0 = config["arg0"];
    
    // TODO Load script engine based on scripting language command line argument
    v8::V8::InitializeICUDefaultLocation(arg0);
    v8::V8::InitializeExternalStartupData(arg0);
    m_pPlatform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
    
    // Create a new Isolate and make it the current one.
    // TODO We should probably support a new isolate and mulitple contexts
    // but for now just one of each.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    m_pIsolate = v8::Isolate::New(create_params);

    // New global scope
    m_pGlobalScope = new v8::Isolate::Scope(m_pIsolate);

    // Is this necessary?    
    // Create a stack-allocated handle scope.
    // v8::HandleScope scope(m_pIsolate);

    // New global object
    m_pGlobalObject = v8::ObjectTemplate::New(m_pIsolate);

    m_pGlobalObject->Set(
      v8::String::NewFromUtf8(isolate, "print", v8::NewStringType::kNormal)
          .ToLocalChecked(),
      v8::FunctionTemplate::New(m_pIsolate, print));

    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, m_pGlobalObject);

    // Enter the context for compiling and running the script.
    v8::Context::Scope context_scope(context);

    if(isolate == nullptr) 
    {
        std::cout << "Error creating isolate." << std::endl;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectHeap&
V8Engine::heap()
{
    throw std::runtime_error("bool::heap(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
V8Engine::executeScript(const std::string& _fileName)
{
    throw std::runtime_error("bool::executeScript(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Engine::executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms)
{
    throw std::runtime_error("bool::executeMethod(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Engine::pScriptModule_type
V8Engine::createScriptModule(const std::string& _moduleName, const std::string& _docString)
{
    throw std::runtime_error("bool::createScriptModule(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
