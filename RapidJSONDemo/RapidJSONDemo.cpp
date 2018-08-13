// RapidJSONDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include <boost/locale/encoding.hpp>
#include "rapidjson/error/en.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/schema.h"
#include "rapidjson/stringbuffer.h"
#include <sstream>

using namespace rapidjson;
using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::istringstream;

void Demo()
{
	// 1. Parse a JSON string into DOM.
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document d;
	d.Parse(json);
	// 2. Modify it by DOM.
	Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);
	// 3. Stringify the DOM
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;
}

void MyTest()
{
	string strData = "{\"_id\":\"597202d328b8d7436a22e4f2\",\"roomName\":\"血染全服发起的约战\",\"startTime\":1500715800,\"deadline\":1500718200,\"password\":\"51109\",\"startMember\":[{\"nickname\":\"红颜未曾醉\",\"openid\":\"E35629495DD0C4DF5F8E03708F910FDD\",\"accountid\":\"33585611\"},{\"nickname\":\"战争丶神。。。\",\"openid\":\"BD8198CE1351C6169955D10D5E4253DD\",\"accountid\":\"87438862\"},{\"nickname\":\"孤独~~血狼\",\"openid\":\"16C62C41CC11A1588913404448294C24\",\"accountid\":\"74435171\"},{\"nickname\":\"牛黄解毒丸\",\"openid\":\"325FF7F1160CBFF10FCB30C984418EB7\",\"accountid\":\"67783223\"},{\"nickname\":\"乜z尘颸\",\"openid\":\"7D8C7F672309A669AADE84D06DB0748E\",\"accountid\":\"17343844\"}],\"acceptMember\":[{\"nickname\":\"徐啦啦啦\",\"openid\":\"3BA40BE3CFCEB924BD86C17001E00EB3\",\"accountid\":\"27558341\"},{\"nickname\":\"愁心离去\",\"openid\":\"938944DC1A6228134C77EF415205A42D\",\"accountid\":\"27761356\"},{\"nickname\":\"怒帝\",\"openid\":\"E0480C9B8CA18FC746ADF267CAB4A40F\",\"accountid\":\"88274843\"},{\"nickname\":\"掌魂_14630516940\",\"openid\":\"228984B97BC36D4B653EED26D63E857E\",\"accountid\":\"73521256\"},{\"nickname\":\"独守卍空城\",\"openid\":\"C4DA3C9DEA5F6592EE2CCF9C6C992AE4\",\"accountid\":\"77863531\"}]}";
	//string strData = "{\"roomName\":\"rapidjson\",\"stars\":10}";

	string strUtf8 = boost::locale::conv::between(strData, "UTF-8", "GBK");

	string strOne = "{\"num\":\"一\\\"}";
	string strUtf8One = boost::locale::conv::between(strOne, "UTF-8", "GBK");

	Document d;
	d.Parse(strUtf8One.c_str());

	Value& roomName = d["num"];

	std::cout << boost::locale::conv::between(roomName.GetString(), "GBK", "UTF8") << std::endl;
}

int TestSchema() {
	// Read a JSON schema from file into Document
	Document d;
	char buffer[4096];

	{
		const char* pszSchemaDefs = "JsonSchema/Coord.json";
		FILE *fp = fopen(pszSchemaDefs, "r");
		if (!fp) {
			printf("Schema file '%s' not found\n", pszSchemaDefs);
			return -1;
		}
		FileReadStream fs(fp, buffer, sizeof(buffer));
		d.ParseStream(fs);
		if (d.HasParseError()) {
			fprintf(stderr, "Schema file '%s' is not a valid JSON\n", pszSchemaDefs);
			fprintf(stderr, "Error(offset %u): %s\n",
				static_cast<unsigned>(d.GetErrorOffset()),
				GetParseError_En(d.GetParseError()));
			fclose(fp);
			return EXIT_FAILURE;
		}
		fclose(fp);
	}

	// Then convert the Document into SchemaDocument
	SchemaDocument sd(d);

	// Use reader to parse the JSON in stdin, and forward SAX events to validator
	SchemaValidator validator(sd);
	Reader reader;
// 	FileReadStream is(stdin, buffer, sizeof(buffer));
// 	istringstream iss("{\"x\":1, \"y\":1}");
// 	ostringstream oss;
// 	oss << "{\"x\":1, \"y\":1}";
// 	StringBuffer sb;
// 	Writer<StringBuffer> writer(sb);
	StringStream ss("{\"x\":1}");
	if (!reader.Parse(ss, validator) && reader.GetParseErrorCode() != kParseErrorTermination) {
		// Schema validator error would cause kParseErrorTermination, which will handle it in next step.
		fprintf(stderr, "Input is not a valid JSON\n");
		fprintf(stderr, "Error(offset %u): %s\n",
			static_cast<unsigned>(reader.GetErrorOffset()),
			GetParseError_En(reader.GetParseErrorCode()));
	}

	// Check the validation result
	if (validator.IsValid()) {
		printf("Input JSON is valid.\n");
		return EXIT_SUCCESS;
	}
	else {
		printf("Input JSON is invalid.\n");
		StringBuffer sb;
		validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
		fprintf(stderr, "Invalid schema: %s\n", sb.GetString());
		fprintf(stderr, "Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
		sb.Clear();
		validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
		fprintf(stderr, "Invalid document: %s\n", sb.GetString());
		return EXIT_FAILURE;
	}
}

int main() 
{
	//{
	//	clock_t cur = clock();

	//	for (int i = 0; i < 10000; ++i) {
	//		Value root;
	//		root["territoryOP"]["wood"] = 1;
	//		root["territoryOP"]["iron"] = 1;
	//		root["territoryOP"]["stone"] = 1;
	//		root["territoryOP"]["food"] = 1;

	//		root["buildingOP"]["wood"] = 1;
	//		root["buildingOP"]["iron"] = 1;
	//		root["buildingOP"]["stone"] = 1;
	//		root["buildingOP"]["food"] = 1;

	//		root["allianceCityOP"]["wood"] = 1;
	//		root["allianceCityOP"]["iron"] = 1;
	//		root["allianceCityOP"]["stone"] = 1;
	//		root["allianceCityOP"]["food"] = 1;

	//		root["troopCostOP"]["wood"] = 1;
	//		root["troopCostOP"]["iron"] = 1;
	//		root["troopCostOP"]["stone"] = 1;
	//		root["troopCostOP"]["food"] = 1;

	//		StringBuffer buffer;
	//		Writer<StringBuffer> writer(buffer);
	//		root.Accept(writer);
	//		string str = buffer.GetString();
	//	}

	//	cout << "time cost: " << clock() - cur << endl;
	//	system("pause");
	//}

// 	Value val;
// 	val["hello"] = "world";
// 	int i = val["hello"].GetUint();

	TestSchema();

	system("pause");
	return 0;
}