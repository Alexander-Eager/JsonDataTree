#include <iostream>
#include <string>

#include "../library/include/Json.h"

#include <QFile>

using namespace std;
using namespace JSON;

int main()
{
	// read it in
	JsonReaderErrors ok;
	JsonReader reader;
	QFile file("input.json");
	if (!file.open(QFile::ReadWrite))
	{
		return 1;
	}
	JsonValue val = reader.read(&file, &ok);

	// test paths
	JsonWriter writer;
	writer.setData(val.follow({ }));
	cout << "Empty Path:" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow({ "Hello" }));
	cout << "\"Hello\"" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow({ "I am", 1 }));
	cout << "\"I am\" -> 1" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow({ "See you later", "boolean" }));
	cout << "\"See you later\" -> \"boolean\"" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow("I am.0"));
	cout << "I am.0" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow("See you later.number 2"));
	cout << "See you later.number 2" << endl;
	cout << writer.string().toStdString() << endl << endl;

	writer.setData(val.follow({"does not exist"}));
	cout << "does not exist" << endl;
	cout << writer.string().toStdString() << endl << endl;
	return 0;
}
