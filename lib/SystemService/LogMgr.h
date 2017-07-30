#ifndef LOGMGR_H
#define LOGMGR_H
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

#include <string>
#include <ctime>



class LogMgr
{
protected:
	boost::log::sources::severity_logger< boost::log::trivial::severity_level > m_lg;

	// New macro that includes severity, filename and line number
#define CUSTOM_LOG(logger, sev) \
   BOOST_LOG_STREAM_WITH_PARAMS( \
      (logger), \
         (set_get_attrib("File", path_to_filename(__FILE__))) \
         (set_get_attrib("Line", __LINE__)) \
         (::boost::log::keywords::severity = (sev)) \
   )

	// Set attribute and return the new value
	template<typename ValueType>
	ValueType set_get_attrib(const char* name, ValueType value) {
		auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(boost::log::core::get()->get_global_attributes()[name]);
		attr.set(value);
		return attr.get();
	}

	// Convert file path to only the filename
	std::string path_to_filename(std::string path) {
		return path.substr(path.find_last_of("/\\") + 1);
	}
public:
	LogMgr();
	~LogMgr();

	void PrintLog(boost::log::trivial::severity_level lv, const char* format, ...);
};
#endif
