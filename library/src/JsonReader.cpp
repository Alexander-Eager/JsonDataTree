// header file
#include <JsonDataTree/JsonReader.h>

// internal data
#include <QSharedData>
#include <QVariant>
#include <JsonDataTree/JsonObject.h>
#include <JsonDataTree/JsonArray.h>

#include <iostream>

// private data class
namespace JSON
{
	class JsonReaderPrivate : public QSharedData
	{
		public:
			// read a value from the stream
			JsonValue readValue(QTextStream& stream, JsonReaderErrors* errors) const;

			// read a string from the stream
			QString readString(QTextStream& stream, JsonReaderErrors* errors) const;

			// read a number from the stream
			double readNumber(QTextStream& stream, JsonReaderErrors* errors) const;

			// read an array from the stream
			JsonArray readArray(QTextStream& stream, JsonReaderErrors* errors) const;

			// read an object from the stream
			JsonObject readObject(QTextStream& stream, JsonReaderErrors* errors) const;

			// skip over comments and white space
			void skipNonData(QTextStream& stream, JsonReaderErrors* errors) const;

			// TODO read comments?
	};
}

using namespace JSON;

JsonReader::JsonReader()
	:	d(new JsonReaderPrivate)
	{ }

JsonReader::~JsonReader() { }

JsonReader::JsonReader(const JsonReader& other)
	:	d(other.d)
	{ }

JsonValue JsonReader::parse(QString string, JsonReaderErrors* errors) const
{
	QTextStream stream(&string);
	return read(stream, errors);
}

JsonValue JsonReader::read(QIODevice* io, JsonReaderErrors* errors) const
{
	QTextStream stream(io);
	return read(stream, errors);
}

JsonValue JsonReader::read(QTextStream& stream, JsonReaderErrors* errors) const
{
	// skip preceding white space and comments
	d->skipNonData(stream, errors);
	if (errors && errors->numErrors())
	{
		return JsonValue::Null;
	}
	stream.setIntegerBase(10);
	// read in the value
	JsonValue ans = d->readValue(stream, errors);
	// set *ok if required
	return ans;
}

JsonValue JsonReaderPrivate::readValue(QTextStream& stream,
								JsonReaderErrors* errors) const
{
	// get the first character
	QChar firstChar;
	stream >> firstChar;
	ushort c = firstChar.unicode();
	int offset = stream.pos();
	// but move back, because it could be important
	if (!stream.seek(stream.pos() - 1))
	{
		if (errors)
		{
			errors->addError(JsonReaderError::StreamFailure,
								offset);
		}
		return JsonValue::Null;
	}
	// determine what to do based on that char
	JsonValue ans;
	switch (c)
	{
		case '{': // object
			ans = readObject(stream, errors);
			break;
		case '[': // array
			ans = readArray(stream, errors);
			break;
		case '\"': // string
			ans = readString(stream, errors);
			break;
		case '0': case '1': case '2':
		case '3': case '4': case '5':
		case '6': case '7': case '8':
		case '9': case '-': // numbers
			ans = readNumber(stream, errors);
			break;
		case 'f': // false
			if (stream.read(5) == "false")
			{
				ans = false;
			}
			else if (errors)
			{
				errors->addError(JsonReaderError::UnknownLiteral,
									offset);
			}
			break;
		case 't': // true
			if (stream.read(4) == "true")
			{
				ans = true;
			}
			else if (errors)
			{
				errors->addError(JsonReaderError::UnknownLiteral,
									offset);
			}
			break;
		case 'n': // null
			if (stream.read(4) == "null")
			{
				ans = JsonValue::Null;
			}
			else if (errors)
			{
				errors->addError(JsonReaderError::UnknownLiteral,
									offset);
			}
			break;
		default: // unrecognizable
			if (errors)
			{
				errors->addError(JsonReaderError::UnknownLiteral,
									offset);
			}
			break;
	}
	return ans;
}

QString JsonReaderPrivate::readString(QTextStream& stream,
								JsonReaderErrors* errors) const
{
	int offset = stream.pos();
	QString ans;
	QTextStream output(&ans);
	QChar curr;
	stream >> curr;
	if (stream.status())
	{
		// error after reading the first quote
		if (errors)
		{
			errors->addError(JsonReaderError::StreamFailure,
								offset);
		}
		return ans;
	}
	// get the next character afterward
	stream >> curr;
	while (!stream.status() && curr != '\"')
	{
		ushort c = curr.unicode();
		switch (c)
		{
			case '\\':
				stream >> curr;
				c = curr.unicode();
				switch (c)
				{
					case '\"': // single quote
						output << QChar('\"');
						break;
					case '\\': // backslash
						output << QChar('\\');
						break;
					case '/': // forward slash
						output << QChar('/');
						break;
					case 'b': // backspace
						output << QChar('\b');
						break;
					case 'f': // formfeed
						output << QChar('\f');
						break;
					case 'n': // newline
						output << QChar('\n');
						break;
					case 'r': // linefeed
						output << QChar('\r');
						break;
					case 't': // tab
						output << QChar('\t');
						break;
					case 'u': // 4-letter unicode hex
						// get the hex
						bool good;
						int hexOffset = stream.pos();
						ushort number = stream.read(4).toInt(&good, 16);
						if (!good)
						{
							if (errors)
							{
								errors->addError(JsonReaderError::StringWithBadHex,
													hexOffset);
							}
							return ans;
						}
						// output the corresponding char
						output << QChar(number);
						break; 
				} // end escape switch statement
				break;
			default: // some random character
				output << curr;
				break;

		} // end main switch statement

		stream >> curr;

	} // end while loop

	// if we didn't reach the end of the string,
	// signal the error
	if (curr != '\"' && errors)
	{
		errors->addError(JsonReaderError::StringWithNoClosingQuote,
							offset);
	}

	return ans;
}

double JsonReaderPrivate::readNumber(QTextStream& stream,
										JsonReaderErrors* errors) const
{
	int offset = stream.pos();
	double ans;
	stream >> ans;
	if (stream.status() && !stream.atEnd() && errors)
	{
		errors->addError(JsonReaderError::NumberWithBadCharacter,
							offset);
	}
	return ans;
}

JsonArray JsonReaderPrivate::readArray(QTextStream& stream,
										JsonReaderErrors* errors) const
{
	JsonArray ans;

	// get rid of the first [
	int arrayStart = stream.pos();
	QChar curr;
	stream >> curr;

	// now skip white space and comments
	skipNonData(stream, errors);
	if (errors && errors->numErrors())
	{
		return ans;
	}

	// check for empty array
	stream >> curr;
	if (curr == ']')
	{
		return ans;
	}
	if (!stream.seek(stream.pos() - 1))
	{
		if (errors)
		{
			errors->addError(JsonReaderError::StreamFailure,
								stream.pos());
		}
		return ans;
	}

	// read in values until the ]
	while (!stream.status())
	{
		// now skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;
		
		// read in the value
		JsonValue toAdd = readValue(stream, errors);
		if (errors && errors->numErrors()) return ans;
		ans << toAdd;

		// skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// get the next character
		stream >> curr;
		if (curr == ',')
		{
			// skip to the next value
			skipNonData(stream, errors);
			if (errors && errors->numErrors()) return ans;

			// get the next char
			stream >> curr;
			if (curr == ']')
			{
				// , and ] can't happen next to each other
				if (errors)
				{
					errors->addError(JsonReaderError::ArrayWithExtraComma,
										stream.pos());
				}
				return ans;
			}
			if (!stream.seek(stream.pos() - 1))
			{
				if (errors)
				{
					errors->addError(JsonReaderError::StreamFailure,
										stream.pos());
				}
				return ans;
			}
		}
		else
		{
			if (curr != ']')
			{
				// wasn't a separator,
				// so we had to be done
				if (errors)
				{
					errors->addError(JsonReaderError::ArrayWithNoClosingBracket,
										arrayStart);
				}
				return ans;
			}
			break;
		}
		// move back, since that char is important
		if (!stream.seek(stream.pos() - 1))
		{
			if (errors)
			{
				errors->addError(JsonReaderError::StreamFailure,
									stream.pos());
			}
			return ans;
		}
	}

	return ans;
}

JsonObject JsonReaderPrivate::readObject(QTextStream& stream,
											JsonReaderErrors* errors) const
{
	JsonObject ans;
	
	// get rid of the first {
	int objectStart = stream.pos();
	QChar curr;
	stream >> curr;

	// now skip white space/comments
	skipNonData(stream, errors);
	if (errors && errors->numErrors()) return ans;

	// check for empty object
	stream >> curr;
	if (curr == '}')
	{
		return ans;
	}
	if (!stream.seek(stream.pos() - 1))
	{
		if (errors)
		{
			errors->addError(JsonReaderError::StreamFailure,
								stream.pos());
		}
		return ans;
	}

	// read in values until the }
	while (!stream.status())
	{
		// now skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// read in the key
		QString key = readString(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// now skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// read in the :
		stream >> curr;
		if (curr != ':')
		{
			if (errors)
			{
				errors->addError(JsonReaderError::ObjectWithMissingColon,
									stream.pos());
			}
			break;
		}

		// now skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// read in the value
		JsonValue value = readValue(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// add key-value pair
		ans.insert(key, value);

		// now skip white space/comments
		skipNonData(stream, errors);
		if (errors && errors->numErrors()) return ans;

		// check the next character
		stream >> curr;
		if (curr == ',')
		{
			// now skip white space/comments
			skipNonData(stream, errors);
			if (errors && errors->numErrors()) return ans;

			// check the next character
			stream >> curr;
			if (curr == '}')
			{
				// , and } can't happen next to each other
				if (errors)
				{
					errors->addError(JsonReaderError::ObjectWithExtraComma,
										stream.pos());
				}
				return ans;
			}
			if (!stream.seek(stream.pos() - 1))
			{
				if (errors)
				{
					errors->addError(JsonReaderError::StreamFailure,
										stream.pos());
				}
				return ans;
			}
		}
		else
		{
			if (curr != '}')
			{
				// since there was no comma,
				// we had to be done
				if (errors)
				{
					errors->addError(JsonReaderError::ObjectWithNoClosingBrace,
										objectStart);
				}
			}
			break;
		}
		if (curr != '\"')
		{
			// has to be a string
			if (errors)
			{
				errors->addError(JsonReaderError::ObjectWithNonStringKey,
									stream.pos());
			}
			return ans;
		}
		// move back, since that char is important
		else if (!stream.seek(stream.pos() - 1))
		{
			if (errors)
			{
				errors->addError(JsonReaderError::StreamFailure,
									stream.pos());
			}
			return ans;
		}
	}

	return ans;
}

void JsonReaderPrivate::skipNonData(QTextStream& stream,
									JsonReaderErrors* errors) const
{
	// skip preceding whitespace
	stream.skipWhiteSpace();
	int commentStart = stream.pos();
	QChar curr;
	stream >> curr;
	while (!stream.status() && curr == '/')
	{
		// get the comment type
		stream >> curr;
		if (curr == '/') // single line comment
		{
			// skip over the line
			stream.readLine();
		}
		else if (curr == '*') /* block comment */
		{
			// go until after the trailing */
			QChar lastOne;
			stream >> lastOne;
			bool quitWell = false;
			while (!stream.status())
			{
				if (lastOne == '*')
				{
					stream >> lastOne;
					if (lastOne == '/')
					{
						quitWell = true;
						break;
					}
				}
				else
				{
					stream >> lastOne;
				}
			}
			if (!quitWell)
			{
				if (errors)
				{
					errors->addError(JsonReaderError::CommentWithNoEnd,
										commentStart);
				}
			}
		}
		// skip white space
		stream.skipWhiteSpace();
		// get the next char
		stream >> curr;
	}
	// unget the last char
	if (!stream.seek(stream.pos() - 1))
	{
		if (errors)
		{
			errors->addError(JsonReaderError::StreamFailure,
								stream.pos());
		}
	}
}

// The rest of this deals with the specifics of the error classes

namespace JSON
{
	class JsonReaderErrorPrivate : public QSharedData
	{
		public:
			JsonReaderError::ErrorType type;
			int offset;
			QString message;

			static const QString messages[16];
	};

	class JsonReaderErrorsPrivate : public QSharedData
	{
		public:
			QList<JsonReaderError> errors;
	};
}

const QString JsonReaderErrorPrivate::messages[16] = {
	"No Error; offset %1 specified",
	"Misplaced character in number starting at %1.",
	"Unrecognized escape sequence at %1.",
	"Bad hexadecimal number for \"\\u\" at %1.",
	"String starting at %1 missing closing quote.",
	"Literal (i.e. \"true\", \"false\", or \"null\") "
		"starting at %1 not recognized.",
	"Missing colon for key-value pair before %1.",
	"Missing comma before key-value pair at %1.",
	"Extra comma in object at %1.",
	"Missing closing brace to match one at %1.",
	"Key-value pair in object at %1 is not a key.",
	"Missing comma in array before %1.",
	"Extra comma in array at %1.",
	"Missing closing bracket to match one at %1.",
	"Block comment starting at %1 does not have closing \"*/\"",
	"The stream failed at offset %1."
};

JsonReaderError::JsonReaderError(JsonReaderError::ErrorType type,
									int offset)
	:	d(new JsonReaderErrorPrivate)
{
	d->type = type;
	d->offset = offset;
	d->message = d->messages[type].arg(QString::number(offset));
}

JsonReaderError::~JsonReaderError() { }

JsonReaderError::JsonReaderError(const JsonReaderError& other)
	:	d(other.d)
	{ }

JsonReaderError& JsonReaderError::operator= (const JsonReaderError& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

JsonReaderError::ErrorType JsonReaderError::type() const
{
	return d->type;
}

QString JsonReaderError::message() const
{
	return d->message;
}

int JsonReaderError::offset() const
{
	return d->offset;
}

JsonReaderErrors::JsonReaderErrors()
	:	d(new JsonReaderErrorsPrivate)
	{ }

JsonReaderErrors::~JsonReaderErrors() { }

JsonReaderErrors::JsonReaderErrors(const JsonReaderErrors& other)
	:	d(other.d)
	{ }

JsonReaderErrors& JsonReaderErrors::operator= (const JsonReaderErrors& other)
{
	if (this == &other) return *this;
	d = other.d;
	return *this;
}

int JsonReaderErrors::numErrors() const
{
	return d->errors.count();
}

JsonReaderError JsonReaderErrors::get(int i) const
{
	if (i < 0 || i >= numErrors())
	{
		return JsonReaderError(JsonReaderError::NoError, -1);
	}
	return d->errors.at(i);
}

JsonReaderErrors::const_iterator JsonReaderErrors::begin() const
{
	return d->errors.begin();
}

JsonReaderErrors::const_iterator JsonReaderErrors::end() const
{
	return d->errors.end();
}

void JsonReaderErrors::addError(JsonReaderError error)
{
	d->errors.append(error);
}

void JsonReaderErrors::addError(JsonReaderError::ErrorType type, int offset)
{
	addError(JsonReaderError(type, offset));
}