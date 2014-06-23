// header file
#include <JsonDataTree/JsonPath.h>

// internal data
#include <QSharedData>

// private internal data class for JsonPath
class JSON::JsonPathPrivate : public QSharedData
{
    // currently empty
};

using namespace JSON;

JsonPath::JsonPath()
    : QList<JsonKey>(),
      d(new JsonPathPrivate) { }

JsonPath::JsonPath(std::initializer_list<JsonKey> keys)
    : QList<JsonKey>(keys),
      d(new JsonPathPrivate) { }

JsonPath::JsonPath(const char * const keys, char sep)
    : JsonPath(QString(keys), sep) { }

JsonPath::JsonPath(QString keys, QChar sep)
    : JsonPath(keys.split(sep)) { }

JsonPath::JsonPath(QStringList keys)
    : QList<JsonKey>(),
      d(new JsonPathPrivate) {
    for (QString key : keys) {
        append(JsonKey(key));
    }
}

JsonPath::JsonPath(QList<JsonKey> other)
    : QList<JsonKey>(other),
      d(new JsonPathPrivate) { }

JsonPath::JsonPath(const JsonPath& other)
    : QList<JsonKey>(other),
      d(new JsonPathPrivate) { }

JsonPath::~JsonPath() { }

auto JsonPath::operator= (const JsonPath& other) -> JsonPath& {
    if (d == other.d) {
        return *this;
    }
    d = other.d;
    reinterpret_cast<QList<JsonKey>*>(this)
        ->operator= (other);
    return *this;
}

auto JsonPath::operator== (const JsonPath& other) const -> bool {
    if (d == other.d) {
        return true;
    }
    return reinterpret_cast<const QList<JsonKey>*>(this)
        ->operator== (other);
}

auto JsonPath::operator!= (const JsonPath& other) const -> bool {
    return !(operator== (other));
}

auto operator<< (JsonPath& path, JsonKey key) -> JsonPath& {
	path.append(key);
	return path;
}

auto operator<< (JsonPath& path, JsonPath other) -> JsonPath& {
	path.append(other);
	return path;
}

// this is for JsonKey
class JSON::JsonKeyPrivate : public QSharedData
{
    public:
        bool isInteger;
        bool isArrayIndex;

        union {
            int integer;
            QString* string;
        };

        JsonKeyPrivate() { }

        JsonKeyPrivate(const JsonKeyPrivate& other) {
            isInteger = other.isInteger;
            isArrayIndex = other.isArrayIndex;
            if (isInteger) {
                integer = other.integer;
            }
            else {
                string = new QString(*other.string);
            }
        }

        ~JsonKeyPrivate() {
            if (!isInteger) {
                delete string;
            }
        }
};

JsonKey::JsonKey(QString key)
    : d(new JsonKeyPrivate) {
	d->isInteger = false;
	d->string = new QString(key);
	d->string->toInt(&d->isArrayIndex);
}

JsonKey::JsonKey(const char* const key)
    : JsonKey(QString(key)) { }

JsonKey::JsonKey(int key)
    : d(new JsonKeyPrivate) {
	d->isInteger = true;
	d->isArrayIndex = true;
	d->integer = key;
}

JsonKey::~JsonKey() { }

JsonKey::JsonKey(const JsonKey& other)
    : d(other.d) { }

auto JsonKey::operator= (const JsonKey& other) -> JsonKey& {
    if (d == other.d) {
        return *this;
    }
	d = other.d;
	return *this;
}

auto JsonKey::operator== (const JsonKey& other) const -> bool {
    if (d == other.d) {
        return true;
    }
    if (isInteger() != other.isInteger()) {
		return false;
	}
    if (isInteger()) {
		return toInteger() == other.toInteger();
	}
	return toString() == other.toString();
}

auto JsonKey::operator!= (const JsonKey& other) const -> bool {
	return !(operator== (other));
}

auto JsonKey::isObjectKey() const -> bool {
	return isString();
}

auto JsonKey::toObjectKey(bool* ok) const -> QString {
    if (ok) {
		*ok = isObjectKey();
	}
    if (isObjectKey()) {
		return *d->string;
	}
	return QString();
}

auto JsonKey::isArrayIndex() const -> bool {
	return d->isArrayIndex;
}

auto JsonKey::toArrayIndex(bool* ok) const -> int {
    if (ok) {
		*ok = isArrayIndex();
	}
    if (isInteger()) {
		return d->integer;
	}
    if (isArrayIndex()) {
		return d->string->toInt();
	}
	return -1;
}

auto JsonKey::isString() const -> bool {
	return !d->isInteger;
}

auto JsonKey::toString(bool* ok) const -> QString {
    if (ok) {
		*ok = isString();
	}
    if (isString()) {
		return *d->string;
	}
	return QString();
}

auto JsonKey::isInteger() const -> bool {
	return d->isInteger;
}

auto JsonKey::toInteger(bool* ok) const -> int {
    if (ok) {
		*ok = isInteger();
	}
    if (isInteger()) {
		return d->integer;
	}
	return -1;
}
