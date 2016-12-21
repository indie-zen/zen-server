//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// V8 plugin for Zen Scripting
//
// Copyright (C) 2001 - 2016 Raymond A. Richards
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "V8Engine.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#include <libplatform/libplatform.h>

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
    std::cout << "initialize" << std::endl;

    Configuration_type& config = *_pConfiguration;
    const char* arg0 = config["arg0"].c_str();
    
    // TODO Load script engine based on scripting language command line argument
    v8::V8::InitializeICUDefaultLocation(arg0);
    v8::V8::InitializeExternalStartupData(arg0);
    m_pPlatform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(m_pPlatform);
    v8::V8::Initialize();
    
    // Create a new Isolate and make it the current one.
    // TODO We should probably support a new isolate and mulitple contexts
    // but for now just one of each.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();

    m_pIsolate = v8::Isolate::New(create_params);

    std::cout << "Creating global scope" << std::endl;
    
    // New global scope
    m_pGlobalScope = new v8::Isolate::Scope(m_pIsolate);

    std::cout << "Creating handle scope" << std::endl;
    // Is this necessary?    
    // Create a stack-allocated handle scope.
    // m_pHandleScope = new v8::HandleScope(m_pIsolate);
    v8::HandleScope scope(m_pIsolate);

    std::cout << "Creating global object" << std::endl;
    
    // New global object
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(m_pIsolate);

    // m_global->Set(
    //   v8::String::NewFromUtf8(m_pIsolate, "print", v8::NewStringType::kNormal)
    //       .ToLocalChecked(),
    //   v8::FunctionTemplate::New(m_pIsolate, print));

    // TODO Should the context and context_scope be part of "this"?
    v8::Local<v8::Context> context = v8::Context::New(m_pIsolate, nullptr, global);
    m_context.Reset(m_pIsolate, context);

    if(m_pIsolate == nullptr) 
    {
        // TODO Thrown an exception instead of just returning?
        std::cout << "Error creating isolate." << std::endl;
        return;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectHeap&
V8Engine::heap()
{
    throw std::runtime_error("V8Engine::heap(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
V8Engine::executeScript(const std::string& _fileName)
{
    std::cout << "executeScript" << std::endl;
    v8::HandleScope handle_scope(m_pIsolate);
    
    std::cout << "creating context_scope" << std::endl;

    // Enter the context for compiling and running the script.
    v8::Context::Scope context_scope(m_context.Get(m_pIsolate));

    std::cout << "getting file name" << std::endl;
    v8::Local<v8::String> fileName =
        v8::String::NewFromUtf8(m_pIsolate, _fileName.c_str(), v8::NewStringType::kNormal)
            .ToLocalChecked();

    std::cout << "Reading source" << std::endl;
    v8::Local<v8::String> source;
    if (!this->readFile(_fileName).ToLocal(&source))
    {
        // Error reading file 
        // TODO Throw exception?
        return false;
    }
    std::cout << "Executing string" << std::endl;
    bool success = this->executeString(source, fileName, true, true);
    while (v8::platform::PumpMessageLoop(m_pPlatform, m_pIsolate)) continue;
    return success;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
V8Engine::executeMethod(boost::any& _object, boost::any& _method, std::vector<boost::any>& _parms)
{
    throw std::runtime_error("V8Engine::executeMethod(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
V8Engine::pScriptModule_type
V8Engine::createScriptModule(const std::string& _moduleName, const std::string& _docString)
{
    throw std::runtime_error("V8Engine::createScriptModule(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::shared_ptr<V8Engine>
V8Engine::getSelfReference()
{
    return shared_from_this();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
V8Engine::executeString(v8::Local<v8::String> _source, v8::Local<v8::Value> _name, 
        bool _printResult, bool _reportExecptions)
{
    std::cout << "executeString" << std::endl;

    v8::HandleScope        handleScope(m_pIsolate);
    v8::TryCatch           tryCatch(m_pIsolate);
    v8::ScriptOrigin       origin(_name);
    v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
    v8::Local<v8::Script>  script;
    
    if (!v8::Script::Compile(context, _source, &origin).ToLocal(&script)) 
    {
        std::cout << "Exception occured while compiling script" << std::endl;
        if (_reportExecptions)
        {
            this->reportException(&tryCatch);
        }
        return false;
    }
    v8::Local<v8::Value> result;
    if (!script->Run(context).ToLocal(&result)) 
    {
        std::cout << "Exception occured while running script" << std::endl;
        if (_reportExecptions)
        {
            reportException(&tryCatch);
        }
        return false;
    }
    std::cout << "Got results" << std::endl;
    if (_printResult && !result->IsUndefined()) 
    {
        v8::String::Utf8Value str(result);
        const char* cstr = toCString(str);
        std::cout << cstr << std::endl;
    }
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
v8::MaybeLocal<v8::String> 
V8Engine::readFile(const std::string& _scriptName)
{
    std::cout << "readFile" << std::endl;

    auto returnVal = v8::MaybeLocal<v8::String>();
    FILE* pFile = fopen(_scriptName.c_str(), "rb");
    if (pFile != NULL)
    {
        fseek(pFile, 0, SEEK_END);
        size_t size = ftell(pFile);
        rewind(pFile);
        
        char* pChars = new char[size + 1];
        if(!pChars)
        {
            // Out of memory or something else bad happened
            return returnVal;
        }
        pChars[size] = '\0';
        for(size_t i = 0; i < size;) 
        {
            i += fread(&pChars[i], 1, size - i, pFile);
            if (ferror(pFile))
            {
                fclose(pFile);
                delete[] pChars;
                return returnVal;
            }
        }

        returnVal = v8::String::NewFromUtf8(m_pIsolate, pChars, 
            v8::NewStringType::kNormal, static_cast<int>(size));

        delete[] pChars;
        fclose(pFile);
    }

    return returnVal;
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
V8Engine::reportException(v8::TryCatch* _pTryCatch)
{
    std::cout << "reportException" << std::endl;

    v8::HandleScope handle_scope(m_pIsolate);
    v8::String::Utf8Value exception(_pTryCatch->Exception());
    const char* pExceptionString = toCString(exception);
    v8::Local<v8::Message> message = _pTryCatch->Message();
    if (message.IsEmpty()) 
    {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        std::cout << pExceptionString << std::endl;
    } 
    else 
    {
        // Print (filename):(line number): (message).
        v8::String::Utf8Value filename(message->GetScriptOrigin().ResourceName());
        v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
        int lineNum = message->GetLineNumber(context).FromJust();
        const char* pFileName = toCString(filename);
        int linenum = message->GetLineNumber(context).FromJust();
        std::cout << pFileName << ":" << lineNum << ":" << pExceptionString << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const char* 
V8Engine::toCString(const v8::String::Utf8Value& value) 
{
  return *value ? *value : "<string conversion failed>";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zv8
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
