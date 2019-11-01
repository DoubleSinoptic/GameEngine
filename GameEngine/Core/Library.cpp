#include "Library.h"
#ifdef _WIN32
#	include <Windows.h>
#else
#	error NotImpl
#endif
namespace ge 
{
	bool Library::isLoad() const noexcept
	{
#		ifdef _WIN32
			HMODULE selectedModule = m_module.get<HMODULE>();
#		else
			geAssertFalse("Not impl.");
#		endif		
		return selectedModule;
	}

	Library::Library(const String& title) noexcept :
#		ifdef _WIN32
			m_module(::LoadLibraryW((const wchar_t*)title.c_str()))
#		else
			m_module(nullptr)
#		endif
	{}

	void* Library::getProcessAddressPtr(const String& path) const noexcept
	{
#		ifdef _WIN32
			HMODULE selectedModule = m_module.get<HMODULE>();
			geAssert(selectedModule);
			::std::string symbol(path.begin(), path.end());
			FARPROC proc = GetProcAddress(selectedModule, symbol.c_str());
			if (!proc)
				return nullptr;
#		else
			geAssertFalse("Not impl.");
#		endif	
		return proc;
	}


	class s 
	{
	public:
		enum class sf {
			A,
			B,
			C
		} s;
		void f() 
		{
			s = sf::A;
		}


	
	
	};

}
