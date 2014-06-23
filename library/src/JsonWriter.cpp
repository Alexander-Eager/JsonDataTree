// header file
#include <JsonDataTree/JsonWriter.h>

// internal data
#include <QSharedData>
#include <QVariant>
#include <JsonDataTree/JsonObject.h>
#include <JsonDataTree/JsonArray.h>

using namespace JSON;

// private data class
class JSON::JsonWriterPrivate : public QSharedData {
    public:
        JsonValue data;

        // write an indent to stream
        auto writeIndent(QTextStream& stream, int indent) const -> void;

        // write value to stream
        auto writeValue(QTextStream& stream, JsonValue value,
                        int indent) const -> void;

        // write a string value to stream
        auto writeString(QTextStream& stream, QString string) const -> void;

        // write a numeric value to stream
        auto writeNumber(QTextStream& stream, double number) const -> void;

        // write a boolean value to stream
        auto writeBoolean(QTextStream& stream, bool boolean) const -> void;

        // write a null value to stream
        auto writeNull(QTextStream& stream) const -> void;

        // write an object to stream
        auto writeObject(QTextStream& stream, JsonObject object,
                         int indent) const -> void;

        // write an array to stream
        auto writeArray(QTextStream& stream, JsonArray array,
                        int indent) const -> void;
};

JsonWriter::JsonWriter()
    :    d(new JsonWriterPrivate) {
    d->data = JsonValue::Null;
}

JsonWriter::JsonWriter(JsonValue data)
    :    d(new JsonWriterPrivate) {
    d->data = data;
}

JsonWriter::JsonWriter(const JsonWriter& other)
    :    d(other.d)
    { }

JsonWriter::~JsonWriter() { }

auto JsonWriter::operator= (const JsonWriter& other) -> JsonWriter& {
    if (d == other.d) return *this;
    d = other.d;
    return *this;
}

auto JsonWriter::getData() const -> JsonValue {
    return d->data;
}

auto JsonWriter::setData(JsonValue data) -> void {
    d->data = data;
}

auto JsonWriter::string() const -> QString {
    QString str;
    writeTo(&str);
    return str;
}

auto JsonWriter::writeTo(QString* str) const -> void {
    QTextStream stream(str);
    writeTo(stream);
}

auto JsonWriter::writeTo(QIODevice* io) const -> void {
    QTextStream stream(io);
    writeTo(stream);
}

auto JsonWriter::writeTo(QTextStream& stream) const -> void {
    d->writeValue(stream, d->data, 0);
}

auto JsonWriterPrivate::writeIndent(QTextStream& stream,
                                    int indent) const -> void {
    for (int i = 0; i < indent; ++ i) {
        stream << QChar('\t');    
    }
}

auto JsonWriterPrivate::writeValue(QTextStream& stream, JsonValue value,
                                   int indent) const -> void {
    JsonValue::Type type = value.getType();
    switch (type) {
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

auto JsonWriterPrivate::writeNumber(QTextStream& stream,
                                    double number) const -> void {
    stream << number;
}

auto JsonWriterPrivate::writeString(QTextStream& stream,
                                    QString string) const -> void {
    stream << QChar('\"');
    int i = 0;
    while (i < string.count()) {
        ushort c = string.at(i).unicode();
        switch (c) {
            /* Basic escape characters */
            case '\"': // quote
                stream << "\\\"";
                break;
            case '\\': // backslash
                stream << "\\\\";
                break;
            case '/': // this can be, but doesn't have to be, escaped
                // stream << "\\/";
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
                if (string.at(i).isPrint()) {
                    stream << string.at(i);
                } else {
                    // not printable, and not escape,
                    // so we need to \u it
                    stream << "\\u";
                    QString hex = QString::number(c, 16);
                    while (hex.count() < 4) {
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

auto JsonWriterPrivate::writeBoolean(QTextStream& stream,
                                     bool boolean) const -> void {
    if (boolean) {
        stream << "true";
    } else {
        stream << "false";
    }
}

auto JsonWriterPrivate::writeNull(QTextStream& stream) const -> void {
    stream << "null";
}

auto JsonWriterPrivate::writeArray(QTextStream& stream, JsonArray array,
                                   int indent) const -> void {
    // if its empty, one-line [] works
    if (array.isEmpty()) {
        stream << "[]";
        return;
    }
    // write the first value
    stream << "[\n";
    writeIndent(stream, indent + 1);
    writeValue(stream, array.first(), indent + 1);
    // write the rest, with preceding comma
    for (int i = 1; i < array.count(); ++ i) {
        stream << ",\n";
        writeIndent(stream, indent + 1);
        writeValue(stream, array.at(i), indent + 1);
    }
    // write the last ]
    stream << QChar('\n');
    writeIndent(stream, indent);
    stream << QChar(']');
}

auto JsonWriterPrivate::writeObject(QTextStream& stream, JsonObject object,
                                    int indent) const -> void {
    // if its empty, one-line {} works
    if (object.isEmpty()) {
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
    while (iter != object.constEnd()) {
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