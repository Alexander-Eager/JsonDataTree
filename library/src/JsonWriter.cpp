// header file
#include <JsonDataTree/JsonWriter.h>

// internal data
#include <QSharedData>
#include <QVariant>
#include <JsonDataTree/JsonObject.h>
#include <JsonDataTree/JsonArray.h>

// private data class
namespace JSON
{
	class JsonWriterPrivate : public QSharedData
	{
		public:
			JsonValue data;

			// write an indent to stream
			void writeIndent(QTextStream& stream, int indent) const;

			// write value to stream
			void writeValue(QTextStream& stream, JsonValue value,
							int indent) const;

			// write a string value to stream
			void writeString(QTextStream& stream, QString string) const;

			// write a numeric value to stream
			void writeNumber(QTextStream& stream, double number) const;

			// write a boolean value to stream
			void writeBoolean(QTextStream& stream, bool boolean) const;

			// write a null value to stream
			void writeNull(QTextStream& stream) const;

			// write an object to stream
			void writeObject(QTextStream& stream, JsonObject object,
								int indent) const;

			// write an array to stream
			void writeArray(QTextStream& stream, JsonArray array,
								int indent) const;
	};
}

using namespace JSON;

JsonWriter::JsonWriter()
	:	d(new JsonWriterPrivate)
{
	d->data = JsonValue::Null;
}

JsonWriter::JsonWriter(JsonValue data)
	:	d(new JsonWriterPrivate)
{
	d->data = data;
}

JsonWriter::JsonWriter(const JsonWriter& other)
	:	d(other.d)
	{ }

JsonWriter::~JsonWriter() { }

JsonWriter& JsonWriter::operator= (const JsonWriter& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

JsonValue JsonWriter::getData() const
{
	return d->data;
}

void JsonWriter::setData(JsonValue data)
{
	d->data = data;
}

QString JsonWriter::string() const
{
	QString str;
	writeTo(&str);
	return str;
}

void JsonWriter::writeTo(QString* str) const
{
	QTextStream stream(str);
	writeTo(stream);
}

void JsonWriter::writeTo(QIODevice* io) const
{
	QTextStream stream(io);
	writeTo(stream);
}

void JsonWriter::writeTo(QTextStream& stream) const
{
	d->writeValue(stream, d->data, 0);
}

void JsonWriterPrivate::writeIndent(QTextStream& stream, int indent) const
{
	for (int i = 0; i < indent; ++ i)
	{
		stream << QChar('\t');	
	}
}

void JsonWriterPrivate::writeValue(QTextStream& stream, JsonValue value,
									int indent) const
{
	JsonValue::Type type = value.getType();
	switch (type)
	{
		case JsonValue::Number:
			writeNumber(stream, value.toDouble());
			break;
		case JsonValue::String:
			writeString(stream, value.toString());
			break;
		case JsonValue::Boolean:
			writeBoolean(stream, value.toBoolean());
			break;
		case JsonValue::Object:
			writeObject(stream, value.toObject(), indent);
			break;
		case JsonValue::Array:
			writeArray(stream, value.toArray(), indent);
			break;
		case JsonValue::Null:
		default:
			writeNull(stream);
	}
}

void JsonWriterPrivate::writeNumber(QTextStream& stream, double number) const
{
	stream << number;
}

void JsonWriterPrivate::writeString(QTextStream& stream, QString string) const
{
	stream << QChar('\"');
	int i = 0;
	while (i < string.count())
	{
		ushort c = string.at(i).unicode();
		switch (c)
		{
			/* Basic escape characters */
			case '\"': // quote
				stream << "\\\"";
				break;
			case '\\': // backslash
				stream << "\\\\";
				break;
			case '/': // this can be, but doesn't have to be, escaped
				stream << "\\/";
				break;
			case '\b': // backspace character
				stream << "\\b";
				break;
			case '\f': // formfeed
				stream << "\\f";
				break;
			case '\n': // newline
				stream << "\\n";
				break;
			case '\r': // linefeed
				stream << "\\r";
				break;
			case '\t': // tab
				stream << "\\t";
				break;
			/* Everything else */
			default:
				if (string.at(i).isPrint())
				{
					stream << string.at(i);
				}
				else
				{
					// not printable, and not escape,
					// so we need to \u it
					stream << "\\u";
					QString hex = QString::number(c, 16);
					while (hex.count() < 4)
					{
						hex = '0' + hex;
					}
					stream << hex;
				}
				break;
		}
		++ i;
	}
	stream << QChar('\"');
}

void JsonWriterPrivate::writeBoolean(QTextStream& stream, bool boolean) const
{
	if (boolean)
	{
		stream << "true";
	}
	else
	{
		stream << "false";
	}
}

void JsonWriterPrivate::writeNull(QTextStream& stream) const
{
	stream << "null";
}

void JsonWriterPrivate::writeArray(QTextStream& stream, JsonArray array,
									int indent) const
{
	// if its empty, one-line [] works
	if (array.isEmpty())
	{
		stream << "[]";
		return;
	}
	// write the first value
	stream << "[\n";
	writeIndent(stream, indent + 1);
	writeValue(stream, array.first(), indent + 1);
	// write the rest, with preceding comma
	for (int i = 1; i < array.count(); ++ i)
	{
		stream << ",\n";
		writeIndent(stream, indent + 1);
		writeValue(stream, array.at(i), indent + 1);
	}
	// write the last ]
	stream << QChar('\n');
	writeIndent(stream, indent);
	stream << QChar(']');
}

void JsonWriterPrivate::writeObject(QTextStream& stream, JsonObject object,
									int indent) const
{
	// if its empty, one-line {} works
	if (object.isEmpty())
	{
		stream << "{}";
		return;
	}
	// write the first key-value pair
	stream << "{\n";
	writeIndent(stream, indent + 1);
	auto iter = object.constBegin();
	writeString(stream, iter.key());
	stream << ": ";
	writeValue(stream, *iter, indent + 1);
	++ iter;
	// write the rest, with preceding comma
	while (iter != object.constEnd())
	{
		stream << ",\n";
		writeIndent(stream, indent + 1);
		writeString(stream, iter.key());
		stream << ": ";
		writeValue(stream, *iter, indent + 1);
		++ iter;
	}
	// write the ending }
	stream << QChar('\n');
	writeIndent(stream, indent);
	stream << QChar('}');
}