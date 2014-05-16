// header file
#include <JsonDataTree/JsonPath.h>

// internal data
#include <QSharedData>

// private internal data classes
namespace JSON
{
	class JsonPathPrivate : public QSharedData
	{
		public:
			QList<JsonKey> keys;
	};

	class JsonKeyPrivate : public QSharedData
	{
		public:
			bool isInteger;
			bool isArrayIndex;

			union
			{
				int integer;
				QString* string;
			};

			JsonKeyPrivate() { }

			JsonKeyPrivate(const JsonKeyPrivate& other)
			{
				isInteger = other.isInteger;
				isArrayIndex = other.isArrayIndex;
				if (isInteger)
				{
					integer = other.integer;
				}
				else
				{
					string = new QString(*other.string);
				}
			}

			~JsonKeyPrivate()
			{
				if (!isInteger)
				{
					delete string;
				}
			}
	};
}

using namespace JSON;

JsonKey invalidKey(-1);

JsonPath::JsonPath()
	:	d(new JsonPathPrivate)
	{ }

JsonPath::JsonPath(std::initializer_list<JsonKey> keys)
	:	d(new JsonPathPrivate)
{
	d->keys = keys;
}

JsonPath::JsonPath(const char* const keys, char sep)
	:	JsonPath(QString(keys), QChar(sep))
	{ }

JsonPath::JsonPath(QString keys, QChar sep)
	:	JsonPath(keys.split(sep))
	{ }

JsonPath::JsonPath(QStringList keys)
	:	d(new JsonPathPrivate)
{
	for (auto str : keys)
	{
		d->keys << str;
	}
}

JsonPath::~JsonPath() { }

JsonPath::JsonPath(const JsonPath& other)
	:	d(other.d)
	{ }

JsonPath& JsonPath::operator= (const JsonPath& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

bool JsonPath::operator== (const JsonPath& other) const
{
	if (this == &other) return true;
	if (length() != other.length())
	{
		return false;
	}
	for (int i = 0; i < length(); ++ i)
	{
		if (at(i) != other.at(i))
		{
			return false;
		}
	}
	return true;
}

bool JsonPath::operator!= (const JsonPath& other) const
{
	return !(operator== (other));
}

bool JsonPath::startsWithObject() const
{
	if (!length())
	{
		return false;
	}
	return d->keys[0].isObjectKey();
}

bool JsonPath::startsWithArray() const
{
	if (!length())
	{
		return false;
	}
	return d->keys[0].isArrayIndex();
}

int JsonPath::length() const
{
	return d->keys.count();
}

JsonKey JsonPath::at(int index) const
{
	if (index < 0 || index >= length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys[index];
}

JsonKey& JsonPath::at(int index)
{
	if (index < 0 || index >= length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys[index];
}

JsonKey JsonPath::operator[] (int index) const
{
	return at(index);
}

JsonKey& JsonPath::operator[] (int index)
{
	return at(index);
}

bool JsonPath::set(int index, JsonKey key)
{
	if (index < 0 || index >= length())
	{
		return false;
	}
	d->keys[index] = key;
	return true;
}

bool JsonPath::remove(int index)
{
	if (index < 0 || index >= length())
	{
		return false;
	}
	d->keys.removeAt(index);
	return true;
}

void JsonPath::insert(int index, JsonKey key)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index > length())
	{
		index = length();
	}
	d->keys.insert(index, key);
}

void JsonPath::append(JsonKey key)
{
	d->keys.append(key);
}

void JsonPath::append(JsonPath path)
{
	for (auto key : path)
	{
		d->keys.append(key);
	}
}

bool JsonPath::removeLast()
{
	if (length())
	{
		d->keys.removeLast();
		return true;
	}
	return false;
}

JsonKey JsonPath::last() const
{
	if (!length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys.last();
}

JsonKey& JsonPath::last()
{
	if (!length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys.last();
}

void JsonPath::prepend(JsonKey key)
{
	d->keys.prepend(key);
}

void JsonPath::prepend(JsonPath path)
{
	for (auto key : path)
	{
		d->keys.prepend(key);
	}
}

bool JsonPath::removeFirst()
{
	if (length())
	{
		d->keys.removeFirst();
		return true;
	}
	return false;
}

JsonKey JsonPath::first() const
{
	if (!length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys.first();
}

JsonKey& JsonPath::first()
{
	if (!length())
	{
		invalidKey = -1;
		return invalidKey;
	}
	return d->keys.first();
}

bool JsonPath::contains(JsonKey key) const
{
	return d->keys.contains(key);
}

int JsonPath::indexOf(JsonKey key) const
{
	return d->keys.indexOf(key);
}

int JsonPath::lastIndexOf(JsonKey key) const
{
	return d->keys.lastIndexOf(key);
}

JsonPath JsonPath::trimmed(int first, int last) const
{
	if (first == 0 && last == length())
	{
		return *this;
	}
	JsonPath ans;
	for (int i = first; i < last; ++ i)
	{
		ans.append(at(i));
	}
	return ans;
}

JsonPath::iterator JsonPath::begin()
{
	return d->keys.begin();
}

JsonPath::const_iterator JsonPath::begin() const
{
	return d->keys.begin();
}

JsonPath::const_iterator JsonPath::constBegin() const
{
	return d->keys.constBegin();
}

JsonPath::iterator JsonPath::end()
{
	return d->keys.end();
}

JsonPath::const_iterator JsonPath::end() const
{
	return d->keys.end();
}

JsonPath::const_iterator JsonPath::constEnd() const
{
	return d->keys.constEnd();
}

JsonPath& operator<< (JsonPath& path, JsonKey key)
{
	path.append(key);
	return path;
}

JsonPath& operator<< (JsonPath& path, JsonPath other)
{
	path.append(other);
	return path;
}

// this is for JsonKey

JsonKey::JsonKey(QString key)
	:	d(new JsonKeyPrivate)
{
	d->isInteger = false;
	d->string = new QString(key);
	d->string->toInt(&d->isArrayIndex);
}

JsonKey::JsonKey(const char* const key)
	:	JsonKey(QString(key))
	{ }

JsonKey::JsonKey(int key)
	:	d(new JsonKeyPrivate)
{
	d->isInteger = true;
	d->isArrayIndex = true;
	d->integer = key;
}

JsonKey::~JsonKey() { }

JsonKey::JsonKey(const JsonKey& other)
	:	d(other.d)
	{ }

JsonKey& JsonKey::operator= (const JsonKey& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

bool JsonKey::operator== (const JsonKey& other) const
{
	if (this == &other) return true;
	if (isInteger() != other.isInteger())
	{
		return false;
	}
	if (isInteger())
	{
		return toInteger() == other.toInteger();
	}
	return toString() == other.toString();
}

bool JsonKey::operator!= (const JsonKey& other) const
{
	return !(operator== (other));
}

bool JsonKey::isObjectKey() const
{
	return isString();
}

QString JsonKey::toObjectKey(bool* ok) const
{
	if (ok)
	{
		*ok = isObjectKey();
	}
	if (isObjectKey())
	{
		return *d->string;
	}
	return QString();
}

bool JsonKey::isArrayIndex() const
{
	return d->isArrayIndex;
}

int JsonKey::toArrayIndex(bool* ok) const
{
	if (ok)
	{
		*ok = isArrayIndex();
	}
	if (isInteger())
	{
		return d->integer;
	}
	if (isArrayIndex())
	{
		return d->string->toInt();
	}
	return -1;
}

bool JsonKey::isString() const
{
	return !d->isInteger;
}

QString JsonKey::toString(bool* ok) const
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

bool JsonKey::isInteger() const
{
	return d->isInteger;
}

int JsonKey::toInteger(bool* ok) const
{
	if (ok)
	{
		*ok = isInteger();
	}
	if (isInteger())
	{
		return d->integer;
	}
	return -1;
}