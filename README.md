JsonDataTree
============

A JSON library in C++ designed for Qt.

## Idea Behind Development

When I started working on my own project, I ran into a few issues
regarding the currently available JSON libraries:

- Most of them used standard library classes like `std::vector`,
	`std::map` and `std::string`, but I wanted something that
	worked more closely with Qt classes, like `QList`, `QHash`,
	and `QString`.
- I found that most available libraries did not have the kind
	of path system that I would like. I wanted to be able to
	pass a path object of some sort to a JSON value that
	would then return the value at the end of that path.
- For whatever reason, Qt's built-in JSON library does not support
	dynamic changes to the file. You can read in the JSON tree,
	and read all of your data from it, and you can write out the
	tree, but you cannot edit the tree.

I eventually decided to make my own JSON library that would do all
of these things, even if it ended up being less efficient.

## Reasons to Choose this Library

- It is well integrated into Qt, including the Qt property system.
- It is implicitly shared (i.e. copy-on-write).
- It has built-in path support, at least for traversing downward.
- Unlike Qt's built-in JSON support, you can edit the JSON tree
	in memory.

This library is not the most efficient one out there. The reader
and writer both work, but they are not particularly optimized.
The library as a whole could probably use some optimization, as well.
If you have a mission-critical section of code that relies on quick
reading/writing of JSON files, this may not be the library for you.
Eventually, I hope to make the reading/writing efficient, but for
now it is what it is.

## Features

Almost everything is self explanatory and well documented, but
a discussion about paths is in order.

### Paths

A JSON path goes from a value to a child value. It is, essentially,
a list of keys, where the key could be either a string (for objects),
or an integer (for arrays). The idea of a key is represented by
the `JSON::JsonKey` class, and the idea of a path is represented by
the `JSON::JsonPath` class.

Paths are almost exclusively used with `JSON::JsonValue`'s `follow()`
function. Take this section of code, for example:

	QString jsonText = 	"{"
							"\"hello\": {"
								"\"world\": [5]"
							"}"
						"}";
	JSON::JsonReader reader;
	JSON::JsonValue val = reader.parse(jsonText);
	qDebug() << val.follow({"hello", "world", 0}).toInteger();

`val` follows the path by entering the value paired with
the `"hello"` key, then `"world"`, then `0`, and arrives at
the value `5`, which is then printed to the console.

#### Creating Paths

As seen in the above example, one way to create a path is with
an initializer list of `QString`s, `int`s, and cstrings.

Another way would be through the use of a formatted string,
with a special character (`'.'`, by default) acting as a separator
between keys. In the above example, `"hello.world.0"` would yield
the same result.

If you need to manually edit the path, you can create it and use
the various functions in `JSON::JsonPath`, like `append()` and
`prepend()`, to modify the path.

#### A Note on Keys

The `JSON::JsonKey` class can hold keys in the form of a `QString`
or an `int`. When the key is in the form of a `QString`, it can
always act as an object key, and, when it is the string representation
of a number, it can also act as an array index. `int` keys, however,
can only act as array indices.
