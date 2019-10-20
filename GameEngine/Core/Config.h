#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Core.h"
#include "Any.h"
#include <map>

namespace ge
{
	class Config 
	{
		String				  m_file;
		std::map<String, Any> m_values;
	public:
		static void setCurrentConfig(Ptr<Config> config);
		static Config& instance();

		Config(Config&&) = delete;
		Config& operator=(Config&&) = delete;

		Config(const Config&) = delete;
		Config& operator=(const Config&) = delete;

		void save() const;
		~Config();
		Config(const String& file);

		void setValueInt64(int64 value, const String& name);
		void setValueString(const String& value, const String& name);
		void setValueDouble(double value, const String& name);
		void setBoolDouble(bool value, const String& name);

		int64 getValueInt64(const String name, int64 def);
		String getValueString(const String name, const String& def);
		double getValueDouble(const String name, double def);
		bool getValueBool(const String name, bool def);

		String toString() const;
	};
}

#endif