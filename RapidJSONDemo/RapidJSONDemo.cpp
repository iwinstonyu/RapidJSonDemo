// RapidJSONDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include <boost/locale/encoding.hpp>

using namespace rapidjson;
using std::string;

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
	string strData = "{\"_id\":\"597202d328b8d7436a22e4f2\",\"roomName\":\"ѪȾȫ�������Լս\",\"startTime\":1500715800,\"deadline\":1500718200,\"password\":\"51109\",\"startMember\":[{\"nickname\":\"����δ����\",\"openid\":\"E35629495DD0C4DF5F8E03708F910FDD\",\"accountid\":\"33585611\"},{\"nickname\":\"ս��ؼ�񡣡���\",\"openid\":\"BD8198CE1351C6169955D10D5E4253DD\",\"accountid\":\"87438862\"},{\"nickname\":\"�¶�~~Ѫ��\",\"openid\":\"16C62C41CC11A1588913404448294C24\",\"accountid\":\"74435171\"},{\"nickname\":\"ţ�ƽⶾ��\",\"openid\":\"325FF7F1160CBFF10FCB30C984418EB7\",\"accountid\":\"67783223\"},{\"nickname\":\"ؿz���\\",\"openid\":\"7D8C7F672309A669AADE84D06DB0748E\",\"accountid\":\"17343844\"}],\"acceptMember\":[{\"nickname\":\"��������\",\"openid\":\"3BA40BE3CFCEB924BD86C17001E00EB3\",\"accountid\":\"27558341\"},{\"nickname\":\"������ȥ\",\"openid\":\"938944DC1A6228134C77EF415205A42D\",\"accountid\":\"27761356\"},{\"nickname\":\"ŭ��\",\"openid\":\"E0480C9B8CA18FC746ADF267CAB4A40F\",\"accountid\":\"88274843\"},{\"nickname\":\"�ƻ�_14630516940\",\"openid\":\"228984B97BC36D4B653EED26D63E857E\",\"accountid\":\"73521256\"},{\"nickname\":\"���؅d�ճ�\",\"openid\":\"C4DA3C9DEA5F6592EE2CCF9C6C992AE4\",\"accountid\":\"77863531\"}]}";
	//string strData = "{\"roomName\":\"rapidjson\",\"stars\":10}";

	string strUtf8 = boost::locale::conv::between(strData, "UTF-8", "GBK");

	string strOne = "{\"num\":\"һ\\\"}";
	string strUtf8One = boost::locale::conv::between(strOne, "UTF-8", "GBK");

	Document d;
	d.Parse(strUtf8One.c_str());

	Value& roomName = d["num"];

	std::cout << boost::locale::conv::between(roomName.GetString(), "GBK", "UTF8") << std::endl;
}

int main() 
{
	MyTest();

	system("pause");
	return 0;
}