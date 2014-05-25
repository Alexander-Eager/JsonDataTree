// header file
#include <JsonDataTree/JsonValue.h>

// for path following
#include <JsonDataTree/JsonPath.h>

// internal data
#include <QSharedData>

// JsonValuePrivate internal data class
namespace JSON
{
	class JsonValuePrivate : public QSharedData
	{
		public:
			JsonValue::Type type;
			union
			{
				double number;
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
					case JsonValue::Number:
						number = other.number;
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
JsonValue invalidValueTwo;

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
		case Number:
			d->number = 0.0;
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
	d->type = Number;
	d->number = val;
}

JsonValue::JsonValue(double val)
	:	d(new JsonValuePrivate)
{
	setDouble(val);
}

void JsonValue::setDouble(double val)
{
	d->clean();
	d->type = Number;
	d->number = val;
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

bool JsonValue::isNumber() const
{
	return d->type == Number;
}

int JsonValue::toInteger(bool* ok) const
{
	if (ok)
	{
		*ok = isNumber();
	}
	if (isNumber())
	{
		return (int) d->number;
	}
	return 0;
}

double JsonValue::toDouble(bool* ok) const
{
	if (ok)
	{
		*ok = isNumber();
	}
	if (isNumber())
	{
		return d->number;
	}
	return 0.0;
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
	if (isString())
	{
		return *d->string;
	}
	return QString();
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
	if (isBoolean())
	{
		return d->boolean;
	}
	return false;
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
	invalidArray.clear();
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
	invalidObject.clear();
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

JsonValue JsonValue::follow(JsonPath path, bool* ok) const
{
	JsonValue val = *this;
	// follow down the path
	for (auto key : path)
	{
		if (val.isObject())
		{
			if (!key.isObjectKey())
			{
				// not the right kind
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the object and key
			QString k = key.toObjectKey();
			JsonObject obj = val.toObject();
			if (!obj.contains(k))
			{
				// the association isn't there,
				// so quit
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the association's value
			val = obj.get(k);
		}
		else if (val.isArray())
		{
			if (!key.isArrayIndex())
			{
				// not the right kind
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the array and index
			int k = key.toArrayIndex();
			JsonArray arr = val.toArray();
			if (k < 0 || k >= arr.count())
			{
				// not in range
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the value at that index
			val = arr.at(k);
		}
		else
		{
			// not an array/object
			if (ok)
			{
				*ok = false;
			}
			invalidValueTwo = Null;
			return invalidValueTwo;
		}
	}
	if (ok)
	{
		*ok = true;
	}
	return val;
}

JsonValue& JsonValue::follow(JsonPath path, bool* ok)
{
	JsonValue* val = this;
	// follow down the path
	for (auto key : path)
	{
		if (val->isObject())
		{
			if (!key.isObjectKey())
			{
				// not the right kind
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the object and key
			QString k = key.toObjectKey();
			JsonObject* obj = &val->toObject();
			if (!obj->contains(k))
			{
				// the association isn't there,
				// so quit
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the association's value
			val = &obj->get(k);
		}
		else if (val->isArray())
		{
			if (!key.isArrayIndex())
			{
				// not the right kind
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the array and index
			int k = key.toArrayIndex();
			JsonArray* arr = &val->toArray();
			if (k < 0 || k >= arr->count())
			{
				// not in range
				if (ok)
				{
					*ok = false;
				}
				invalidValueTwo = Null;
				return invalidValueTwo;
			}
			// get the value at that index
			val = &arr->at(k);
		}
		else
		{
			// not an array/object
			if (ok)
			{
				*ok = false;
			}
			invalidValueTwo = Null;
			return invalidValueTwo;
		}
	}
	if (ok)
	{
		*ok = true;
	}
	return *val;
}

JsonValue& JsonValue::create(JsonPath path, bool* ok)
{
	JsonValue* val = this;
	// follow down the path
	for (auto key : path)
	{
		if (val->isNull() && key.isObjectKey())
		{
			// make it an object
			val->setType(Object);
		}
		else if (val->isNull() && key.isArrayIndex())
		{
			// make it an array
			val->setType(Array);
		}

		if (val->isObject() && key.isObjectKey())
		{
			// get and/or make the association
			val = &val->toObject().get(key.toObjectKey());
		}
		else if (val->isArray() && key.isArrayIndex())
		{
			JsonArray* arr = &val->toArray();
			int k = key.toArrayIndex();
			if (k >= 0 && k < arr->count())
			{
				// it exists, so just get it
				val = &arr->at(k);
			}
			else
			{
				// create and get the value at that index
				if (k < 0)
				{
					k = 0;
				}
				else
				{
					k = arr->count();
				}
				arr->insert(k, Null);
				val = &arr->at(k);
			}
		}
		else
		{
			// not the right kind of value
			if (ok)
			{
				*ok = false;
			}
			invalidValueTwo = Null;
			return invalidValueTwo;
		}
	}
	if (ok)
	{
		*ok = true;
	}
	return *val;
}