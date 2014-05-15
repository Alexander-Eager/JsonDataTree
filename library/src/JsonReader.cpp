// header file
#include "JsonReader.h"

// internal data
#include <QSharedData>
#include <QVariant>
#include "JsonObject.h"
#include "JsonArray.h"

// private data class
namespace JSON
{
	class JsonReaderPrivate : public QSharedData
	{
		public:
			// skip past white space
			void skipWhiteSpace(QTextStream& stream) const;

			// read a value from the stream
			JsonValue readValue(QTextStream& stream) const;

			// read a string from the stream
			QString readString(QTextStream& stream) const;

			// read a number from the stream
			double readNumber(QTextStream& stream) const;
	};
}

JsonReader::JsonReader()
	:	d(new JsonReaderPrivate)
	{ }

JsonReader::~JsonReader() { }

JsonValue JsonReader::parse()