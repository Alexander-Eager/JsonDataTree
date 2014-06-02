#ifndef JSON_FORWARDS_H
#define JSON_FORWARDS_H

#include <QList>
#include <QHash>
#include <JsonDataTree/LibraryMacros.h>

/**
 * \brief Namespace for JSON classes.
 **/
namespace JSON
{
	// JsonReader.h
	class JsonReader;
	class JsonReaderError;
	class JsonReaderErrors;

	// JsonWriter.h
	class JsonWriter;

	// JsonValue.h
	class JsonValue;

	// JsonObject.h
	using JsonObject = QHash<QString, JsonValue>;

	// JsonArray.h
	using JsonArray = QList<JsonValue>;

	// JsonPath.h
	class JsonKey;
	class JsonPath;
}

#endif // JSON_FORWARDS_H