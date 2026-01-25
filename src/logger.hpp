#pragma once

static int LOG_LEVEL = 3;

// TODO placeholder logging
// move to separate thread able to pipe in text/valus that logs to screen and std::cout

// TODO fix issue of overwriting NOL log lines
#define TRACE_LOG_TEXT_NOL(x) if(LOG_LEVEL>2) std::cout << "T - [" << get_name() << "::" << __func__ << "()] -->" << " " << x << "\t\t\r";
#define TRACE_LOG_FUNC_NOL    if(LOG_LEVEL>2) std::cout << "T - [" << get_name() << "::" << __func__ << "()]" << "\t\t\r";
#define TRACE_LOG_TEXT(x)     if(LOG_LEVEL>2) std::cout << "T - [" << get_name() << "::" << __func__ << "()] -->" << " " << x << std::endl;
#define TRACE_LOG_FUNC        if(LOG_LEVEL>2) std::cout << "T - [" << get_name() << "::" << __func__ << "()]" << std::endl;


#define DEBUG_LOG_TEXT_NOL(x) if(LOG_LEVEL>1) std::cout << "D - [" << get_name() << "::" << __func__ << "()] -->" << " " << x << "\t\t\r";
#define DEBUG_LOG_FUNC_NOL    if(LOG_LEVEL>1) std::cout << "D - [" << get_name() << "::" << __func__ << "()]" << "\t\t\r";
#define DEBUG_LOG_TEXT(x)     if(LOG_LEVEL>1) std::cout << "D - [" << get_name() << "::" << __func__ << "()] -->" << " " << x << std::endl;
#define DEBUG_LOG_FUNC        if(LOG_LEVEL>1) std::cout << "D - [" << get_name() << "::" << __func__ << "()]" << std::endl;
