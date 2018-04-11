
#include "lisp/common.h"
#include "lisp/library/libraries.h"
#include "prelude.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace craft::lisp::library;
using namespace craft::lisp::library::helper;

namespace _impl {
#ifdef _WIN32
	std::string GetLastErrorAsString()
	{
		//Get the error message, if any.
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0)
			return std::string(); //No error message has been recorded

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		//Free the buffer.
		LocalFree(messageBuffer);

		return message;
	}
#endif
}

namespace craft {
namespace lisp {
namespace library
{
	class SSubprocess
		: public craft::types::Aspect
	{
		CRAFT_LISP_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::lisp::library::SSubprocess, "lisp.library.subprocess", types::FactoryAspectManager);


		virtual void exec(instance<std::string> s) = 0;
		virtual instance<bool> running() = 0;
		virtual void kill() = 0;
		virtual instance<std::string> read(instance<int64_t> bytes) = 0;
		virtual instance<int64_t> write(instance<std::string> s) = 0;
		virtual void block(instance<int64_t> ms) = 0;
	};

#ifdef _WIN32
	class Subprocess
		: public virtual craft::types::Object
		, public types::Implements<SSubprocess>
	{
		CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Subprocess);
	private:
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		HANDLE _stdinRead = NULL;
		HANDLE _stdinWrite = NULL;
		HANDLE _stdoutRead = NULL;
		HANDLE _stdoutWrite = NULL;
	public:
		Subprocess()
		{
			memset(&si, 0, sizeof(si));
			memset(&pi, 0, sizeof(pi));
		}
		~Subprocess()
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		virtual void exec(instance<std::string> s) override
		{
			SECURITY_ATTRIBUTES saAttr;


			saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
			saAttr.bInheritHandle = TRUE;
			saAttr.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&_stdoutRead, &_stdoutWrite, &saAttr, 0))
			{
				throw stdext::exception("Failed to Create stdout Pipe: {0}", _impl::GetLastErrorAsString());
			}

			if (!SetHandleInformation(_stdoutRead, HANDLE_FLAG_INHERIT, 0))
			{
				throw stdext::exception("Failed to set stdout Pipe's security info {0}", _impl::GetLastErrorAsString());
			}

			if (!CreatePipe(&_stdinRead, &_stdinWrite, &saAttr, 0))
			{
				throw stdext::exception("Failed to Create stdin Pipe {0}", _impl::GetLastErrorAsString());
			}

			// Ensure the write handle to the pipe for STDIN is not inherited. 

			if (!SetHandleInformation(_stdinWrite, HANDLE_FLAG_INHERIT, 0))
			{
				throw stdext::exception("Failed to set stdin Pipe's security info {0}", _impl::GetLastErrorAsString());
			}
			
			si.cb = sizeof(STARTUPINFOA);
			si.hStdError = _stdoutWrite;
			si.hStdOutput = _stdoutWrite;
			si.hStdInput = _stdinRead;
			si.dwFlags |= STARTF_USESTDHANDLES;


			if (!CreateProcessA(NULL,   // No module name (use command line)
				(LPSTR)s->c_str(),        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				throw stdext::exception("Failed to Create Subprocess: {0}", _impl::GetLastErrorAsString());
			}

			return;
		}

		virtual instance<bool> running() override
		{
			DWORD exitCode;
			auto res = GetExitCodeProcess(pi.hProcess, &exitCode);
			return (res == 0) ? instance<bool>::make(false) : instance<bool>::make(exitCode == STILL_ACTIVE);
		}

		virtual void kill() override
		{
			TerminateProcess(pi.hProcess, 0);
		}

		virtual void block(instance<int64_t> ms)
		{
			WaitForSingleObject(pi.hProcess, (*ms == 0) ? INFINITE : *ms);
		}


		virtual instance<std::string> read(instance<int64_t> bytes) override
		{
			std::string res(*bytes, '\0');
			DWORD dwRead;
			BOOL bSuccess;
			bSuccess = ReadFile(_stdoutRead, (char*)res.data(), *bytes, &dwRead, NULL);
			if (bSuccess == false)
			{
				throw stdext::exception("Broken Read Pipe");
			}

			res.resize(dwRead);
			return instance<std::string>::make(std::move(res));
		}

		virtual instance<int64_t> write(instance<std::string> s) override
		{
			DWORD dwWritten;

			auto res = WriteFile(_stdinWrite, s->data(), s->size(), &dwWritten, NULL);
			if (!res) throw stdext::exception("Broken Write Pipe");

			return instance<int64_t>::make(dwWritten);
		}
	};
#else
  class Subprocess
    : public virtual craft::types::Object
    , public types::Implements<SSubprocess>
  {
    CRAFT_LISP_EXPORTED CRAFT_OBJECT_DECLARE(craft::lisp::library::Subprocess);
  private:
  public:
    Subprocess()
    {
      
    }
    ~Subprocess()
    {
    }

    virtual void exec(instance<std::string> s) override
    {
      throw stdext::exception("Not Implemented On Posix");

      return;
    }

    virtual instance<bool> running() override
    {
      throw stdext::exception("Not Implemented On Posix");
    }

    virtual void kill() override
    {
      throw stdext::exception("Not Implemented On Posix");
    }

    virtual void block(instance<int64_t> ms) override
    {
     throw stdext::exception("Not Implemented On Posix");
    }


    virtual instance<std::string> read(instance<int64_t> bytes) override
    {
      throw stdext::exception("Not Implemented On Posix");
    }

    virtual instance<int64_t> write(instance<std::string> s) override
    {
      throw stdext::exception("Not Implemented On Posix");
    }
  };
#endif
}}}



CRAFT_DEFINE(SSubprocess) { _.defaults(); }


CRAFT_DEFINE(Subprocess) { _.defaults(); }

void core::make_subprocess_globals(instance<Module> ret)
{
	auto semantics = ret->require<CultSemantics>();

	auto exec = instance<MultiMethod>::make();
	semantics->builtin_implementMultiMethod("subprocess/exec",
		[](instance<std::string> a) -> instance<Subprocess>
	{
		auto res = instance<Subprocess>::make();

		res->exec(instance<std::string>::make(path::normalize(*a)));
		return res;
	});

	semantics->builtin_implementMultiMethod("subprocess/running",
		[](instance<Subprocess> a) -> instance<bool>
	{
		return a->running();
	});

	semantics->builtin_implementMultiMethod("subprocess/block",
		[](instance<Subprocess> a, instance<int64_t> b)
	{
		a->block(b);
	});

	semantics->builtin_implementMultiMethod("subprocess/kill",
		[](instance<Subprocess> a)
	{
		a->kill();
	});

	semantics->builtin_implementMultiMethod("subprocess/read",
		[](instance<Subprocess> a, instance<int64_t> b) -> instance<std::string>
	{
		return a->read(b);
	});

	semantics->builtin_implementMultiMethod("subprocess/write",
		[](instance<Subprocess> a, instance<std::string> b) -> instance<int64_t>
	{
		return a->write(b);
	});
}
