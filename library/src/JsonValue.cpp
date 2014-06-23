// header file
#include <JsonDataTree/JsonValue.h>

// for path following
#include <JsonDataTree/JsonPath.h>

// internal data
#include <QSharedData>

// for global variables
#include <QGlobalStatic>

// JsonValuePrivate internal data class
class JSON::JsonValuePrivate : public QSharedData {
	public:
		JsonValue::Type type;
		union {
			double number;
			QString* string;
			bool boolean;
			JsonArray* array;
			JsonObject* object;
		};

		// deletes necessary objects and resets
		// the type to Null
		auto clean() -> void {
			switch (type) {
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
			:	type(JsonValue::Null) { }

		~JsonValuePrivate() {
			clean();
		}

		JsonValuePrivate(const JsonValuePrivate& other)
			:	type(other.type) {
			switch (type) {
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

using namespace JSON;

Q_GLOBAL_STATIC(JsonArray, invalidArray)
Q_GLOBAL_STATIC(JsonObject, invalidObject)
Q_GLOBAL_STATIC(JsonValue, invalidValueTwo)

JsonValue::~JsonValue() { }

JsonValue::JsonValue()
	:	d(new JsonValuePrivate) {
	setType(Null);
}

JsonValue::JsonValue(const JsonValue& other)
	:	d(other.d) { }

auto JsonValue::operator= (const JsonValue& other) -> JsonValue& {
	if (d == other.d) return *this;
	d = other.d;
	return *this;
}

JsonValue::JsonValue(JsonValue::Type type)
	:	d(new JsonValuePrivate) {
	setType(type);
}

auto JsonValue::setType(JsonValue::Type type) -> void {
	d->clean();
	d->type = type;
	switch (type) {
		case Array:
			d->array = new JsonArray();
			break;
		case Object:
			d->object = new JsonObject();
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
	:	d(new JsonValuePrivate) {
	setInteger(val);
}

auto JsonValue::setInteger(int val) -> void {
	d->clean();
	d->type = Number;
	d->number = val;
}

JsonValue::JsonValue(double val)
	:	d(new JsonValuePrivate) {
	setDouble(val);
}

auto JsonValue::setDouble(double val) -> void {
	d->clean();
	d->type = Number;
	d->number = val;
}

JsonValue::JsonValue(QString val)
	:	d(new JsonValuePrivate) {
	setString(val);
}

auto JsonValue::setString(QString val) -> void {
	d->clean();
	d->type = String;
	d->string = new QString(val);
}

JsonValue::JsonValue(const char* const val)
	:	JsonValue(QString(val)) { }

auto JsonValue::setString(const char* const val) -> void {
	setString(QString(val));
}

JsonValue::JsonValue(bool val)
	:	d(new JsonValuePrivate) {
	setBoolean(val);
}

auto JsonValue::setBoolean(bool val) -> void {
	d->clean();
	d->type = Boolean;
	d->boolean = val;
}

JsonValue::JsonValue(JsonArray val)
	:	d(new JsonValuePrivate) {
	setArray(val);
}

auto JsonValue::setArray(JsonArray val) -> void {
	d->clean();
	d->type = Array;
	d->array = new JsonArray(val);
}

JsonValue::JsonValue(JsonObject val)
	:	d(new JsonValuePrivate) {
	setObject(val);
}

auto JsonValue::setObject(JsonObject val) -> void {
	d->clean();
	d->type = Object;
	d->object = new JsonObject(val);
}

auto JsonValue::getType() const -> Type {
	return d->type;
}

auto JsonValue::isNull() const -> bool {
	return d->type == Null;
}

auto JsonValue::isNumber() const -> bool {
	return d->type == Number;
}

auto JsonValue::toInteger(bool* ok) const -> int {
	if (ok) {
		*ok = isNumber();
	}
	if (isNumber()) {
		return (int) d->number;
	}
	return 0;
}

auto JsonValue::toDouble(bool* ok) const -> double {
	if (ok) {
		*ok = isNumber();
	}
	if (isNumber()) {
		return d->number;
	}
	return 0.0;
}

auto JsonValue::isString() const -> bool {
	return d->type == String;
}

auto JsonValue::toString(bool* ok) const -> QString {
	if (ok) {
		*ok = isString();
	}
	if (isString()) {
		return *d->string;
	}
	return QString();
}

auto JsonValue::isBoolean() const -> bool {
	return d->type == Boolean;
}

auto JsonValue::toBoolean(bool* ok) const -> bool {
	if (ok) {
		*ok = isBoolean();
	}
	if (isBoolean()) {
		return d->boolean;
	}
	return false;
}

auto JsonValue::isArray() const -> bool {
	return d->type == Array;
}

auto JsonValue::toArray(bool* ok) -> JsonArray& {
	if (ok) {
		*ok = isArray();
	}
	if (isArray()) {
		return *d->array;
	}
	invalidArray->clear();
	return *invalidArray;
}

auto JsonValue::toArray(bool* ok) const -> JsonArray {
	if (ok) {
		*ok = isArray();
	}
	if (isArray()) {
		return *d->array;
	}
	return JsonArray();
}

auto JsonValue::constToArray(bool* ok) const -> JsonArray {
	return toArray(ok);
}

auto JsonValue::isObject() const -> bool {
	return d->type == Object;
}

auto JsonValue::toObject(bool* ok) -> JsonObject& {
	if (ok) {
		*ok = isObject();
	}
	if (isObject()) {
		return *d->object;
	}
	invalidObject->clear();
	return *invalidObject;
}

auto JsonValue::toObject(bool* ok) const -> JsonObject {
	if (ok) {
		*ok = isObject();
	}
	if (isObject()) {
		return *d->object;
	}
	return JsonObject();
}

auto JsonValue::constToObject(bool* ok) const -> JsonObject {
	return toObject(ok);
}

auto JsonValue::follow(JsonPath path, bool* ok) -> JsonValue& {
	JsonValue* val = this;
	// follow down the path
	for (auto key : path) {
		if (val->isObject()) {
			if (!key.isObjectKey()) {
				// not the right kind
				if (ok) {
					*ok = false;
				}
				invalidValueTwo->setType(Null);
				return *invalidValueTwo;
			}
			// get the object and key
			QString k = key.toObjectKey();
			JsonObject* obj = &val->toObject();
			if (!obj->contains(k)) {
				// the association isn't there,
				// so quit
				if (ok) {
					*ok = false;
				}
				invalidValueTwo->setType(Null);
				return *invalidValueTwo;
			}
			// get the association's value
			val = &obj->operator[] (k);
		} else if (val->isArray()) {
			if (!key.isArrayIndex()) {
				// not the right kind
				if (ok) {
					*ok = false;
				}
				invalidValueTwo->setType(Null);
				return *invalidValueTwo;
			}
			// get the array and index
			int k = key.toArrayIndex();
			JsonArray* arr = &val->toArray();
			if (k < 0 || k >= arr->count()) {
				// not in range
				if (ok) {
					*ok = false;
				}
				invalidValueTwo->setType(Null);
				return *invalidValueTwo;
			}
			// get the value at that index
			val = &arr->operator[] (k);
		} else {
			// not an array/object
			if (ok) {
				*ok = false;
			}
			invalidValueTwo->setType(Null);
			return *invalidValueTwo;
		}
	}
	if (ok) {
		*ok = true;
	}
	return *val;
}

auto JsonValue::follow(JsonPath path, bool* ok) const -> JsonValue {
	JsonValue val = *this;
	// follow down the path
	for (auto key : path) {
		if (val.isObject()) {
			if (!key.isObjectKey()) {
				// not the right kind
				if (ok) {
					*ok = false;
				}
				return JsonValue::Null;
			}
			// get the object and key
			QString k = key.toObjectKey();
			JsonObject obj = val.toObject();
			if (!obj.contains(k)) {
				// the association isn't there,
				// so quit
				if (ok) {
					*ok = false;
				}
				return JsonValue::Null;
			}
			// get the association's value
			val = obj.value(k);
		} else if (val.isArray()) {
			if (!key.isArrayIndex()) {
				// not the right kind
				if (ok) {
					*ok = false;
				}
				return JsonValue::Null;
			}
			// get the array and index
			int k = key.toArrayIndex();
			JsonArray arr = val.toArray();
			if (k < 0 || k >= arr.count()) {
				// not in range
				if (ok) {
					*ok = false;
				}
				return JsonValue::Null;
			}
			// get the value at that index
			val = arr.at(k);
		} else {
			// not an array/object
			if (ok) {
				*ok = false;
			}
			return JsonValue::Null;
		}
	}
	if (ok) {
		*ok = true;
	}
	return val;
}

auto JsonValue::constFollow(JsonPath path, bool* ok) const -> JsonValue {
	return follow(path, ok);
}

auto JsonValue::create(JsonPath path, bool* ok) -> JsonValue& {
	JsonValue* val = this;
	// follow down the path
	for (auto key : path) {
		if (val->isNull() && key.isObjectKey()) {
			// make it an object
			val->setType(Object);
		} else if (val->isNull() && key.isArrayIndex()) {
			// make it an array
			val->setType(Array);
		}

		if (val->isObject() && key.isObjectKey()) {
			// get and/or make the association
			val = &val->toObject()[key.toObjectKey()];
		} else if (val->isArray() && key.isArrayIndex()) {
			JsonArray* arr = &val->toArray();
			int k = key.toArrayIndex();
			if (k >= 0 && k < arr->count()) {
				// it exists, so just get it
				val = &arr->operator[] (k);
			} else {
				// create and get the value at that index
				if (k < 0) {
					k = 0;
				} else {
					k = arr->count();
				}
				arr->insert(k, Null);
				val = &arr->operator[] (k);
			}
		} else {
			// not the right kind of value
			if (ok) {
				*ok = false;
			}
			invalidValueTwo->setType(Null);
			return *invalidValueTwo;
		}
	}
	if (ok) {
		*ok = true;
	}
	return *val;
}