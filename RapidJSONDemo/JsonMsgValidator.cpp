//
//	<one line to give the program's name and a brief idea of what it does.>
//	Copyright (C) 2018. NetDragon. All rights reserved.
//
//	Created: 2018/8/13 17:11:23
//	Version: 1.0
//  Author: WenJin Yu
//

#include "stdafx.h"
#include "JsonMsgValidator.h"
#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace lp {

using rapidjson::GetParseError_En;
typedef rapidjson::FileReadStream		RJFileReadStream;
typedef rapidjson::StringStream			RJStringStream;
typedef rapidjson::StringBuffer			RJStringBuffer;
typedef rapidjson::Value				RJVal;

///
/// RemoteSchemaProvider
///
SchemaProvider::SchemaProvider()
{
	//EASYLOG_INF_ENTRY << "SchemaProvider construct";
}

SchemaProvider::~SchemaProvider()
{
	//EASYLOG_INF_ENTRY << "SchemaProvider destruct";

	for (auto it = schemas_.begin(); it != schemas_.end(); ++it) {
		if (it->second)
			delete it->second;
	}
	schemas_.clear();
}

void SchemaProvider::AddSchema(const char* uri, RJSchemaDoc* pSchema)
{
	schemas_[string(uri)] = pSchema;
}

const RJSchemaDoc* SchemaProvider::GetRemoteDocument(const char* uri, RJSizeType length)
{
	string s(uri, length);
	return schemas_.count(s) ? schemas_.at(s) : nullptr;
}

///
/// JsonMsgValidator
///
JsonMsgValidator::JsonMsgValidator()
{
	//EASYLOG_INF_ENTRY << "JsonMsgValidator construct";
}

JsonMsgValidator::~JsonMsgValidator()
{
	//EASYLOG_INF_ENTRY << "JsonMsgValidator destruct";

	for (auto it = validators_.begin(); it != validators_.end(); ++it) {
		if(it->second)
			delete it->second;
	}
	validators_.clear();

	for (auto it = validatorSchemas_.begin(); it != validatorSchemas_.end(); ++it) {
		if (*it)
			delete (*it);
	}
}

void JsonMsgValidator::InitSchemaProvier(const char* uri, const char* pFileName)
{
	RJSchemaDoc* pSchema = InitSchema(pFileName);
	if (!pSchema)
		return;

	schemaProvider_.AddSchema(uri, pSchema);
}

void JsonMsgValidator::InitValidator(EJsonMsg msgType, const char* pFileName)
{
	RJSchemaDoc* pSchema = InitSchema(pFileName);
	if (!pSchema)
		return;

	validatorSchemas_.push_back(pSchema);

	RJValidator* pValidator = DBG_NEW RJValidator(*pSchema);
	validators_[msgType] = pValidator;
}

bool JsonMsgValidator::ExistValidator(EJsonMsg msgType)
{
	return validators_.count(msgType) ? true : false;
}

bool JsonMsgValidator::IsValidJson(EJsonMsg msgType, string& strJson)
{
	if (!validators_.count(msgType))
		return false;
	auto pValidator = validators_.at(msgType);
	pValidator->Reset();

	RJStringStream ss(strJson.c_str());
	if (!reader_.Parse(ss, *pValidator) && reader_.GetParseErrorCode() != rapidjson::kParseErrorTermination) {
		cout << "Input is not a valid JSON: " << reader_.GetErrorOffset() << " " << GetParseError_En(reader_.GetParseErrorCode()) << endl;
		return false;
	}

	if (!pValidator->IsValid()) {
		ostringstream oss;
		oss << "Invalid msg: " << msgType;
		RJStringBuffer sb;
		pValidator->GetInvalidSchemaPointer().StringifyUriFragment(sb);
		oss << "	schema: " << sb.GetString();
		oss << "	keyword: " << pValidator->GetInvalidSchemaKeyword();
		sb.Clear();
		pValidator->GetInvalidDocumentPointer().StringifyUriFragment(sb);
		oss << "	document: " << sb.GetString();
		cout << oss.str() << endl;
		return false;
	}

	return true;
}

RJSchemaDoc* JsonMsgValidator::InitSchema(const char* pFileName)
{
	FILE *pFile = fopen(pFileName, "r");
	if (!pFile) {
		cout << "Fail open file: " << pFileName;
		return nullptr;
	}

	char buffer[8192] = { 0 };
	RJFileReadStream frs(pFile, buffer, sizeof(buffer));
	RJDoc doc;
	doc.ParseStream(frs);
	if (doc.HasParseError()) {
		cout << "Fail parse: " << pFileName << " " << doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError());
		fclose(pFile);
		return nullptr;
	}
	fclose(pFile);

	return DBG_NEW RJSchemaDoc(doc, &schemaProvider_);
}

}