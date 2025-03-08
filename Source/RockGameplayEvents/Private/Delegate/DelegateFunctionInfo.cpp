#include "Delegate/DelegateFunctionInfo.h"

#include "Misc/MiscHelperFunctions.h"

FText FRockDelegateInfo::GetNameWithClass() const
{
	return FText::FromString((DefiningClass ? DefiningClass->GetName() : "") + "::" + Name);
}

FText FRockDelegateInfo::GetDisplayName() const
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

FText FRockDelegateInfo::GetSignatureFunctionString() const
{
	// get all the parameter types and put them in a foo(int, float, bool) format

	auto parameters = UMiscHelperFunctions::BuildFunctionParameterString(SignatureFunction, false);
	if (parameters.Len() > 0)
	{
		parameters = "(" + parameters + ")";
	}
	const FString SignatureString = Name + parameters;
	return FText::FromString(SignatureString);
}

FText FRockDelegateInfo::GetName() const
{
	return FText::FromString(Name);
}
