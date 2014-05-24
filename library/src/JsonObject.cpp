// header file
#include <JsonDataTree/JsonObject.h>

// JsonValue class
#include <JsonDataTree/JsonValue.h>

// internal data
#include <QSharedData>
#include <QHash>

namespace JSON
{
	class JsonObjectPrivate : public QSharedData
	{
		public:
			QHash<QString, JsonValue> map;
	};
}

using namespace JSON;

JsonObject::JsonObject()
	:	d(new JsonObjectPrivate)
	{ }

JsonObject::~JsonObject() { }

JsonObject::JsonObject(const JsonObject& other)
	:	d(other.d)
	{ }

JsonObject& JsonObject::operator= (const JsonObject& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

int JsonObject::size() const
{
	return d->map.count();
}

int JsonObject::count() const
{
	return size();
}

bool JsonObject::isEmpty() const
{
	return !size();
}

void JsonObject::clear()
{
	d->map.clear();
}

QList<QString> JsonObject::keys() const
{
	return d->map.keys();
}

JsonObject::iterator JsonObject::insert(QString key, JsonValue value)
{
	return d->map.insert(key, value);
}

bool JsonObject::remove(QString key)
{
	if (!d->map.contains(key))
	{
		return false;
	}
	d->map.remove(key);
	return true;
}

JsonValue JsonObject::get(QString key) const
{
	return d->map[key];
}

JsonValue& JsonObject::get(QString key)
{
	return d->map[key];
}

JsonValue JsonObject::operator[] (QString key) const
{
	return get(key);
}

JsonValue& JsonObject::operator[] (QString key)
{
	return get(key);
}

JsonObject::iterator JsonObject::find(QString key)
{
	return d->map.find(key);
}

JsonObject::const_iterator JsonObject::find(QString key) const
{
	return d->map.find(key);
}

JsonObject::const_iterator JsonObject::constFind(QString key) const
{
	return d->map.constFind(key);
}

bool JsonObject::contains(QString key) const
{
	return d->map.contains(key);
}

JsonObject::iterator JsonObject::begin()
{
	return d->map.begin();
}

JsonObject::const_iterator JsonObject::begin() const
{
	return d->map.begin();
}

JsonObject::const_iterator JsonObject::constBegin() const
{
	return d->map.constBegin();
}

JsonObject::iterator JsonObject::end()
{
	return d->map.end();
}

JsonObject::const_iterator JsonObject::end() const
{
	return d->map.end();
}

JsonObject::const_iterator JsonObject::constEnd() const
{
	return d->map.constEnd();
}