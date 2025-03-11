#include "Delegate/DelegateFunctionInfo.h"

#include "Misc/MiscHelperFunctions.h"

FString FRockDelegateInfo::GetNameWithClass() const
{
	return (DefiningClass ? DefiningClass->GetName() : "") + "::" + Name;
}

FString FRockDelegateInfo::GetDisplayName() const
{
	// check UDeveloperSettings if we show GetName or GetSignatureFunctionString

	if (false
		//&& GetDefault<URockGameplayEventDeveloperSettings>()->ShowFullFunctionSignature
	)
	{
		return GetSignatureFunctionString();
	}
	else
	{
		return GetName();
	}
}

FString FRockDelegateInfo::GetSignatureFunctionString() const
{
	// get all the parameter types and put them in a foo(int, float, bool) format
	auto parameters = UMiscHelperFunctions::BuildFunctionParameterString(SignatureFunction, true);
	if (parameters.Len() > 0)
	{
		parameters = "(" + parameters + ")";
	}
	const FString SignatureString = Name + parameters;
	return SignatureString;
}

FString FRockDelegateInfo::GetDelegateTypeString() const
{
	switch (DelegateType)
	{
	case ERockDelegateType::MulticastDelegate:
		return "Multicast Delegate";
	case ERockDelegateType::BlueprintDelegate:
		return "Blueprint Delegate";
	default:
		return "Unknown";
	}
}

FString FRockDelegateInfo::GetName() const
{
	return Name;
}

FString FRockFunctionInfo::GetSignatureFunctionString() const
{
	// get all the parameter types and put them in a foo(int, float, bool) format

	auto parameters = UMiscHelperFunctions::BuildFunctionParameterString(Function, false);
	if (parameters.Len() > 0)
	{
		parameters = "(" + parameters + ")";
	}
	const FString SignatureString = Name + parameters;
	return SignatureString;
}
