/*
Copyright (c) 2011 Christoph Stoepel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once
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

#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>
#include <chrono>
#include <ctime>

namespace Logging
{
	enum LOG_LEVEL
	{
		LOG_LEVEL_TRACE = 0,
		LOG_LEVEL_DEBUG = 1,
		LOG_LEVEL_INFO = 2,
		LOG_LEVEL_WARN = 3,
		LOG_LEVEL_ERROR = 4,
		LOG_LEVEL_FATAL = 5
	};

	class CLogMessage
	{
	public:
		std::string m_szMessage;

	public:
		CLogMessage(std::string szMsg) : m_szMessage(szMsg) { }
		CLogMessage(const CLogMessage& msg) : m_szMessage(msg.m_szMessage) { }
		CLogMessage(const char* szFormat, ...)
		{
			va_list args;
			char buffer[2048];
			va_start(args, szFormat);
			vsprintf_s(buffer, szFormat, args);
			//m_szMessage.FormatV(szFormat, args);
			va_end(args);
			m_szMessage = buffer;
		}

		//operator LPCTSTR() const { return (LPCTSTR)m_szMessage; }
		CLogMessage& operator=(const CLogMessage& msg) { m_szMessage = msg.m_szMessage; return *this; }
		bool operator==(const CLogMessage& msg) const { return std::strcmp(m_szMessage.c_str(), msg.m_szMessage.c_str()) == 0; }
	};

	class ILogTarget
	{
	public:
		virtual ~ILogTarget() { }
		virtual bool IsEnabled(LOG_LEVEL lvl) = 0;
		virtual void Append(std::string szMsg) = 0;
	};

	class CLogTargetBase : public ILogTarget
	{
	protected:
		LOG_LEVEL m_nLevel;

	public:
		CLogTargetBase(LOG_LEVEL lvl) : m_nLevel(lvl) { }
		virtual ~CLogTargetBase() {}
		virtual bool IsEnabled(LOG_LEVEL lvl) { return m_nLevel <= lvl; }
	};

	class CLogTargetDebugger : public CLogTargetBase
	{
	public:
		CLogTargetDebugger(LOG_LEVEL lvl) : CLogTargetBase(lvl) { }
		virtual ~CLogTargetDebugger() { }
		//virtual void Append(std::string szMsg) { ::OutputDebugString(szMsg); }
		virtual void Append(std::string szMsg)
		{
			switch (m_nLevel)
			{
			case Logging::LOG_LEVEL_TRACE:
				BOOST_LOG_TRIVIAL(trace) << szMsg;
				break;
			case Logging::LOG_LEVEL_DEBUG:
				BOOST_LOG_TRIVIAL(debug) << szMsg;
				break;
			case Logging::LOG_LEVEL_INFO:
				BOOST_LOG_TRIVIAL(info) << szMsg;
				break;
			case Logging::LOG_LEVEL_WARN:
				BOOST_LOG_TRIVIAL(warning) << szMsg;
				break;
			case Logging::LOG_LEVEL_ERROR:
				BOOST_LOG_TRIVIAL(error) << szMsg;
				break;
			case Logging::LOG_LEVEL_FATAL:
				BOOST_LOG_TRIVIAL(fatal) << szMsg;
				break;
			default:
				break;
			}
		}
	};

	//class CLogTargetMessageBox : public CLogTargetBase
	//{
	//public:
	//	CLogTargetMessageBox(LOG_LEVEL lvl) : CLogTargetBase(lvl) { }
	//	virtual ~CLogTargetMessageBox() { }
	//	virtual void Append(std::string szMsg) { ::AtlMessageBox(NULL, szMsg); }
	//};

	class CLogger
	{
	private:
		int m_dwLogStart;
		std::vector<ILogTarget*> m_pTargets;

	public:
		CLogger()
		{
			m_dwLogStart = getTickCount();
		}
		~CLogger()
		{
			for (size_t i = 0; i<m_pTargets.size(); i++)
				delete m_pTargets[i];
			m_pTargets.clear();
		}

		void AddTarget(ILogTarget* pTarget)
		{
			m_pTargets.push_back(pTarget);
		}
		void Log(LOG_LEVEL lvl, std::string szMsg, std::string szFile, std::string szFunction, int nLine)
		{
			bool shouldLog = false;
			for (size_t i = 0; i<m_pTargets.size(); i++)
			{
				if (m_pTargets[i]->IsEnabled(lvl))
				{
					shouldLog = true;
					break; // for
				}
			}

			if (shouldLog)
			{
				boost::filesystem::path p(szFile);
				std::ostringstream msg;
				msg << std::setfill('0') << std::setw(6) << getTickCount() - m_dwLogStart << " ";
				msg << "[" << szFunction << "] ";
				msg << p.filename() << ":" << nLine << " - " << szMsg;
				//std::string msg;
				//msg.Format(_T("%06d [%s] %s:%d - %s"), getTickCount() - m_dwLogStart, szFunction, ::PathFindFileName(szFile), nLine, szMsg);

				for (size_t i = 0; i<m_pTargets.size(); i++)
					if (m_pTargets[i]->IsEnabled(lvl))
						m_pTargets[i]->Append(msg.str());
			}
		}
	private:
		unsigned getTickCount()
		{
#ifdef WINDOWS
			return GetTickCount();
#else
			//auto t = std::chrono::system_clock::now();
			using namespace std::chrono;
			return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
			//struct timeval tv;
			//gettimeofday(&tv, 0);
			//return unsigned((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
		}
	};

	class CLoggerFactory
	{
	private:
		//static CLogger m_SingletonInstance;	// TODO: write a real singleton/ factory pattern implementation

	public:
		static CLogger* getDefaultInstance()
		{ 
			static CLogger m_SingletonInstance;
			return &m_SingletonInstance;
		}
	};
}

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)

#define __LOGMSG(lvl, msg, file, func, line) Logging::CLoggerFactory::getDefaultInstance()->Log(lvl, msg, file, func, line)

#ifdef UNICODE
#define LOGMSG(lvl, msg)  __LOGMSG(lvl, msg, WIDEN(__FILE__), WIDEN(__FUNCSIG__), __LINE__)
#define LOGMSG_DEBUG(msg) __LOGMSG(Logging::LOG_LEVEL_DEBUG, msg, WIDEN(__FILE__), WIDEN(__FUNCTION__), __LINE__)
#define LOGMSG_INFO(msg)  __LOGMSG(Logging::LOG_LEVEL_INFO, msg, WIDEN(__FILE__), WIDEN(__FUNCTION__), __LINE__)
#define LOGMSG_WARN(msg)  __LOGMSG(Logging::LOG_LEVEL_WARN, msg, WIDEN(__FILE__), WIDEN(__FUNCTION__), __LINE__)
#define LOGMSG_ERROR(msg) __LOGMSG(Logging::LOG_LEVEL_ERROR, msg, WIDEN(__FILE__), WIDEN(__FUNCTION__), __LINE__)
#define LOGMSG_FATAL(msg) __LOGMSG(Logging::LOG_LEVEL_FATAL, msg, WIDEN(__FILE__), WIDEN(__FUNCTION__), __LINE__)
#else
#define LOGMSG(lvl, msg)  __LOGMSG(lvl, msg, __FILE__, __FUNCSIG__, __LINE__)
#define LOGMSG_DEBUG(msg) __LOGMSG(Logging::LOG_LEVEL_DEBUG, msg, __FILE__, __FUNCTION__, __LINE__)
#define LOGMSG_INFO(msg)  __LOGMSG(Logging::LOG_LEVEL_INFO, msg, __FILE__, __FUNCTION__, __LINE__)
#define LOGMSG_WARN(msg)  __LOGMSG(Logging::LOG_LEVEL_WARN, msg, __FILE__, __FUNCTION__, __LINE__)
#define LOGMSG_ERROR(msg) __LOGMSG(Logging::LOG_LEVEL_ERROR, msg, __FILE__, __FUNCTION__, __LINE__)
#define LOGMSG_FATAL(msg) __LOGMSG(Logging::LOG_LEVEL_FATAL, msg, __FILE__, __FUNCTION__, __LINE__)
#endif

//class LogMgr
//{
//protected:
//	boost::log::sources::severity_logger< boost::log::trivial::severity_level > m_lg;
//
//	// New macro that includes severity, filename and line number
//#define CUSTOM_LOG(logger, sev) \
//   BOOST_LOG_STREAM_WITH_PARAMS( \
//      (logger), \
//         (set_get_attrib("File", path_to_filename(__FILE__))) \
//         (set_get_attrib("Line", __LINE__)) \
//         (::boost::log::keywords::severity = (sev)) \
//   )
//
//	// Set attribute and return the new value
//	template<typename ValueType>
//	ValueType set_get_attrib(const char* name, ValueType value) {
//		auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(boost::log::core::get()->get_global_attributes()[name]);
//		attr.set(value);
//		return attr.get();
//	}
//
//	// Convert file path to only the filename
//	std::string path_to_filename(std::string path) {
//		return path.substr(path.find_last_of("/\\") + 1);
//	}
//public:
//	LogMgr();
//	~LogMgr();
//
//	void PrintLog(boost::log::trivial::severity_level lv, const char* format, ...);
//};