#include "IO.h"
#include <fstream>

namespace ge
{
	Vector<byte> FileUtils::readAllContents(const String& path)
	{
		std::ifstream file(std::string(path.begin(), path.end()), std::ios::binary);
		if (!file.is_open())
			return {};
		file.seekg(0, std::ios::end);
		usize length = file.tellg();
		file.seekg(0, std::ios::beg);
		Vector<byte> data(length);
		file.read(reinterpret_cast<char*>(data.data()), length);
		return std::move(data);
	}

	void FileUtils::writeAllContents(const Vector<byte>& data, const String& path)
	{
		std::ofstream file(std::string(path.begin(), path.end()), std::ios::binary);
		file.write(reinterpret_cast<const char*>(data.data()), data.size());
	}
}
