// GPL (General Purpose Library) export
//

#pragma once

#ifdef _WIN32
#ifdef GPL_EXPORT
#define GPL_API __declspec(dllexport)
#else
#define GPL_API __declspec(dllimport)
#endif
#else	// case unix
#define gpl_API __attribute__ ((visibility ("default")))
#endif