#include "Config.h"
#include "IO.h"
#include "Serializer.h"

namespace ge 
{
	Ptr<Config> currentConfig;
	void Config::setCurrentConfig(Ptr<Config> config)
	{
		currentConfig = config;
	}

	Config& Config::instance()
	{
		return *currentConfig;
	}

	void Config::save() const
	{
		MemoryWriter writer;
		writer.writeString(u"BCFG");
		writer.write<uint64>(m_values.size());
		for (auto& x : m_values)
		{
			writer.writeString(x.first);
			if (x.second.is<bool>()) 
			{
				writer.write<uint8>(0);
				writer.write<bool>(x.second.get<bool>());
			}
			if (x.second.is<int64>())
			{
				writer.write<uint8>(1);
				writer.write<int64>(x.second.get<int64>());
			}
			if (x.second.is<double>())
			{
				writer.write<uint8>(2);
				writer.write<double>(x.second.get<double>());
			}
			if (x.second.is<String>())
			{
				writer.write<uint8>(3);
				writer.writeString(x.second.get<String>());
			}
		}
		FileUtils::writeAllContents(writer.get(), m_file);
	}

	Config::~Config()
	{
		save();
	}

	Config::Config(const String& file) :
		m_file(file)
	{
		auto fileData = FileUtils::readAllContents(file);
		if (!fileData.size())
			return;
		MemoryReader reader(fileData);
		String header = reader.readString();
		if (header != u"BCFG")
			geAssertFalse("Invalid config file format.");
		uint64 propertyCount = reader.read<uint64>();
		for (uint64 i = 0; i < propertyCount; i++) 
		{
			String propertyName = reader.readString();
			uint8 type = reader.read<uint8>();
			if (type == 0) 
			{
				bool value = reader.read<bool>();
				m_values[propertyName] = value;
			}
			else if (type == 1)
			{
				int64 value = reader.read<int64>();
				m_values[propertyName] = value;
			}
			else if (type == 2)
			{
				double value = reader.read<double>();
				m_values[propertyName] = value;
			}
			else if (type == 3)
			{
				String value = reader.readString();
				m_values[propertyName] = value;
			} else 
			{
				geAssertFalse("Invalid binary type format.");
			}
		}
	}

	void Config::setValueInt64(int64 value, const String& name)
	{
		m_values[name] = value;
	}

	void Config::setValueString(const String& value, const String& name)
	{
		m_values[name] = value;
	}

	void Config::setValueDouble(double value, const String& name)
	{
		m_values[name] = value;
	}

	void Config::setBoolDouble(bool value, const String& name)
	{
		m_values[name] = value;
	}

	int64 Config::getValueInt64(const String name, int64 def)
	{
		Any& value = m_values[name];
		if (value.is<int64>())
			return value.get<int64>();
		return def;
	}

	String Config::getValueString(const String name, const String& def)
	{
		Any& value = m_values[name];
		if (value.is<String>())
			return value.get<String>();
	}

	double Config::getValueDouble(const String name, double def)
	{
		Any& value = m_values[name];
		if (value.is<double>())
			return value.get<double>();
	}

	bool Config::getValueBool(const String name, bool def)
	{
		Any& value = m_values[name];
		if (value.is<bool>())
			return value.get<bool>();
	}

	String Config::toString() const
	{
		String result;
		return result;
	}
}
