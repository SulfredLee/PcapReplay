#include "LogMgr.h"

//LogMgr::LogMgr()
//{
//	time_t t = time(0);   // get time now
//	struct tm * now = new tm;
//	localtime_s(now, &t);
//	std::string today = std::to_string(now->tm_year + 1900) + "_"
//		+ std::to_string(now->tm_mon + 1) + "_"
//		+ std::to_string(now->tm_mday);
//	delete now;
//
//	// New attributes that hold filename and line number
//	boost::log::core::get()->add_global_attribute("File", boost::log::attributes::mutable_constant<std::string>(""));
//	boost::log::core::get()->add_global_attribute("Line", boost::log::attributes::mutable_constant<int>(0));
//
//	boost::log::add_file_log
//		(
//		boost::log::keywords::file_name = today + ".log",                                        /*< file name pattern >*/
//		boost::log::keywords::rotation_size = 10 * 1024 * 1204,                                 /*< rotate files every 10 MiB... >*/
//		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), /*< ...or at midnight >*/
//		boost::log::keywords::format =
//		(
//		boost::log::expressions::stream
//		<< boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d_%H:%M:%S.%f")
//		<< ": <" << boost::log::trivial::severity << "> "
//		<< '[' << boost::log::expressions::attr<std::string>("File")
//		<< ':' << boost::log::expressions::attr<int>("Line") << "] "
//		<< boost::log::expressions::smessage
//		)
//		);
//	boost::log::add_common_attributes();
//}
//
//
//LogMgr::~LogMgr()
//{
//}
//
//void LogMgr::PrintLog(boost::log::trivial::severity_level lv, const char* format, ...)
//{
//	va_list arg;
//	char buffer[2048];
//	//int done;
//
//	va_start(arg, format);
//	//done = vfprintf(stdout, format, arg);
//	vsprintf_s(buffer, format, arg);
//	va_end(arg);
//
//	std::string str(buffer);
//	//BOOST_LOG_SEV(m_lg, lv) << str;
//	CUSTOM_LOG(m_lg, lv) << str;
//}