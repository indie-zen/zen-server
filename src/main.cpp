#include <string>
#include <iostream>

#include <v8.h>
#include <libplatform/libplatform.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#define ERR_NO_COMMAND_LINE         1
#define ERR_LOADING_STARTUP_SCRIPT  2
#define ERR_EXECUTE_SCRIPT          4

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const char* 
toCString(const v8::String::Utf8Value& value) 
{
  return *value ? *value : "<string conversion failed>";
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
reportException(v8::Isolate* _pIsolate, v8::TryCatch* _pTryCatch)
{
    v8::HandleScope handle_scope(_pIsolate);
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
        v8::Local<v8::Context> context(_pIsolate->GetCurrentContext());
        int lineNum = message->GetLineNumber(context).FromJust();
        const char* pFileName = toCString(filename);
        int linenum = message->GetLineNumber(context).FromJust();
        std::cout << pFileName << ":" << lineNum << ":" << pExceptionString << std::endl;
    }
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
v8::MaybeLocal<v8::String>
readFile(v8::Isolate* _pIsolate, const std::string& _scriptName)
{
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
        std::cout << "In readFile" << std::endl;
        std::cout << pChars << std::endl;
        returnVal = v8::String::NewFromUtf8(_pIsolate, pChars, 
            v8::NewStringType::kNormal, static_cast<int>(size));
        delete[] pChars;
        fclose(pFile);
    }
    
    std::cout << "Returning from readFile" << std::endl;
    return returnVal;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
executeString(v8::Isolate* _pIsolate, 
    v8::Local<v8::String> _source,
    v8::Local<v8::Value> _name, 
    bool _printResult, 
    bool _reportExecptions)
{
    std::cout << "in executeString" << std::endl;
    v8::HandleScope        handleScope(_pIsolate);
    v8::TryCatch           tryCatch(_pIsolate);
    v8::ScriptOrigin       origin(_name);
    v8::Local<v8::Context> context(_pIsolate->GetCurrentContext());
    v8::Local<v8::Script>  script;
    
    std::cout << "Compiling..." << std::endl;
    if (!v8::Script::Compile(context, _source, &origin).ToLocal(&script)) 
    {
        std::cout << "Error compiling" << std::endl;
        if (_reportExecptions)
        {
            reportException(_pIsolate, &tryCatch);
        }
        std::cout << "Exception occured while compiling script" << std::endl;
        return false;
    }
    v8::Local<v8::Value> result;
    std::cout << "Running..." << std::endl;
    if (!script->Run(context).ToLocal(&result)) 
    {
        if (_reportExecptions)
        {
            reportException(_pIsolate, &tryCatch);
        }
        std::cout << "Exception occured while running script" << std::endl;
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
int
runScript(v8::Platform* _pPlatform, v8::Isolate* _pIsolate, 
    const char* _pScriptName)
{
    std::cout << "In runScript" << std::endl;
    v8::Local<v8::String> file_name =
        v8::String::NewFromUtf8(_pIsolate, _pScriptName, v8::NewStringType::kNormal)
            .ToLocalChecked();
            
    v8::Local<v8::String> source;
    if (!readFile(_pIsolate, _pScriptName).ToLocal(&source))
    {
        std::cerr << "Error reading " << _pScriptName << std::endl;
        return ERR_LOADING_STARTUP_SCRIPT;
    }
    bool success = executeString(_pIsolate, source, file_name, true, true);
    while (v8::platform::PumpMessageLoop(_pPlatform, _pIsolate)) continue;
    if (!success) 
    {
        return ERR_EXECUTE_SCRIPT;    
    }
    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
main(int _argc, char** _argv) 
{
    std::cout << "Welcome to Zen Server Daemon" << std::endl;

    // Declare the supported options.
    po::options_description description("Allowed options");

    std::string startupScript;

    description.add_options()
        (
            "help,h",
            "Produce this help message"
        )
        (
            "script",
            po::value<std::string>(&startupScript),
            "Startup script file"
        )
        //("lang,l", po::value<std::string>(&m_scriptLanguage)->default_value("python"), "Script language extension")
        //("script,s", po::value<std::string>(&m_defaultScript)->default_value("initServer.py"), "Boot script")
        //("interactive,i", po::value<bool>(&m_bRunInteractive)->default_value(false), "Run interactively")
    ;

    po::positional_options_description positionalDescription;
    positionalDescription.add("script", -1);

    po::variables_map variables;
    po::store(po::command_line_parser(_argc, _argv)
        .options(description).positional(positionalDescription).run(), variables);
    po::notify(variables);

    if (variables.size() == 0 || variables.count("help") == 1)
    {
        std::cout << std::endl << description << std::endl;
        return ERR_NO_COMMAND_LINE;
    }

    // TODO make sure startupScript exists.

    // Initialize V8 script engine 
    // TODO Load script engine based on scripting language command line argument
    v8::V8::InitializeICUDefaultLocation(_argv[0]);
    v8::V8::InitializeExternalStartupData(_argv[0]);
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
      
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope scope(isolate);

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);

    // Enter the context for compiling and running the script.
    v8::Context::Scope context_scope(context);

    if(isolate == nullptr) 
    {
        std::cout << "Error creating isolate." << std::endl;
    }
    return runScript(platform, isolate, startupScript.c_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
