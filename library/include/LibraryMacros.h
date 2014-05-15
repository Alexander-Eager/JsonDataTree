#ifndef JSON_LIBRARY_MACROS_H
#define JSON_LIBRARY_MACROS_H

#if defined(BUILDING_JSON_LIBRARY) // we are building the JSON library

#	define JSON_LIBRARY Q_DECL_EXPORT

#elif defined(INCLUDING_JSON_LIBRARY) // we are including the JSON library

#	define JSON_LIBRARY Q_DECL_IMPORT

#else // the files are being included with the project

#	define JSON_LIBRARY

#endif // defined(...)

#endif // JSON_LIBRARY_MACROS_H