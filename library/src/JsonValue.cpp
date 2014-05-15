// header file
#include "JsonValue.h"

// internal data
#include <QSharedData>
#include <QVariant>
#include "JsonObject.h"
#include "JsonArray.h"

// JsonValuePrivate internal data class
namespace JSON
{
	class JsonValuePrivate : public QSharedData
	{
		public:
			JsonValue::Type type;
			union
			{
				int integer;
				double floating;
				QString* string;
				bool boolean;
				JsonArray* array;
				JsonObject* object;
			};

			// deletes necessary objects and resets
			// the type to Null
			void clean()
			{
				switch (type)
				{
					case JsonValue::String:
						delete string;
						break;
					case JsonValue::Array:
						delete array;
						break;
					case JsonValue::Object:
						delete object;
						break;
					default:
						break;
				}
				type = JsonValue::Null;
			}

			JsonValuePrivate()
				:	type(JsonValue::Null)
				{ }

			~JsonValuePrivate()
			{
				clean();
			}

			JsonValuePrivate(const JsonValuePrivate& other)
				:	type(other.type)
			{
				switch (type)
				{
					case JsonValue::Integer:
						integer = other.integer;
						break;
					case JsonValue::Double:
						floating = other.floating;
						break;
					case JsonValue::Boolean:
						boolean = other.boolean;
						break;
					case JsonValue::String:
						string = new QString(*other.string);
						break;
					case JsonValue::Array:
						array = new JsonArray(*other.array);
						break;
					case JsonValue::Object:
						object = new JsonObject(*other.object);
						break;
					case JsonValue::Null:
						// nothing to do
						break;
					default:
						// not a defined type
						break;
				}
			}
	};
}

using namespace JSON;

JsonArray invalidArray;
JsonObject invalidObject;

JsonValue::~JsonValue() { }

JsonValue::JsonValue()
	:	d(new JsonValuePrivate)
{
	setType(Null);
}

JsonValue::JsonValue(const JsonValue& other)
	:	d(other.d)
	{ }

JsonValue& JsonValue::operator= (const JsonValue& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

JsonValue::JsonValue(JsonValue::Type type)
	:	d(new JsonValuePrivate)
{
	setType(type);
}

void JsonValue::setType(JsonValue::Type type)
{
	d->clean();
	d->type = type;
	switch (type)
	{
		case Array:
			d->array = new JsonArray();
			break;
		case Object:
			//d->object = new JsonObject();
			break;
		case String:
			d->string = new QString();
			break;
		case Integer:
			d->integer = 0;
			break;
		case Double:
			d->floating = 0.0;
			break;
		case Boolean:
			d->boolean = false;
			break;
		case Null:
			// nothing to do
			break;
		default:
			// not a legit type, so make it Null
			d->type = Null;
			break;
	}
}

JsonValue::JsonValue(int val)
	:	d(new JsonValuePrivate)
{
	setInteger(val);
}

void JsonValue::setInteger(int val)
{
	d->clean();
	d->type = Integer;
	d->integer = val;
}

JsonValue::JsonValue(double val)
	:	d(new JsonValuePrivate)
{
	setDouble(val);
}

void JsonValue::setDouble(double val)
{
	d->clean();
	d->type = Double;
	d->floating = val;
}

JsonValue::JsonValue(QString val)
	:	d(new JsonValuePrivate)
{
	setString(val);
}

void JsonValue::setString(QString val)
{
	d->clean();
	d->type = String;
	d->string = new QString(val);
}

JsonValue::JsonValue(const char* const val)
	:	JsonValue(QString(val))
	{ }

void JsonValue::setString(const char* const val)
{
	setString(QString(val));
}

JsonValue::JsonValue(bool val)
	:	d(new JsonValuePrivate)
{
	setBoolean(val);
}

void JsonValue::setBoolean(bool val)
{
	d->clean();
	d->type = Boolean;
	d->boolean = val;
}

JsonValue::JsonValue(JsonArray val)
	:	d(new JsonValuePrivate)
{
	setArray(val);
}

void JsonValue::setArray(JsonArray val)
{
	d->clean();
	d->type = Array;
	d->array = new JsonArray(val);
}

JsonValue::JsonValue(JsonObject val)
	:	d(new JsonValuePrivate)
{
	setObject(val);
}

void JsonValue::setObject(JsonObject val)
{
	d->clean();
	d->type = Object;
	d->object = new JsonObject(val);
}

JsonValue::Type JsonValue::getType() const
{
	return d->type;
}

bool JsonValue::isNull() const
{
	return d->type == Null;
}

bool JsonValue::isInteger() const
{
	return d->type == Integer;
}

int JsonValue::toInteger(bool* ok) const
{
	if (ok)
	{
		*ok = isInteger();
	}
	switch (d->type)
	{
		case Integer:
			return d->integer;
		case Double:
			return (int) d->floating;
		default:
			return 0;
	}
}

bool JsonValue::isDouble() const
{
	return d->type == Double;
}

double JsonValue::toDouble(bool* ok) const
{
	if (ok)
	{
		*ok = isDouble();
	}
	switch (d->type)
	{
		case Double:
			return d->floating;
		case Integer:
			return (double) d->integer;
		default:
			return 0.0;
	}
}

bool JsonValue::isString() const
{
	return d->type == String;
}

QString JsonValue::toString(bool* ok) const
{
	if (ok)
	{
		*ok = isString();
	}
	switch (d->type)
	{
		case String:
			return *d->string;
		default:
			return QString();
	}
}

bool JsonValue::isBoolean() const
{
	return d->type == Boolean;
}

bool JsonValue::toBoolean(bool* ok) const
{
	if (ok)
	{
		*ok = isBoolean();
	}
	switch (d->type)
	{
		case Boolean:
			return d->boolean;
		case Integer:
			return d->integer;
		case Double:
			return d->floating;
		default:
			return false;
	}
}

bool JsonValue::isArray() const
{
	return d->type == Array;
}

JsonArray& JsonValue::toArray(bool* ok)
{
	if (ok)
	{
		*ok = isArray();
	}
	if (isArray())
	{
		return *d->array;
	}
	return invalidArray;
}

JsonArray JsonValue::toArray(bool* ok) const
{
	if (ok)
	{
		*ok = isArray();
	}
	if (isArray())
	{
		return *d->array;
	}
	return JsonArray();
}

bool JsonValue::isObject() const
{
	return d->type == Object;
}

JsonObject& JsonValue::toObject(bool* ok)
{
	if (ok)
	{
		*ok = isObject();
	}
	if (isObject())
	{
		return *d->object;
	}
	return invalidObject;
}

JsonObject JsonValue::toObject(bool* ok) const
{
	if (ok)
	{
		*ok = isObject();
	}
	if (isObject())
	{
		return *d->object;
	}
	return JsonObject();
}