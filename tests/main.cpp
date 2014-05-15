#include <iostream>
#include <string>

#include "../library/include/JsonValue.h"
#include "../library/include/JsonArray.h"
#include "../library/include/JsonObject.h"
#include "../library/include/JsonWriter.h"

using namespace std;
using namespace JSON;

int main()
{
	// write it out
	JsonWriter writer(object);
	cout << writer.string().toStdString() << endl;
	return 0;
}
