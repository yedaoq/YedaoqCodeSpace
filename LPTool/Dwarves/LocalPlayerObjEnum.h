#pragma once

enum EnumLocalPlayerTables
{
	TBL_FileInfo,
	TBL_Parser,
	TBL_PlayerKernel,
	TBL_GuidInfo,
	TBL_Stream,
	TBL_CodecPin,
	TBL_FilterType,
	TBL_CodecType,
	TBL_Links,
	TBL_PackageLua,
	TBL_ParserCondition,
	TBL_LinkRules,
	TBL_Rules,
	TBL_PackageDepends,
	TBL_CheckBytes,
	TBL_LinkInfo,
	TBL_ParserResult,
	TBL_SourceLink,
	TBL_MediaExtension,
	TBL_SourceConfig,
	TBL_LinkInput,
	TBL_Codecs,
	TBL_PackageIndex,
	TBL_PackageInfo,
	TBL_Option,
	TBL_LinkLink,
	TBL_MediaType,
	TBL_DmlLog,
	TBL_MediaFile,
};

enum EnumFileInfoColumns
{
	COL_FileInfo_FileId,
	COL_FileInfo_FileName,
	COL_FileInfo_FilePath,
	COL_FileInfo_FileSize,
	COL_FileInfo_FileVer,
	COL_FileInfo_FileMd5,
};

enum EnumParserColumns
{
	COL_Parser_ParserId,
	COL_Parser_ParserName,
	COL_Parser_ParseDesc,
};

enum EnumPlayerKernelColumns
{
	COL_PlayerKernel_KernelType,
	COL_PlayerKernel_KernelName,
	COL_PlayerKernel_Desc,
};

enum EnumGuidInfoColumns
{
	COL_GuidInfo_Guid,
	COL_GuidInfo_GuidName,
	COL_GuidInfo_Code,
};

enum EnumStreamColumns
{
	COL_Stream_StreamId,
	COL_Stream_StreamName,
	COL_Stream_StreamMajorType,
	COL_Stream_StreamSubType,
	COL_Stream_FormatType,
};

enum EnumCodecPinColumns
{
	COL_CodecPin_CodecClsid,
	COL_CodecPin_StreamId,
	COL_CodecPin_PinType,
};

enum EnumFilterTypeColumns
{
	COL_FilterType_FilterType,
	COL_FilterType_CodecTypeName,
};

enum EnumCodecTypeColumns
{
	COL_CodecType_CodecType,
	COL_CodecType_CodecTypeName,
};

enum EnumLinksColumns
{
	COL_Links_LinkId,
	COL_Links_LinkName,
	COL_Links_Preconnected,
	COL_Links_Merit,
};

enum EnumPackageLuaColumns
{
	COL_PackageLua_PckId,
	COL_PackageLua_LuaInstall,
	COL_PackageLua_LuaUpdate,
	COL_PackageLua_LuaCheck,
	COL_PackageLua_LuaUninstall,
};

enum EnumParserConditionColumns
{
	COL_ParserCondition_ConditionId,
	COL_ParserCondition_ParserId,
	COL_ParserCondition_Param,
	COL_ParserCondition_Value,
	COL_ParserCondition_Priority,
};

enum EnumLinkRulesColumns
{
	COL_LinkRules_LinkId,
	COL_LinkRules_ConditionId,
	COL_LinkRules_Validation,
	COL_LinkRules_Priority,
};

enum EnumRulesColumns
{
	COL_Rules_RuleId,
	COL_Rules_ConditionId,
	COL_Rules_Validation,
	COL_Rules_Priority,
};

enum EnumPackageDependsColumns
{
	COL_PackageDepends_PckId,
	COL_PackageDepends_DependsPckId,
};

enum EnumCheckBytesColumns
{
	COL_CheckBytes_MediaTypeId,
	COL_CheckBytes_Offset,
	COL_CheckBytes_Length,
	COL_CheckBytes_Mask,
	COL_CheckBytes_Value,
	COL_CheckBytes_IsIgnoreFirst,
	COL_CheckBytes_IgnoreChar,
};

enum EnumLinkInfoColumns
{
	COL_LinkInfo_LinkId,
	COL_LinkInfo_CodecClsid,
	COL_LinkInfo_LinkPosition,
	COL_LinkInfo_StreamId,
};

enum EnumParserResultColumns
{
	COL_ParserResult_ParserId,
	COL_ParserResult_Param,
	COL_ParserResult_ExecuteState,
	COL_ParserResult_Value,
	COL_ParserResult_Desc,
};

enum EnumSourceLinkColumns
{
	COL_SourceLink_PreStepId,
	COL_SourceLink_Value,
	COL_SourceLink_StepId,
	COL_SourceLink_StepType,
	COL_SourceLink_ParserId,
	COL_SourceLink_Param,
	COL_SourceLink_ConfigId,
};

enum EnumMediaExtensionColumns
{
	COL_MediaExtension_MediaTypeId,
	COL_MediaExtension_FileExtension,
};

enum EnumSourceConfigColumns
{
	COL_SourceConfig_ConfigId,
	COL_SourceConfig_KernelType,
	COL_SourceConfig_LinkId,
	COL_SourceConfig_Priority,
};

enum EnumLinkInputColumns
{
	COL_LinkInput_LinkId,
	COL_LinkInput_StreamId,
	COL_LinkInput_Priority,
};

enum EnumCodecsColumns
{
	COL_Codecs_PckId,
	COL_Codecs_FileId,
	COL_Codecs_CodecClsid,
	COL_Codecs_CodecName,
	COL_Codecs_FilterType,
	COL_Codecs_CodecType,
	COL_Codecs_ReleaseTime,
};

enum EnumPackageIndexColumns
{
	COL_PackageIndex_PckId,
	COL_PackageIndex_FileId,
};

enum EnumPackageInfoColumns
{
	COL_PackageInfo_PckId,
	COL_PackageInfo_PckName,
	COL_PackageInfo_Url,
};

enum EnumOptionColumns
{
	COL_Option_Id,
	COL_Option_Name,
	COL_Option_Value,
	COL_Option_Param1,
	COL_Option_Param2,
};

enum EnumLinkLinkColumns
{
	COL_LinkLink_LinkId,
	COL_LinkLink_StreamId,
	COL_LinkLink_NextLinkId,
	COL_LinkLink_RuleId,
	COL_LinkLink_Priority,
};

enum EnumMediaTypeColumns
{
	COL_MediaType_MediaTypeId,
	COL_MediaType_MediaTypeName,
	COL_MediaType_Priority,
};

enum EnumDmlLogColumns
{
	COL_DmlLog_Dbver,
	COL_DmlLog_Command,
	COL_DmlLog_TableName,
	COL_DmlLog_RecordOrigin,
	COL_DmlLog_RecordFresh,
	COL_DmlLog_Datetime,
	COL_DmlLog_User,
};

enum EnumMediaFileColumns
{
	COL_MediaFile_FileId,
	COL_MediaFile_FileName,
	COL_MediaFile_FilePath,
	COL_MediaFile_FileMd5,
	COL_MediaFile_FileSize,
};

