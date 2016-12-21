#include <iostream>
#include <map>
#include <string>

#include "Zv8/V8EngineFactory.hpp"

// TODO This will eventually go away once we've fully switched over to the 
// Zen Scripting framework
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
// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
void
print(const v8::FunctionCallbackInfo<v8::Value>& args) {
  bool first = true;
  for (int i = 0; i < args.Length(); i++) {
    v8::HandleScope handle_scope(args.GetIsolate());
    if (first) {
      first = false;
    } else {
      printf(" ");
    }
    v8::String::Utf8Value str(args[i]);
    const char* cstr = toCString(str);
    printf("%s", cstr);
  }
  printf("\n");
  fflush(stdout);
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

        returnVal = v8::String::NewFromUtf8(_pIsolate, pChars, 
            v8::NewStringType::kNormal, static_cast<int>(size));

        delete[] pChars;
        fclose(pFile);
    }

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
    v8::HandleScope        handleScope(_pIsolate);
    v8::TryCatch           tryCatch(_pIsolate);
    v8::ScriptOrigin       origin(_name);
    v8::Local<v8::Context> context(_pIsolate->GetCurrentContext());
    v8::Local<v8::Script>  script;
    
    if (!v8::Script::Compile(context, _source, &origin).ToLocal(&script)) 
    {
        std::cout << "Exception occured while compiling script" << std::endl;
        if (_reportExecptions)
        {
            reportException(_pIsolate, &tryCatch);
        }
        return false;
    }
    v8::Local<v8::Value> result;
    if (!script->Run(context).ToLocal(&result)) 
    {
        std::cout << "Exception occured while running script" << std::endl;
        if (_reportExecptions)
        {
            reportException(_pIsolate, &tryCatch);
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
int
runScript(v8::Platform* _pPlatform, v8::Isolate* _pIsolate, 
    const char* _pScriptName)
{
    v8::Local<v8::String> file_name =
        v8::String::NewFromUtf8(_pIsolate, _pScriptName, v8::NewStringType::kNormal)
            .ToLocalChecked();
            
    v8::Local<v8::String> source;
    if (!readFile(_pIsolate, _pScriptName).ToLocal(&source))
    {
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
    std::cout << _argv[0] << std::endl;
    
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
    // TODO this will need to change once I_ScriptEngine uses I_Configuration
    // instead of map<string, string>.
    std::map<std::string, std::string> configuration;
    configuration["arg0"] = _argv[0];

    auto pScriptEngine = Zen::Zv8::V8EngineFactory::getSingleton().create("javasript");
    
    pScriptEngine->initialize(&configuration);

    // return runScript(platform, isolate, startupScript.c_str());
    
    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
