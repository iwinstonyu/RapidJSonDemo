//
//	通过配置好的json-schema文件对json消息进行校验
//	Copyright (C) 2018. NetDragon. All rights reserved.
//
//	Created: 2018/8/13 17:11:31
//	Version: 1.0
//  Author: WenJin Yu
//

#pragma once
#include <rapidjson/schema.h>
#include <map>
#include <vector>
using namespace std;

namespace lp{

typedef rapidjson::Reader							RJReader;
typedef rapidjson::IRemoteSchemaDocumentProvider	RJIRemoteSchmaProvider;
typedef rapidjson::SizeType							RJSizeType;
typedef rapidjson::SchemaValidator					RJValidator;
typedef rapidjson::SchemaDocument					RJSchemaDoc;
typedef rapidjson::Document							RJDoc;

enum EJsonMsg
{
	EJsonMsg_AttackLand,
};

/// @brief Schema引用
class SchemaProvider : public RJIRemoteSchmaProvider
{
public:
	SchemaProvider();
	~SchemaProvider();

	void AddSchema(const char* uri, RJSchemaDoc* pSchema);
	virtual const RJSchemaDoc* GetRemoteDocument(const char* uri, RJSizeType length);

private:
	map<string, RJSchemaDoc*> schemas_;
};

/// @brief json校验器
/// @details 每个消息对应一个Schema。
class JsonMsgValidator
{
public:
	JsonMsgValidator();
	~JsonMsgValidator();

	/// @brief 初始化要引用的Schema，需要再InitValidator之前调用
	void InitSchemaProvier(const char* uri, const char* pFileName);
	/// @brief 初始化验证器
	void InitValidator(EJsonMsg msgType, const char* pFileName);
	/// @brief
	bool ExistValidator(EJsonMsg msgType);
	/// @brief
	bool IsValidJson(EJsonMsg msgType, string& strJson);

private:
	RJSchemaDoc* InitSchema(const char* pFileName);

private:
	RJReader							reader_;
	map<EJsonMsg, RJValidator*>			validators_;
	SchemaProvider						schemaProvider_;
	vector<RJSchemaDoc*>				validatorSchemas_;		///< validators_不释放Schema，所以需要保存Schema在析构的时候释放。
};

}