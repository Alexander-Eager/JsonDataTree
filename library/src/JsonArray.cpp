// header file
#include <JsonDataTree/JsonArray.h>

// JsonValue class
#include <JsonDataTree/JsonValue.h>

// internal data
#include <QSharedData>
#include <QList>

// static vars
#include <QGlobalStatic>

namespace JSON
{
	class JsonArrayPrivate : public QSharedData
	{
		public:
			QList<JsonValue> array;
	};
}

using namespace JSON;

Q_GLOBAL_STATIC(JsonValue, invalidValue)

JsonArray::JsonArray()
	:	d(new JsonArrayPrivate)
	{ }

JsonArray::~JsonArray() { }

JsonArray::JsonArray(const JsonArray& other)
	:	d(other.d)
	{ }

JsonArray& JsonArray::operator= (const JsonArray& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

int JsonArray::size() const
{
	return d->array.size();
}

int JsonArray::count() const
{
	return size();
}

bool JsonArray::isEmpty() const
{
	return !size();
}

void JsonArray::clear()
{
	d->array.clear();
}

void JsonArray::append(JsonValue value)
{
	d->array.append(value);
}

bool JsonArray::removeLast()
{
	if (isEmpty())
	{
		return false;
	}
	d->array.removeLast();
	return true;
}

JsonValue JsonArray::last() const
{
	if (isEmpty())
	{
		return JsonValue();
	}
	return d->array.last();
}

JsonValue& JsonArray::last()
{
	if (isEmpty())
	{
		invalidValue->setType(JsonValue::Null);
		return *invalidValue;
	}
	return d->array.last();
}

void JsonArray::prepend(JsonValue value)
{
	d->array.prepend(value);
}

bool JsonArray::removeFirst()
{
	if (isEmpty())
	{
		return false;
	}
	d->array.removeFirst();
	return true;
}

JsonValue JsonArray::first() const
{
	if (isEmpty())
	{
		return JsonValue();
	}
	return d->array.first();
}

JsonValue& JsonArray::first()
{
	if (isEmpty())
	{
		invalidValue->setType(JsonValue::Null);
		return *invalidValue;
	}
	return d->array.first();
}

void JsonArray::insert(int index, JsonValue value)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index > size())
	{
		index = size();
	}
	d->array.insert(index, value);	
}

bool JsonArray::remove(int index)
{
	if (index < 0 || index >= size())
	{
		return false;
	}
	d->array.removeAt(index);
	return true;
}

JsonValue JsonArray::at(int index) const
{
	if (index < 0 || index >= size())
	{
		return JsonValue();
	}
	return d->array.at(index);
}

JsonValue& JsonArray::at(int index)
{
	if (index < 0 || index >= size())
	{
		invalidValue->setType(JsonValue::Null);
		return *invalidValue;
	}
	return d->array[index];
}

JsonValue JsonArray::operator[] (int index) const
{
	return at(index);
}

JsonValue& JsonArray::operator[] (int index)
{
	return at(index);
}

bool JsonArray::set(int index, JsonValue value)
{
	if (index < 0 || index >= size())
	{
		return false;
	}
	d->array[index] = value;
	return true;
}

JsonArray::iterator JsonArray::begin()
{
	return d->array.begin();
}

JsonArray::const_iterator JsonArray::begin() const
{
	return d->array.begin();
}

JsonArray::const_iterator JsonArray::constBegin() const
{
	return d->array.begin();
}

JsonArray::iterator JsonArray::end()
{
	return d->array.end();
}

JsonArray::const_iterator JsonArray::end() const
{
	return d->array.end();
}

JsonArray::const_iterator JsonArray::constEnd() const
{
	return d->array.begin();
}

JsonArray& operator<< (JsonArray& array, JsonValue value)
{
	array.append(value);
	return array;
}